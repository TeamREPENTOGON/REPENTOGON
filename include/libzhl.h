
#if defined(libzhl_EXPORTS)
#define LIBZHL_API __declspec(dllexport)
#define LIBZHL_API_GLOBS __declspec(dllexport)
#else
#define LIBZHL_API __declspec(dllimport)
#define LIBZHL_API_GLOBS __declspec(dllimport)
#endif

#define MOD_EXPORT extern "C" __declspec(dllexport)