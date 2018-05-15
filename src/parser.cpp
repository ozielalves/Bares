/**
 * @file parser.cpp
 * @version 1.0
 * @date May, 14.
 * @author Daniel Guerra and Oziel Alves
 * @title  Operation Parser 
 */

#include "../include/parser.hpp"
#include <iterator>
#include <algorithm>
#include <cassert>

int scopeOPENING = 0;
int scopeCLOSING = 0;

/// @brief Converts the input character c_ into its corresponding terminal symbol code.
Parser::terminal_symbol_t  Parser::lexer( char c_ ) const
{
    switch( c_ )
    {
		case '^':  return terminal_symbol_t::TS_EXPO;
		case '*':  return terminal_symbol_t::TS_TIMES;
		case '/':  return terminal_symbol_t::TS_DIV;
		case '%':  return terminal_symbol_t::TS_MOD;
        case '+':  return terminal_symbol_t::TS_PLUS;
        case '-':  return terminal_symbol_t::TS_MINUS;
        case ' ':  return terminal_symbol_t::TS_WS;
        case   9:  return terminal_symbol_t::TS_TAB;
		case '(':  return terminal_symbol_t::TS_OPENING;
		case ')':  return terminal_symbol_t::TS_CLOSING;
        case '0':  return terminal_symbol_t::TS_ZERO;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':  return terminal_symbol_t::TS_NON_ZERO_DIGIT;
        case '\0': return terminal_symbol_t::TS_EOS; // end of string: the $ terminal symbol
    }
    return terminal_symbol_t::TS_INVALID;
}

/// @brief Determines each symbol precedence.
int Parser::get_precedence( std::string token_value )
{
	int weight = 0; //!< Symbol weight

	if( token_value == "^" ) weight = 4;
	else if( token_value == "*" or token_value == "/" or token_value == "%" ) weight = 3;
	else if( token_value == "+" or token_value == "-" ) weight = 2;
	else if( token_value == "(" ) weight = 1;
	else assert( false );

	return weight;
}

/// @brief Consumes a valid character from the input source expression.
void Parser::next_symbol( void )
{
    // Advances iterator to the next valid symbol for processing
    std::advance( it_curr_symb, 1 );
}

/// @brief Checks whether we reached the end of the input expression string.
bool Parser::end_input( void ) const
{
    // "Fim de entrada" ocorre quando o iterador chega ao
    // fim da string que guarda a expressão.
    return it_curr_symb == expr.end();
}

/// @return The result of trying to match the current character with c_, **without** consuming the current character from the input expression.
bool Parser::peek( terminal_symbol_t c_ ) const
{
    // Checks whether the input symbol is equal to the argument symbol.
    return ( not end_input() and
             lexer( *it_curr_symb ) == c_ );
}

/// @return The result of trying to match (peek()) and consume the current character with c_.
/*!
 * If the match is not successful, the current character (from the input) **is not consumed**.
 * @param c_ the current character from the input source expression we wish to parse.
 * @see peek().
 * @return true if we got a successful match; false otherwise.
 */
bool Parser::accept( terminal_symbol_t c_ )
{
    // If we have a match, we consume the character from the input source expression.
    // caractere da entrada.
    if ( peek( c_ ) )
    {
        next_symbol();
        return true;
    }

    return false;
}

/// @brief Skips all white spaces and tries to accept() the next valid character. @see accept().
bool Parser::expect( terminal_symbol_t c_ )
{
    // Skip all white spaces first.
    skip_ws();
    return accept( c_ );
}


/// @brief Ignores any white space or tabs in the expression until reach a valid character or end of input.
void Parser::skip_ws( void )
{
    // Skip white spaces, while at the same time, check for end of string.
    while ( not end_input() and
            ( lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_WS  or
              lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_TAB ) )
    {
        next_symbol();
    }
}


//=== Non Terminal Symbols (NTS) methods.

/// @brief Validates (i.e. returns true or false) and consumes an expression from the input string.
/*! This method parses a valid expression from the input and, at the same time, it tokenizes its components.
 *
 * Production rule is:
 * ```
 *  <expr> := <term>,{ ("+"|"-"|"*"|"/"|"%"|"^"),<term> };
 * ```
 * An expression might be just a term or one or more terms with '+' / '-' / '*' / '/' / '%' / '^' between them.
 */
