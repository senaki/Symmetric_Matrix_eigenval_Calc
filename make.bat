@echo off
REM:Makefie
set src=src/matrix.c src/jacobi.c src/main.c
set obj=matrix.o jacobi.o main.o
REM:-U TOL -D"TOL=1E-5"
set opt1= -static -O -std=gnu11 -lm -W -Wall -g
REM : change the path to gcc
set gcc="c:\mingw\bin\gcc.exe"
set prog=rotjacobi.exe
if "%1"=="clean" goto clean
if "%1"=="object" goto create_obj
if "%1"=="exec" goto create_exe
if "%1"=="all" goto all
REM: Compiling the code
:create_obj
echo Compiling %src%
gcc %opt1% -c %src%
exit
:create_exe
echo Assembling %prog%
gcc %obj% -g -o %prog%
exit
:all
echo Compiling %src%
gcc %opt1% %src% -o %prog%
exit
:clean
echo Deleting of %obj%
del %obj%
exit
