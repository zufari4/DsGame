# DS Game 

This is a 2D game showing the DS company logo. The scene makes fun of a situation that shows how we struggle with bugs every day. All objects on the scene have the correct physical behavior model.

![demo](https://github.com/zufari4/DsGame/assets/5335933/c630f8e2-8e64-4fbd-88b4-a68369de172b)

# Dependencies

* SDL2
* SDL2_ttf
* chipmink
* freetype
* libpng16
* brotli
* zlib
* bzip2

# Build

I tried to build only on Windows, since the project uses SDL and VKPKG, I think build on Linux will not pose any problems.

## Steps to build
```
git clone git@github.com:zufari4/DsGame.git
git clone git@github.com:microsoft/vcpkg.git
cd vcpkg
setx VCPKG_ROOT %cd%
bootstrap-vcpkg.bat
vcpkg install SDL2:x64-windows-static
vcpkg install SDL2-ttf:x64-windows-static
vcpkg install chipmunk:x64-windows-static
cd ../DsGame

Open solution file DsGame.sln via Visual Studio
```
