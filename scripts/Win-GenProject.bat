@echo off

pushd ..\
call vender\premake\premake5.exe vs2022
popd ..\
PAUSE