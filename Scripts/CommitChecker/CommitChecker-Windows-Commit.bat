@echo off
pushd %~dp0\..\..\

git add *
git commit -m %1 -m %2

popd