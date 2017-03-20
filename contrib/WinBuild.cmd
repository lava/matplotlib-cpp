@echo off
@setlocal EnableDelayedExpansion

if NOT DEFINED MSVC_VERSION set MSVC_VERSION=14
if NOT DEFINED CMAKE_CONFIG set CMAKE_CONFIG=Release
if NOT DEFINED PYTHONHOME   set PYTHONHOME=C:/Users/%username%/Anaconda3

if "%MSVC_VERSION%"=="14" (
    if "%processor_architecture%" == "AMD64" (
        set CMAKE_GENERATOR=Visual Studio 14 2015 Win64
    ) else (
        set CMAKE_GENERATOR=Visual Studio 14 2015
    )
) else if "%MSVC_VERSION%"=="12" (
    if "%processor_architecture%" == "AMD64" (
        set CMAKE_GENERATOR=Visual Studio 12 2013 Win64
    
    ) else (
        set CMAKE_GENERATOR=Visual Studio 12 2013
    )
)

set batch_file=!VS%MSVC_VERSION%0COMNTOOLS!..\..\VC\vcvarsall.bat
call "%batch_file%" %processor_architecture%

pushd ..
pushd examples
if NOT EXIST build mkdir build
pushd build

cmake -G"!CMAKE_GENERATOR!" ^
      -DPYTHONHOME:STRING=%PYTHONHOME%^
      -DCMAKE_BUILD_TYPE:STRING=%CMAKE_CONFIG% ^
      %~dp0
cmake --build . --config %CMAKE_CONFIG%  

pushd %CMAKE_CONFIG%  
if not EXIST platforms mkdir platforms
if EXIST %PYTHONHOME%/Library/plugins/platforms/qwindows.dll ^
cp %PYTHONHOME%/Library/plugins/platforms/qwindows.dll ./platforms/
popd
REM move ./%CMAKE_CONFIG% ../
popd
popd
popd
@endlocal
