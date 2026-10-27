/* This script is used to generate the implement of `super` from a ZHL-aware
 * library. This is used when building ZHL-aware libraries using clang-cl with
 * a target of i686-windows-msvc.
 *
 * Normally, msvc does not allow __declspec(naked) to appear on member function
 * definitions (both static and non-static). In practice, that rule is not
 * enforced. clang-cl, however, enforces it. As ZHL relies extensively on
 * __declspec(naked) to implement trampolines, we need to work around that
 * limitation.
 *
 * When building under clang-cl, ZHL does not implement super() in a hook in
 * any way. The function is declared, but not defined. This script takes as
 * input every single object file that is a part of the library and scans their
 * symbol table for undefined 'super*' symbols. It then generates a single .asm
 * file that implements every single super.
 */

#ifdef _MSC_VER
#  define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>

#include <cstdio>

#include <algorithm>
#include <atomic>
#include <fstream>
#include <future>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct mangled_name {
    /* Full name used in the assembly. */
    std::string name;
    /* The namespace in which super leaves. This is used to generate the mangled
     * name of the function that needs to be called to get access to super.
     */
    std::string hook_namespace;
    std::string anonymous_namespace;
};

static std::wstring quote(std::wstring const& src) {
  return L"\"" + src + L"\"";
}

static std::wstring narrow_utf8_to_wide(std::string const& s) {
    if (s.empty())
      return std::wstring();

    int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
	s.c_str(), -1, NULL, 0);
    DWORD lastError = GetLastError();
    if (len == 0 && lastError != 0) {
	fprintf(stderr, "[ERROR] Unable to convert %s to a wide char (%lu)\n",
	    s.c_str(), lastError);
	throw std::runtime_error("Unable to convert");
    }

    wchar_t* wide = (wchar_t*)calloc(len + 1, sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s.c_str(), -1,
	wide, len);
    std::wstring w(wide);
    return w;
}

std::string run_dumpbin(const char* file, std::wstring const& dumpbin) {
    std::wstring wide_file = narrow_utf8_to_wide(file);
    std::wostringstream stream;
    stream << quote(dumpbin) << L" " << L"/SYMBOLS " << quote(wide_file);
    std::wstring cmd = stream.str();

    STARTUPINFOW startupInfo;
    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInfo;
    memset(&processInfo, 0, sizeof(processInfo));

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = true;

    HANDLE readPipe, writePipe;
    if (CreatePipe(&readPipe, &writePipe, &sa, 0) == 0) {
	fprintf(stderr, "[FATAL] Error while creating pipe between generator and dumpbin: %lu\n",
		GetLastError());
	exit(-1);
    }

    SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0);

    startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    startupInfo.hStdOutput = writePipe;
    startupInfo.hStdError = writePipe;
    startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    printf("Running dumpbin as %ls with params %ls\n", dumpbin.c_str(),
	   cmd.c_str());
    fflush(stdout);

    BOOL ok = CreateProcessW(dumpbin.c_str(), cmd.data(), NULL, NULL, TRUE,
			     0, NULL, NULL, &startupInfo, &processInfo);
    if (!ok) {
	CloseHandle(readPipe);
	fprintf(stderr, "[FATAL] Unable to invoke dumpbin in the ASM generator: %lu\n",
		GetLastError());
	exit(-1);
    }

    CloseHandle(writePipe);
    std::string out;
    char buffer[4096];
    DWORD n = 0;

    while (ReadFile(readPipe, buffer, sizeof(buffer), &n, NULL) && n > 0) {
	out.append(buffer, n);
    }

    DWORD wait = WaitForSingleObject(processInfo.hProcess, INFINITE);
    if (wait != 0) {
	if (wait == WAIT_FAILED) {
	    fprintf(stderr, "[ERROR] Unable to WaitForSingleObject: %lu\n", GetLastError());
	} else {
	    fprintf(stderr, "[WARN] WaitForSingleObject: %lu\n", wait);
	}
    }

    DWORD dumpbinExitCode = 2000;
    if (GetExitCodeProcess(processInfo.hProcess, &dumpbinExitCode) == 0) {
	fprintf(stderr, "[ERROR] Unable to GetExitCodeProcess: %lu\n", GetLastError());
    }
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    CloseHandle(readPipe);

    if (dumpbinExitCode != 0) {
	fprintf(stderr, "[FATAL] Error while invoking dumpbin: %lu\n",
		dumpbinExitCode);
	exit(-1);
    }

    printf("Done running dumpbin\n");
    fflush(stdout);
    return out;
}

const char* super_needle = "?super@wrapper@Hook_";

