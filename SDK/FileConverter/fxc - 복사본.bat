@echo off
cls
REM
REM ���̴� ������ (������ ����)
REM
REM 2016.12.01 Kihong Kim / onlysonim@gmail.com
REM

fxc.exe /E VS_Main /T vs_5_0  /Fo ../../bin/Resource/shd/FXObject.vso Demo.fx /Zpr
fxc.exe /E PS_Main /T ps_5_0  /Fo ../../bin/Resource/shd/FXObject.pso Demo.fx /Zpr /nologo


REM eof
@echo on
