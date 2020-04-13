pushd build
call "c:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set THIRDPARTY=../thirdparty
cl ..\src\main.cpp /I %THIRDPARTY%\arnold_windows\include /I %THIRDPARTY%\stb %THIRDPARTY%\arnold_windows\lib\ai.lib /link /out:lambda.exe
popd

