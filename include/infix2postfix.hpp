/**
 * @file infix2postfix.hpp
 * @version 1.0
 * @date May, 10.
 * @author Daniel Guerra and Oziel Alves
 * @title Infix to Postfix Lib 
 * @brief Infix to Postfix conversion Lib
 */

#ifndef _INFIX2POSTFIX_HPP_
#define _INFIX2POSTFIX_HPP_

#include <stack>     // stack
#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow
#include <stdexcept> // std::runtime_error

// To change type. (Optional)
using value_type = long int;

/// Simple helper functions.
/*
bool is_operator( char c );
bool is_operand( char c );
bool is_opening_scope( char c );
bool is_closing_scope( char c );
*/

/// Sees if you are looking at '^' operator
bool is_right_association( const Token & op );
/// Says if the first operator is bigger than the second operator.
bool has_higher_precedence( const Token & op1, const Token & op2 );

/// Converts a expression in infix notation to a corresponding profix representation.
std::string infix2postfix( std::vector< Token > infix_);

/// Converts a char (1-digit operand) into an integer.
value_type char2integer( char ch );

/// Execute the binary operator on two operands and return the result.
value_type execute_operator( value_type n1, value_type n2, char opr);

/// Change an infix expression into its corresponding postfix representation.
value_type evaluate_postfix( std::string postfix_);

#endif

