@echo off
pscp -l ubuntu -pw ubuntu .\Output\Project.bin ubuntu@58.196.137.213:/home/ubuntu/Engineering/JLink/LoadSeerDIO/
d:\Engneering\putty\putty.exe -ssh -l ubuntu -pw ubuntu 58.196.137.213 -m loadprogram.sh