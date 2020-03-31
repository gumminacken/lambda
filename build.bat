pushd build
call "c:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set ARNOLD_PATH=../thirdparty/Arnold
cl ..\src\main.cpp /I %ARNOLD_PATH%\include %ARNOLD_PATH%\lib\ai.lib /link /out:lambda.exe
popd

