@echo off
REM:Makefie
set src=matrix.c jacobi.c main.c
set obj=matrix.o jacobi.o main.o
set opt1= -static -std=gnu11 -O3 -lm -W -Wall
set gcc="c:\mingw\bin\gcc.exe"
set prog=rotjacobi.exe
if "%1"=="clean" goto clean
if "%1"=="object" goto create_obj
if "%1"=="exec" goto create_exe
if "%1"=="all" goto all
REM: Compiling the code
:create_obj
echo Compiling %src%
gcc  -c %src% %opt1%
exit
:create_exe
echo Assembling %prog%
gcc %obj% -o %prog%
exit
:all
echo Compiling %src%
gcc %src% %opt1% -o %prog%
exit
:clean
echo Deleting of %obj%
del %obj%
exit