void process_dumpbin(std::string const& output,
		     std::vector<mangled_name>& names) {
    std::unique_ptr<char[]> _ptr(new char[output.size() + 1]);

    char* ptr = _ptr.get();
    strcpy(ptr, output.c_str());
    char* current = ptr;

    while ((current = strchr(ptr, '\n'))) {
	*current = '\0';
	if (*(current - 1) == '\r')
	    *(current - 1) = '\0';

	char* super = strstr(ptr, super_needle);
	if (!super) {
	    ptr = current + 1;
	    continue;
	}

	char* start = super + strlen(super_needle);
	char* end = nullptr;
	errno = 0;

	long result = strtol(start, &end, 10);
	if (end != start && *end == '@') {
	    char* space = strchr(start, ' ');
	    if (!space) {
		fprintf(stderr, "[FATAL] Inconsistent state, should have found a space\n");
		exit(-1);
	    }

	    char* anonymous_namespace = strchr(end + 1, '@');
	    *space = '\0';
	    mangled_name name;
	    name.name = std::string(super);
	    name.hook_namespace = std::string(start, end);
	    name.anonymous_namespace = std::string(end + 1, anonymous_namespace);
	    names.push_back(std::move(name));
	}

	ptr = current + 1;
    }
}

void process_file(const char* file, std::wstring const& dumpbin,
	std::vector<mangled_name>& names) {
    std::string output = run_dumpbin(file, dumpbin);
    size_t before = names.size();
    process_dumpbin(output, names);
    size_t after = names.size();
    printf("Read %lu from %s (%lu -> %lu)\n", after - before, file, before, after);
}

void generate_assembly(std::vector<mangled_name> const& names,
	std::string output) {
    FILE* f = fopen(output.c_str(), "w");
    if (!f) {
	fprintf(stderr, "[FATAL] Unable to open '%s' for writing\n",
		output.c_str());
	exit(-1);
    }

    fprintf(f, "bits 32\n\n");
    for (mangled_name const& name: names) {
	int hook_id;
	sscanf(name.hook_namespace.c_str(), "%d", &hook_id);

	fprintf(f, "segment .data\n"
		   "global ?internalSuper@Hook_%d@%s@@3PAXA\n"
		   "?internalSuper@Hook_%d@%s@@3PAXA dd 0\n"
		   "segment .text\n"
		   "global %s\n"
		   "%s:\n"
		   "  jmp ?internalSuper@Hook_%d@%s@@3PAXA\n"
		   "\n",
		hook_id,
		name.anonymous_namespace.c_str(),
		hook_id,
		name.anonymous_namespace.c_str(),
		name.name.c_str(),
		name.name.c_str(),
		hook_id,
		name.anonymous_namespace.c_str());
    }

    fclose(f);
}

void read_file_list(std::string const& file, std::vector<std::string>& files) {
    std::ifstream stream(file);
    if (!stream) {
	fprintf(stderr, "[FATAL] Cannot open file list file '%s'\n", file.c_str());
	exit(-1);
    }

    while (!stream.eof()) {
	std::string line;
	std::getline(stream, line, '\n');

	if (line.empty())
	    continue;

	if (line[0] == '/') {
	    line = "Z:\\" + line;
	}

	files.push_back(std::move(line));
    }
}

std::vector<mangled_name> process_files_thread(std::atomic<size_t>* counter,
	std::wstring const& dumpbin, std::vector<std::string> const& names) {
    size_t s = names.size();
    std::vector<mangled_name> mangled_names;
    size_t x;
    while ((x = counter->fetch_add(1, std::memory_order_relaxed)) < s) {
	process_file(names[x].c_str(), dumpbin, mangled_names);
    }

    return mangled_names;
}

int main(int argc, char** argv) {
    std::vector<std::string> files;
    std::vector<mangled_name> names;
    std::string output, toolchain, file_list;

    for (int i = 1; i < argc; ++i) {
	const char* arg = argv[i];
	const char* eq = strchr(arg, '=');

	if (eq) {
	    ptrdiff_t size = eq - arg;
	    if (!strncmp(arg, "--output", size)) {
		output = eq + 1;
	    } else if (!strncmp(arg, "--toolchain", size)) {
		toolchain = eq + 1;
	    } else if (!strncmp(arg, "--file-list", size)) {
		file_list = eq + 1;
	    }
	} else {
	    files.push_back(arg);
	}
    }

    if (file_list.empty()) {
	fprintf(stderr, "[FATAL] Cannot generate assembly from 0 object files\n");
	exit(-1);
    }

    read_file_list(file_list, files);

    if (output.empty())
	output = "stdout";

    std::wstring dumpbin = narrow_utf8_to_wide(toolchain) + L"\\dumpbin.exe";

    std::atomic<size_t> counter;
    counter.store(0, std::memory_order_relaxed);

    unsigned int concurrency = std::thread::hardware_concurrency();
    if (concurrency == 0) {
	concurrency = 4;
    }

    std::vector<std::future<std::vector<mangled_name>>> threads;
    for (int i = 0; i < concurrency; ++i) {
	threads.push_back(std::async(
		    std::launch::async,
		    process_files_thread,
		    &counter,
		    std::cref(dumpbin),
		    std::cref(files)
	));
    }

    for (int i = 0; i < concurrency; ++i) {
	std::vector<mangled_name> result = threads[i].get();
	std::copy(result.begin(), result.end(), std::back_inserter(names));
    }

    generate_assembly(names, output);

    printf("Done\n");
    fflush(stdout);
    return 0;
}
