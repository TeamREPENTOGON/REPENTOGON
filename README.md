![logo](assets/logo.gif)
## A new era for Isaac modding begins now.

REPENTOGON is a mod for *The Binding of Isaac: Repentance* version v1.7.9b.J835 (untested and likely unsupported on other versions) that extends the Lua API with much-needed bug fixes, extra functionality, and performance enhancements.

Generally referred to as an "EXE mod" by the community, REPENTOGON works very differently than a traditional mod. Powered by *LibZHL*, the same framework used by *[Antibirth](https://antibirth.com/)*, REPENTOGON hooks directly into the game, allowing direct control over functionality which was previously impossible to replicate in mods, or would require significant  performance-intensive hacks or rewrites to replicate.

# Building
(Unless you're a developer, we recommend grabbing a build from [GitHub Actions](https://github.com/IsaacZHL/IsaacZHL/actions) instead.)
### Requirements
The nature of this project requires that we use the same compiler as the game. As a result, a Windows system is explicitly required, along with the following:
* CMake 3.13 or above
* Git
* Visual Studio
* Doxygen (for now, looking into removing this in the future)

### Steps
We assume Git Bash for the duration of this tutorial, as well as CMake GUI.
1. *Recursively* clone the repository: `git clone --recursive https://github.com/IsaacZHL/IsaacZHL`
2. Launch CMake.
3. Under "Where is the source code", navigate to the root directory of the cloned repository (in most cases named IsaacZHL).
4. Under "Where to put the binaries", choose any folder. This is where the generated files will live.
![cmake1](assets/cmake1.png)
5. At the bottom of the CMake GUI, hit "Configure".
    * If this is the first build, you will be prompted for more information. 
    * Compiler must match your Visual Studio version.
    * Platform **MUST** be Win32.
    * Leave all other options as default, and press Finish.
    ![cmake2](assets/cmake2.png)
6. Once configuration is done, hit "Generate". This will create a .sln in the folder you specified before.
7. Open the generated .sln in Visual Studio.
8. Build the project. Unless you're a developer, we recommend Release mode for performance.
9. When the build is finished, copy `resources`, `dsound.dll`, `libzhl.dll`, `zhlModTest.dll`, and `Lua5.3.3r.dll` to the game's folder. Overwrite `Lua5.3.3r.dll` when prompted.