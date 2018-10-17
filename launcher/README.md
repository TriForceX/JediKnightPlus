# Jedi Knight Plus Launcher
Simple tool to launch **Jedi Knight Plus Mod** dedicated server.

# About
This is an additional tool (not required) i made a lot of years ago to learn some **C++**. The purpose was to improve the way to load commands before server launch (like the bash method). This was made to work on *Windows* only (but it can be easily ported to *Linux*) and works together with **JK2 Binaries** (jk2mp.exe or jk2ded.exe) or any server executables and mods.

# Build
Due i reduced some code and changed few functions, to generate the main file `jkplus_launcher.exe` you should use *Visual Studio C++ 11 or higher*. To build it just follow the next steps:

1. Create a `Win32 Console Application` Project called `jkplus_launcher`

2. Previous to finish setup, choose `Console Application` and in additional options check `Empty Project` only

3. Now you need to `Add Existing Item` to project folders: 
   - Add `jkplus_launcher.cpp` to `Source Files`
   - Add `jkplus_launcher.h` to `Headers Files`
   - Add `jkplus_launcher.rc` to `Resource Files`

4. Next you need to statically link the runtime to the project by setting the **/MT** flag. To do this go to `Project > Configuration Properties > C/C++ > Code Generation > Runtime Library` and make sure to set it for the `Release` configuration

5. Finally to make this compatible with *Windows XP* go to `Project > Configuration Properties > General` and choose `Visual Studio <Version> - Windows XP` in `Platform Toolset`

5. That's it! Now you can build `jkplus_launcher.exe` to launch *Jedi Knight Plus Mod*

6. **Note**: This executable creates a file called `jkplus_launcher.cfg` to store the launch options