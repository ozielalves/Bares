/**
 * @file infix2postfix.hpp
 * @version 1.0
 * @date May, 14.
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
#include <vector>    // 

#include "token.hpp"

using value_type = long int; //!< To change type. (Optional)

/// @brief Sees if you are looking at '^' operator.
bool is_right_association( const Token & op );

/// @brief Says if the first operator is bigger than the second operator.
bool has_higher_precedence( const Token & op1, const Token & op2 );

/// @brief Converts a expression in infix notation to a corresponding profix representation.
std::vector< std::string > infix2postfix( std::vector< Token > infix_ );

/// @brief Execute the binary operator on two operands and return the result.
std::pair< value_type,int > execute_operator( value_type n1, value_type n2, std::string opr );

/// @brief Change an infix expression into its corresponding postfix representation.
std::pair< value_type,int > evaluate_postfix( std::vector< std::string > postfix_ );

#endif

