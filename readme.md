# Recursive Descent Calculator
This is an assignment given for the course of Compiler Design.

This is a simple calculator implemented using a recursive descent parser and lexical analyzer (scanner) in C. It supports basic arithmetic operations like addition, subtraction, multiplication, and division, as well as variable declarations, assignments, and input/output operations.

## Files & Folders

- **[`lex.l`](lex.l)**  
  A Flex file defining the lexical rules for tokenizing the input code. It recognizes keywords, identifiers, numbers, operators, and parentheses.

- **[`recursiveDescent.c`](recursiveDescent.c)**  
  The main program containing the recursive descent parser. It processes the tokens produced by the lexer and evaluates expressions.

- **[`y.tab.h`](y.tab.h)**  
  A header file containing the token definitions for use in both the lexer and the parser.

- **[`makefile`](makefile)**  
  A Makefile to compile the project. It handles the building process for the lexer (`lex.yy.o`), parser (`recursiveDescent.o`), and generates the final executable (`a.out`).

- **[`inputs/`](input/)**  
  A folder containing 8 example input files with arithmetic expressions for testing the parser.

## Features

- **Arithmetic Operations**: Supports `+`, `-`, `*`, `/` operations.
- **Data Types**: Supports `int` and `float` data types.
- **Variable Declaration**: Allows declaration of variables with types `int` and `float`.
- **Assignment**: Variables can be assigned values using the `=` operator.
- **Input and Output**: Supports user input via the `input` keyword and output via the `print` keyword.

## Compilation

1. Install Flex and Bison (or your system's equivalent tools):
   - On Ubuntu: `sudo apt-get install flex bison`
   - On macOS: `brew install flex bison`

2. Use the provided `Makefile` to compile the project:
   
```bash
make
```

## Usage
To run the calculator, use the following command:

```bash
./a.out <input_file>
```
Where <input_file> is a text file containing the code to be executed. The code in the input file should follow the syntax defined by the lexer and parser.

## Example Input

```bash
zah x, y;
x = 10;
y = 5;
print x + y;
```

## Example Output

```bash
15
```

## Explanation
- The program supports variables of type int (denoted by zah) and float (denoted by flt).

- The input keyword allows the user to provide values for variables, and print outputs expressions.

- The input file is processed by Flex and Bison, generating the corresponding tokens and parsed expressions.


##  License
This project is licensed under the MIT License.
