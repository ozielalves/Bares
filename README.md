# BARES. A project for Linguagem de Programação I (LP I)

## Introduction

This programming project implements the BARES, Basic Arithmetic Expression Evaluator based on Stacks.  

We introduce a few simplifications on the input so we may focus only on the algorithms itself.
They are: the arithmetic expressions must only accept one-digit operands, and; all expressions are considered syntactically correct.

## Operations, scope, and precedence

The supported operations and scope delimiters are:

- `^`: power, right association, weight=3.
- `*`: multiplication, left association, weight=2.
- `/`: division, left association, weight=2.
- `%`: module, left association, weight=2.
- `+`: addition, left association, weight=1.
- `-`: subtraction, left association, wight=1.
- `(`: opening scope, weight=0.
- `)`: closing scope, weight=n/a.

Here are a few examples of valid expressions:

- `1 + 3 * ( 9/2 - 3 * 2 ^3 )`
- `1+2-3*4%2  ^2`

## Algorithms

We need to implement functions for:

1. Converting an infix expression into its corresponding postfix representation, using a stack of symbols (character).
2. Evaluating an postfix expression using a stack.

## TODO

- [x] Code the function `infix2postfix`
- [x] Code the function `evaluate_postfix`
- [ ] Improve the code so it may accept operands with more than one digit.
- [ ] Incorporate the syntactic analysis to parse any input expression.
- [ ] Integrate all the modifications into a single program.



## How to compile
To compile we will use a makefile, so all compilations be more dynamic and automatic.
```bash
# To compile the whole project, insert 'make':
$ make

# To compile only the objects and see if class implementations were compiled well, insert 'make build':
$ make build

# To clean up all remaining trahsh data and files, such as the binary ones, insert 'make clean':
$ make clean
```

## How to execute
Now, we show how to run the program
```bash
# To execute program:
$ ./  TODO

# Detail: The program will also show the possible errors.(?) A gnt exclui isso se não der pra fazer.;-;
```


## Authorship

Program developed by _Daniel Oliveira Guerra_ (*daniel.guerra13@hotmail.com*) and _Oziel Alves_ (*ozielalves@ufrn.edu.br*), 2018.1

&copy; IMD/UFRN 2018.
