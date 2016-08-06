@echo off
set BUILD_TOOL="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
%BUILD_TOOL% insolence_toy.sln /v:n /tv:14.0 /p:Configuration=Debug

IF NOT "%ERRORLEVEL%" == "0" (
	goto :EOF
)
