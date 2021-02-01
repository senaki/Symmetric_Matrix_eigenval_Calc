# Symmetric matrix eigenvalues calculation with Jacobi's method.

Implementation of eigenvalues calculation of symmetric matrices with Jacobi's method.
* Matrix is read from a text input file *inputfile*
* Eigenvalues are written into a text file named  *eig_inputfile*

It is a 2015 MSc project, revisited and optimized, published just to show you my style of C-programming ;).

## Running code
On Unix-Like distribution, run :

`./rotjacobi /path/to/inputfile /path/to/store/output/`

Example :  `rotjacobi ./test.dat ./`

Output :  

    Processing /path/to/inputfile.dat

    Duration : XXXX ms | Number of rotation : XXXX

## Input file structure
The first line contains the matrix dimension *n*.
The following lines contain the matrix elements separeted by a blank (white space or tabulation).
The matrix row ends with a newline character.
### Example of input file

| 2     |       |
| :---- | ----: |
| 1     | -2    |
| -2    | 0.0   |

## Output file structure
### Header
There are 2 lines of header, each terminated by a newline character.
The header start with # followed by one space.
The header consits of :

        # total iteration : _number of Jacobi's rotations_
        # matrix dimension : *n* x *1*
        y_1
        .
        .
        .
        y_n

where y_n is the n-th eigenvalue.

The second line gives the column matrix containing the eigenvalues.

## Source structure
* __main.c__ :
  * main program
* __matrix.c__ :
  * function declaration for matrix manipulation
* __jacobi.c__ :
  * implements Jacobi's rotation
* __inclusion.h__ :
  * declares functions' prototypes
* __Makefile__ :
  * Makefile to compile the source code
* __make.bat__ :
  * batch script to compile source code on Windows
* __test.dat__ :
  * test file with a 52x52 symmetric matrix input file
* __test_all.{ps1, sh}__ :  
  * powershell script for users on Windows or Unix-Like distros Os.
  * Run with
    `& "test_all.ps1" -src "input/path/" -des "output/path/"`or `"sh ./test_all.sh "input/path/" output/path/` where *input/path/* directory contains input data files and *output/path/* will receive the associated output data files.
