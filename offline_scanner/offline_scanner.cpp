#include <cstdio>

#include "injector/error.h"
#include "injector/injector.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Must give the path to the executable to scan "
			"(drag-and-drop is supported and encouraged)\n");
		return -1;
	}

	HANDLE thread = NULL;
	HANDLE process = NULL;
	int result = injector_inject(argv[1], NULL, 250900, "libzhl.dll",
		"OfflineScan", NULL, 0, &thread, &process);
	if (result != INJECT_RESULT_OK) {
		fprintf(stderr, "Error while injecting libzhl.dll in Isaac\n");
		int error = injector_error();
		if (error == INJECT_RESULT_LAST_ERROR) {
			DWORD last_error = GetLastError();
			fprintf(stderr, "Error occured in %s with GetLastError() = %d\n",
				injector_win32_fn(), last_error);
		} else {
			fprintf(stderr, "Error occured in injector: %s\n",
				injector_strerror(error));
		}

		system("pause");
		return -1;
	}

	printf("Done\n");
	system("pause");
	CloseHandle(thread);
	CloseHandle(process);
	return 0;
}