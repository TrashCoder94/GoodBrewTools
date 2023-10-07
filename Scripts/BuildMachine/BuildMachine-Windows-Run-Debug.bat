@echo off
pushd %~dp0\..\..\

echo "Running Debug GBHeaderToolTest executable!"
cd Binaries\Debug-windows-x86_64\GBHeaderToolTest
call GBHeaderToolTest.exe
if %errorlevel% neq 0 exit /b %errorlevel%

popd