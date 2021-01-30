# Symmetric matrix eigenvalue calculation with Jacobi's method.

Implementation of eigenvalue calculation of symmetric matrices with Jacobi's method.
* Matrix is read from an input file *inputfile.dat*
* Eigenvalues are written into file *eig_inputfile.dat*

It is an old MSc project, reshashed and optimized.

## Running code
`rotjacobi /path/to/inputfile.dat /path/to/store/output/`

Example :  `rotjacobi ./test.dat ./`

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
* __main.c__
        * main program
* __matrix.c__
        * function declaration for matrix manipulation
* __jacobi.c__

        * implements Jacobi's rotation
* __inclusion.h__
        * declares functions' prototypes
* __Makefile__
        * Makefile to compile the source code
* __make.bat__
        * batch scrip to compile source code on Windows
* __test.dat__
        * test input file