Parser::ResultType Parser::expression()
{
    ResultType result; 
	
	result = term(); //!< Process a term
	
	// Let's tokenize this term, if just well formed.
	if( result.type == ResultType::OK )
	{
		skip_ws();
		// Por estar bem formado, devesse ser seguido de operadores
		// and then another term. Time after time.
		while( not end_input() )
		{
			// After reading a term, an operator becomes necessary.
			if( accept( terminal_symbol_t::TS_EXPO) or accept( terminal_symbol_t::TS_TIMES) or
				accept( terminal_symbol_t::TS_DIV) or accept( terminal_symbol_t::TS_MOD) or
				accept( terminal_symbol_t::TS_PLUS) or accept( terminal_symbol_t::TS_MINUS) )
			{
				std::string token_str;
				auto temp(it_curr_symb);
				std::advance( it_curr_symb, -1 );

				// Copy the found string word on to 'token_str'.
				std::copy( it_curr_symb, temp, std::back_inserter( token_str ));

				int pred = get_precedence( token_str );

				token_list.emplace_back( Token( token_str, Token::token_t::OPERATOR, pred ) );

				// Don't forget to consume and advance iterator.
				std::advance( it_curr_symb, 1);
			}
			else
			{
				std::cout << "só pode estar sendo aqui\n";
				return ResultType( ResultType::EXTRANEOUS_SYMBOL, std::distance( expr.begin(), it_curr_symb ) );
			}
			
			// After a operator, we need a term to apply operation.
			// If there is none, a term is missing.
			skip_ws();			
			if( end_input() )
			{
				return ResultType( ResultType::MISSING_TERM, std::distance( expr.begin(), it_curr_symb ) );
			}

			result = term();

			if( result.type != ResultType::OK )
			{
				return result;
			}

			// Skip to get the next operator, if existant.
			skip_ws();
		}
	}

    return result;
}

/// @brief Validates (i.e. returns true or false) and consumes a term from the input string.
/*! This method parses a valid term from the input.
 *
 * Production rule is:
 * ```
 *  <term> := "(",<expr>,")" | <integer>;
 * ```
 * A term is made of a single integer.
 *
 * @return true if a term has been successfuly parsed from the input; false otherwise.
 */
Parser::ResultType Parser::term()
{
	ResultType result;

	/// Process the several '-' signs that may come before a term.
	int minus = 0;
	while( lexer( *it_curr_symb ) == terminal_symbol_t::TS_MINUS )
	{
		minus++;
		next_symbol();
	}
	minus = minus % 2;
	if( lexer( *it_curr_symb ) == terminal_symbol_t::TS_OPENING and minus != 0 )
	{
		token_list.emplace_back( Token( "-1", Token::token_t::OPERAND, 0 ) );
		token_list.emplace_back( Token( "*", Token::token_t::OPERATOR, 3 ) );
	}
	else if( minus != 0 and lexer( *it_curr_symb ) != terminal_symbol_t::TS_OPENING )
	{
		it_curr_symb = it_curr_symb - 1;
	}

	skip_ws();
    if( accept( terminal_symbol_t::TS_OPENING ) )
	{
		// Increases the difference between scopes of opening and closing.
		scopeOPENING++;

		token_list.emplace_back( Token( "(", Token::token_t::SCOPE, 1 ) );

		// If a parenthesis was opened, then it should render an expression.
		// Process the expression
		result = expression();
		if( result.type != ResultType::OK )
		{
			return result;	
		}
		skip_ws();
	}

	// If we do not detect parentheses, then we must parse an integer.
	if ( not accept( terminal_symbol_t::TS_OPENING ) and not accept( terminal_symbol_t::TS_CLOSING ) and not end_input() )
	{			
	    // Saves the beginning of the term in the input, for possible error messages.
    	auto begin_token( it_curr_symb );
		// Process an integer.
	    result = integer();
	    // Let's tokenize the integer if it is well formed.
	    if ( result.type == ResultType::OK )
	    {
	        // Copy the corresponding substring to a string variable.
	        std::string token_str;
	        std::copy( begin_token, it_curr_symb, std::back_inserter( token_str ) );

    	    // Try to convert the string to integer (use stoll ()).
        	input_int_type token_int;
	        try { token_int = stoll( token_str ); }
    	    catch( const std::invalid_argument & e )
        	{
            	return ResultType( ResultType::ILL_FORMED_INTEGER, 
                	               std::distance( expr.begin(), begin_token ) );
	        }

    	    // We received a valid integer, it remains to know if it is within the range.
        	if ( token_int < std::numeric_limits< required_int_type >::min() or
            	 token_int > std::numeric_limits< required_int_type >::max() )
			{
    	        // Out of range, report error
        	    return ResultType( ResultType::INTEGER_OUT_OF_RANGE, 
            	                   std::distance( expr.begin(), begin_token ) );
	        }
    	    // Puts the new token on our token list.
        	token_list.emplace_back( Token( token_str, Token::token_t::OPERAND ) );
	    }
		skip_ws();
	}

	// It consumes sequential closing parentheses.
	while( accept( terminal_symbol_t::TS_CLOSING ) )
	{
		scopeCLOSING++;
		if( (scopeOPENING - scopeCLOSING) < 0 ) {
			return ResultType( ResultType::EXTRANEOUS_SYMBOL, std::distance( expr.begin(), it_curr_symb-1 ) );
		}
		token_list.emplace_back( Token( ")", Token::token_t::SCOPE ) );
		
		// At the end of the parentheses, the term is considered finished.
		skip_ws();
	}

    return result;
}

