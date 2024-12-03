# LDLt Factorization in C++

## Description
This project implements the **LDLt decomposition** in C++ to solve systems of linear equations involving symmetric positive-definite matrices. It reads input from a file (`data.txt`) and outputs the solution.

## Features
- Factorization of matrices into L, D, and Lt
- Solution of the system using forward substitution, diagonal division, and backward substitution
- Display of intermediate results (L, D, and solution vector)

## File Structure
- `main.cpp`: The main implementation of the algorithm.
- `data.txt`: Input file containing the matrix and the vector b.

## Input Format (data.txt)
The input file should contain:
1. The dimension of the matrix `n`.
2. The `n x n` matrix values.
3. The vector `b` of size `n`.

Example:
