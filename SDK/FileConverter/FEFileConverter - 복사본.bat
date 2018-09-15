@echo off
cls

../../SDK/FileConverter/fxc.exe /E PS_Main /T ps_5_0 /Fo ../../bin/Resource/fx/Demo.pso ../../Data/fx/Demo.fx /Zpr

@echo on