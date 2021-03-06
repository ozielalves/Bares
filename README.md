# BARES. A project for Linguagem de Programação I (LP I)

## Introduction

This programming project implements the BARES, Basic Arithmetic Expression Evaluator based on Stacks. Using the [EBNF](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form) grammar, the [recursive descendent parsing](https://en.wikipedia.org/wiki/Recursive_descent_parser) strategy for the expressionsn, and utilizing the [Tokenization method](https://en.wikipedia.org/wiki/Lexical_analysis#Tokenization) for separation.

## EBNF Grammar

The grammatical rules to be aplied are:
```bash
<expr>			  := <term>, { ("+"|"-"|"%"|"/"|"*"|"^"),<term> };
<term>			  := "(",<expr>,")" | <integer>;
<integer>		  := "0" | ["-"],<natural_number>;
<natural_number>  := <digit_excl_zero>,{<digit>};
<digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
<digit>			  := "0" | <digit_excl_zero>;
```

## Operations, scope, and precedence

The supported operations and scope delimiters are:

- `^`: power, right association, weight=4.
- `*`: multiplication, left association, weight=3.
- `/`: division, left association, weight=3.
- `%`: module, left association, weight=3.
- `+`: addition, left association, weight=2.
- `-`: subtraction, left association, wight=2.
- `(`: opening scope, weight=1.
- `)`: closing scope, weight=0.

Here are a few examples of valid expressions:

- `1 + 3 * ( 9/2 - 3 * 2 ^3 )`
- `1+2-3*4%2  ^2`
- `10 + 5/5 + 8 * 3`
- `    128  *  37`
- `(1 + 3^2)^3^1`
- `------3`
- `---(5*(---(1-5) --    (-4+-7))) * 2`
- `(30000/(10+10)) * 12`
- `-5-4-4-(-1-4)`

## Algorithms

Functions implemented for:

1. Converting an expression received into a sequence of tokens, using a `recursive descendent parsing` strategy.
2. Converting an infix tokenized expression into its corresponding postfix representation, using a stack of Tokens.
3. Evaluating an postfix expression using a stack, therefore returning it's mathematical accurate value.

## TODO

- [x] Code the function `infix2postfix`.
- [x] Code the function `evaluate_postfix`.
- [x] Improve the code so it may accept operands with more than one digit.
- [x] Incorporate the syntactic analysis to parse any input expression.
- [x] Integrate all the modifications into a single program.
- [x] Fix parenthesis issue.
- [x] Make it accept countless '-' before terms. May it be a expression between parentheses or a integer. Like  $----3$ or $----(4+5---(2^3)).
- [x] Doxygen commentation.

## How to compile

First of all, obviously this repository is needed to work as desired, so we must to download this repo. As we know that ```git``` is a tremendous tool for any programmer, then we must have git installed.

It is also expected for user to have installed [doxygen](https://en.wikipedia.org/wiki/Doxygen) program. This will automatically generate documentation for code. Not a necessary package to compile and run program, but it is a tool to guide user through code functions. To install `doxygen` on UBUNTU, for example:

```bash
$ sudo apt-get install doxygen
```
To compile we will use a makefile, so compilations may be more dynamic and automatic.
```bash
# Using 'git clone' to clone this repo into desired directory:
$ git clone https://github.com/ozielalves/Bares.git

# Enter repo:
$ cd Bares

# To compile the whole project and also generate documentation, insert 'make' inside root of path:
$ make

# To clean up all remaining trash data and files, such as the binary ones, insert 'make clean':
$ make clean
```

## How to execute

Now, we show how to run the program. 
```bash
# To execute program:
$ ./bares <input_file> <output_file>
```
- `<input_file>`: Represents the file containing all the expressions wished to be tested.
- `<output_file>`: File where the results obtained through parsing and calculations are written.

### Example

Let's say your information is stored in a file called $in.txt$, which is inside the directory $data$, and you want to store the results into a file named $out.txt$, also inside $data$ directory. The program should run like this:
```bash
$ ./bares data/in.txt data/out.txt
```
## GitHub Repository:

*https://github.com/ozielalves/Bares*

## Authorship

Program developed by [_Daniel Oliveira Guerra_](https://github.com/Codigos-de-Guerra) (*daniel.guerra13@hotmail.com*) and [_Oziel Alves_](https://github.com/ozielalves) (*ozielalves@ufrn.edu.br*), 2018.1

&copy; IMD/UFRN 2018.
