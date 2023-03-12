/*#include "Isaac.h"

static std::vector<std::string> g_Paths;

void ZHL::AddFilesystemPath(const char *path)
{
	char tmp[256];
	strcpy_s(tmp, path);

	char *p = tmp + strlen(tmp) - 1;
	while(p >= tmp && (*p == '\\' || *p =='/'))
		*(p--) = 0;

	if(p < tmp) return;

	for(p = tmp ; *p ; ++p)
	{
		if(*p == '\\') *p = '/';
		if(*p >= 'A' && *p <= 'Z') *p -= ('A' - 'a');
	}

	g_Paths.push_back(tmp);
	//printf("added filesystem path '%s'\n", tmp);
}

/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// Sylmir warning: No calling convention given to HOOK_STATIC because I don't know which one to give
/// This may result in weird compilation errors !

HOOK_STATIC(FileManager, OpenRead, (const char *path, bool cached) -> FileBase*)
{
	char tmp[256];
	for(auto it = g_Paths.begin() ; it != g_Paths.end() ; ++it)
	{
		sprintf_s(tmp, "%s/%s", it->c_str(), path);
		FileBase *f = super(tmp, cached);
		if(f) return f;
	}

	return super(path, cached);
}
*/