@echo off
cls
REM
REM 셰이더 컴파일 (디버그 모드)
REM
REM 2016.12.01 Kihong Kim / onlysonim@gmail.com
REM

fxc.exe /E VS_Main /T vs_5_0  /Fo Demo.vso /Zi /Od  Demo.vsh
fxc.exe /E PS_Main /T ps_5_0  /Fo Demo.pso /Zi /Od  Demo.psh  /nologo


REM eof
@echo on