/// @brief Validates (i.e. returns true or false) and consumes an integer from the input string.
/*! This method parses a valid integer from the input and, at the same time, add the integer to the token list.
 *
 * Production rule is:
 * ```
 * <integer> := 0 | ["-"],<natural_number>;
 * ```
 * A integer might be a zero or a natural number, which, in turn, might begin with an unary minus.
 *
 * @return true if an integer has been successfuly parsed from the input; false otherwise.
 */
Parser::ResultType Parser::integer()
{
    // If we accepted a zero, then the whole thing ended here.
    if ( accept( terminal_symbol_t::TS_ZERO ) )
        return ResultType( ResultType::OK );

    // Let's try to accept the '-'.
	accept( terminal_symbol_t::TS_MINUS );
    return natural_number();
}

/// @brief Validates (i.e. returns true or false) and consumes a natural number from the input string.
/*! This method parses a valid natural number from the input.
 *
 * Production rule is:
 * ```
 * <natural_number> := <digit_excl_zero>,{<digit>};
 * ```
 *
 * @return true if a natural number has been successfuly parsed from the input; false otherwise.
 */
Parser::ResultType Parser::natural_number()
{
    // There must be a number that is not zero! (according to the definition).
    if ( not digit_excl_zero() )
	{
		return ResultType( ResultType::ILL_FORMED_INTEGER, std::distance( expr.begin(), it_curr_symb ) ) ;
	}

    // Use the other digits, if there are ...
    while( digit() ) /* empty */ ;

    return ResultType( ResultType::OK );
}

/// @brief Validates (i.e. returns true or false) and consumes a non-zero digit from the input string.
/*! This method parses a single valid non-zero digit from the input.
 *
 * Production rule is:
 * ```
 * <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 * ```
 *
 * @return true if a non-zero digit has been successfuly parsed from the input; false otherwise.
 */
bool Parser::digit_excl_zero()
{
    return accept( terminal_symbol_t::TS_NON_ZERO_DIGIT );
}

/// @brief Validates (i.e. returns true or false) and consumes a zero digit from the input string.
/*! This method parses a single valid digit from the input.
 *
 * Production rule is:
 * ```
 * <digit> := "0"| <digit_excl_zero>;
 * ```
 *
 * @return true if a digit has been successfuly parsed from the input; false otherwise.
 */
bool Parser::digit()
{
    return ( accept( terminal_symbol_t::TS_ZERO ) or digit_excl_zero() ) ? true : false;

/*
    if ( not accept( terminal_symbol_t::TS_ZERO ) )
    {
        return digit_excl_zero();
    }
    return true;
*/
}

/*!
 * This is the parser's entry point.
 * This method tries to (recursivelly) validate an expression.
 * During this process, we also store the tokens into a container.
 *
 * \param e_ The string with the expression to parse.
 * \return The parsing result.
 *
 * @see ResultType
 */
Parser::ResultType Parser::parse( std::string e_ )
{
    expr = e_; //!< Save the expression on the corresponding member.
    it_curr_symb = expr.begin(); //!< Defines an initial symbol to be processed.
    ResultType result; // By default it's OK.

    // Always cleaning the token list from the last time.
    token_list.clear();

    // Let's check if we get a 'Let us ignore any leading white spaces.'
    skip_ws();
    if ( end_input() ) // Premature end?
    {
        result = ResultType( ResultType::UNEXPECTED_END_OF_EXPRESSION,
                std::distance( expr.begin(), it_curr_symb ) );
    }
    else
    {
        // Regular call for expression.
		scopeOPENING = 0;
		scopeCLOSING = 0;

        result = expression();

        // Check if there is something left in the expression.
        if ( result.type == ResultType::OK )
        {
	        // At this point there should be nothing left in the string, except
            // white spaces.
            skip_ws(); // Let's "consume" the blanks, if they exist ....
            if ( not end_input() ) // If everything is ok, we should be at the end of the string.
            {
                return ResultType( ResultType::EXTRANEOUS_SYMBOL, std::distance( expr.begin(), it_curr_symb ) );
            }

			if( scopeOPENING > scopeCLOSING )
			{
				return ResultType( ResultType::MISSING_CLOSING_SCOPE,
									std::distance( expr.begin(), it_curr_symb ) );
			}
        }
    }
		
    return result;

}


/// @return The list of tokens, which is the by-product created during the syntax analysis.
std::vector< Token >
Parser::get_tokens( void ) const
{
    return token_list;
}


//==========================[ End of parse.cpp ]==========================//
