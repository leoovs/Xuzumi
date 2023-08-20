# Xuzumi Engine

The Xuzumi Engine is a(_nother_) game engine with a primary focus on being
cross-platform and flexible. Its goal is to offer support for multiple graphics
APIs, C# scripting, and a user-friendly, convenient editor.

## Feature support

Currently, the latest features are available in and merged into the `dev` branch.
This is due to the fact that the project lacks documentation and testing.
The `master` branch will be updated as soon as those issues are resolved.

## Build and Run
### Prerequisites 

The project heavily relies on __CMake__ as a cross-platform build tool.
Start by obtaining its latest release.
In order to manipulate the repository versions, install __git__.
Building the project requires a properly configured __C++ toolchain__.
Three major compilers supported:

- __GCC__
- __Clang__
- __MSVC__

#### Specifically for Windows builds

When building Xuzumi on Windows, the __Windows SDK__ is required.
The best way to get the latest versions of GCC and Clang is through the __MSYS2__
environment. While __MSYS2__ is not required when building with MSVC,
__Visual Studio__ is.

### Instructions

1. Start by cloning the repository and its submodules: `git clone --recursive
https://github.com/leoovs/Xuzumi`.
2. Create the `build` folder in the repository root.
3. In the repository root generate project files: `cmake -B "build"` or `cmake -B
"build" -G "<Preferred Generator>"`. By default, CMake builds a debug version
of the project. If a release build is required, explicitly state it by adding
the following flag: `-D CMAKE_BUILD_TYPE=Release`.
4. Build the project: `cmake --build "build"`. After executing this command,
CMake performs an _Out-of-source_ build, so that all the baked stuff goes into
the `build` folder.