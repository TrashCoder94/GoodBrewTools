@echo off
pushd %~dp0\..\..\

echo "Running Release GBHeaderToolTest executable!"
cd Binaries\Shipping-windows-x86_64\GBHeaderToolTest
call GBHeaderToolTest.exe
if %errorlevel% neq 0 exit /b %errorlevel%

popd