@echo off
cls
REM
REM 셰이더 컴파일 (릴리즈 버전)
REM
REM 2016.12.01 Kihong Kim / onlysonim@gmail.com
REM

fxc.exe /E VS_Main /T vs_5_0  /Fo Demo.vso Demo.vsh
fxc.exe /E PS_Main /T ps_5_0  /Fo Demo.pso Demo.psh  /nologo


REM eof
@echo on
