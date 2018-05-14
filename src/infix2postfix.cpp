/**
 * @file infix2postfix.cpp
 * @version 1.0
 * @date May, 10.
 * @author Daniel Guerra and Oziel Alves
 * @title Infix to Postfix Code
 * @brief Infix to Postfix conversion code
 */

/*!
 * Input Postfix expression must be in a desired format.
 * Operands and operator, **both must be single character**.
 * Only '+', '-', '*', '%', '/', and '^'. 
 * Any other character is just ignored.
 */
 
#include "../include/infix2postfix.hpp"
#include <limits>

/*---------------------------------------------------------------------------*/

std::vector< std::string > infix2postfix( std::vector< Token > infix_ ){
    
    // Stores the postfix expression.
    std::vector< std::string > postfix; // output
    // Stack to help the conversion.
    std::stack< Token > s;

    // Going through the expression.
    for( auto & ch : infix_ )
	{   
        // Operand goes straight to the output symbol queue.
        if ( (int)(ch.type) == 0 )
		{ 
            postfix.push_back( ch.value );
        }
		else if ( (int)(ch.type) == 1 )
		{
            // Pop out all the element with higher priority.
            while( not s.empty() and
                   has_higher_precedence( s.top() , ch ) )
			{
                postfix.push_back( s.top().value );
                s.pop();
            }
            
            // The incoming operator always goes into the stack.
            s.push( ch );
        } 
		else if (  ch.value == "(" )
		{// "("
            s.push( ch );
        }
        else if ( ch.value == ")" )
		{ // ")"
            // pop out all elements that are not '('.
            while( not s.empty() and s.top().value != "(" )
			{
                postfix.push_back( s.top().value ); // goes to the output.
                s.pop();
            }
            s.pop(); // Remove the '(' that was on the stack.
        } 
		else{ // anything else.
            // ignore this char.
        }
    }

    // Pop out all the remaining operators in the stack.
    while( not s.empty() )
	{    
        postfix.push_back( s.top().value );
        s.pop();
    }

    return postfix;
}

bool is_right_association( const Token & op )
{
    return op.value == "^";
}

bool has_higher_precedence( const Token & op1, const Token & op2 )
{
    auto p1 = op1.precedence;
    auto p2 = op2.precedence;

    if ( p1 == p2 and is_right_association( op1 ) )
	{
        return false;
    }

    return p1 >= p2 ;
}

std::pair< value_type,int > execute_operator( value_type n1, value_type n2, std::string opr )
{   
    /* Generating a pair. The first position represents the resulting value
    over the specified operations. The second position is a way of
    declaring and passing possible errors during execution.
    */
    std::pair< value_type,int > result( 0,0 );
    
/* Still need to give the Parser::ResultType to receive division by zero and 
 * Numeric Overflow */
	
	if( opr == "^" ) result.first = static_cast< value_type >( pow( n1, n2 ) );
	else if( opr == "*" ) result.first = static_cast< value_type >( n1*n2 );
	else if( opr == "/" )
	{
		if( n2 == 0 )
        {
            result.second = -1;
        }
		result.first = n1/n2;
	}
	else if( opr == "%" )
	{
		if( n2 == 0 )
        {
            result.second = -1;
        }
		result.first = n1%n2;
	}
	else if( opr == "+" ) result.first = n1+n2;
	else if( opr == "-" ) result.first = n1-n2;
    else {
        assert( false );
    }

    if ( result.first < std::numeric_limits< short int >::min() or
                 result.first > std::numeric_limits< short int >::max() )
    {
        std::cout << result.first << "\n";
        result.second = 1;
    }

    return result;
}

std::pair< value_type,int > evaluate_postfix( std::vector< std::string > postfix_ ){
    
    std::stack< value_type > s;

    for( auto & ch : postfix_ )
	{
		bool is_operand = true;
		long long int integer;

		try { integer = stoll( ch ); }
		catch( const std::invalid_argument & e )
		{
			is_operand = false;
		}

        if ( is_operand )
		{
            std::cout << ">>> " << integer << "\n";
            s.push( (value_type) integer );
        }

        else if ( std::string("+-%^/*").find( ch ) != std::string::npos )
		{
            // Recover the two operands in reverse order.
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();

            std::pair< value_type,int > result;
            result = execute_operator( op1, op2, ch );
            if( result.second < 0)
            {
                return std::make_pair( s.top(), -10 );
            }
            if( result.second > 0) 
            {
                return std::make_pair( s.top(), 10 );
            }

            s.push( result.first );
        }

		else assert(false);
    }

    return std::make_pair( s.top(), 0 );
}

