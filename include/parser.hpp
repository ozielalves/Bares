/**
 * @file parser.hpp
 * @version 1.0
 * @date May, 10.
 * @author Daniel Guerra and Oziel Alves
 * @title Parcer lib 
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream> // cout, cin
#include <iterator> // std::distance()
#include <vector>   // std::vector
#include <sstream>  // std::istringstream
#include <cstddef>  // std::ptrdiff_t
#include <limits>   // std::numeric_limits, para validar a faixa de um inteiro.
#include <algorithm>// std::copy, para copiar substrings.

#include "token.hpp"// struct Token.

/*!
 * @brief Implements a recursive descendent parser for a EBNF grammar.
 *
 * This class also tokenizes the input expression into its components, creating a list of tokens.
 *
 * The grammar is:
 * ```
 *   <expr>            := <term>,{ ("+"|"-"|"*"|"/"|"%"|"^"),<term> };
 *   <term>            := "(",<expr>,")" | <integer>;
 *   <integer>         := 0 | ["-"],<natural_number>;
 *   <natural_number>  := <digit_excl_zero>,{<digit>};
 *   <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 *   <digit>           := "0"| <digit_excl_zero>;
 * ```
 */

class Parser
{
    public:
        /// @brief This struct represents the result of the parsing operation.
        struct ResultType
        {
            //=== Alias
            typedef std::ptrdiff_t size_type; //!< Used for column location determination.

            /// @brief List of possible syntax errors.
            enum code_t {
                    OK = 0, //!< Expression successfuly parsed.
                    UNEXPECTED_END_OF_EXPRESSION,
                    ILL_FORMED_INTEGER,
                    MISSING_TERM,
                    EXTRANEOUS_SYMBOL,
                    INTEGER_OUT_OF_RANGE,
					MISSING_CLOSING_SCOPE
            };

            //=== Members (public).
            code_t type;		//!< Error code.
            size_type at_col;	//!< Stores the column number where the error happened.

            /// @brief Default contructor.
            explicit ResultType( code_t type_=OK , size_type col_=0u )
                    : type{ type_ }
                    , at_col{ col_ }
            { /* empty */ }
        };

        //==== Aliases
        typedef short int required_int_type; //!< The interger type we accept as valid for an expression.
        typedef long long int input_int_type; //!< The integer type that we read from the input (larger thatn the required int).

        //==== Public interface
        /// @brief Parses and tokenizes an input source expression.  Return the result as a struct.
        ResultType parse( std::string e_ );
        
        /// @brief Retrieves the list of tokens created during the partins process.
        std::vector< Token > get_tokens( void ) const;
		
        /// @brief Tokenizes the precedence of a certain operator.
		int get_precedence( std::string token_value );

        //==== Special methods
        /// @brief Default constructor
        Parser() = default;
        
        /// @brief Default destructor
        ~Parser() = default;

    private:
        
        // @brief Terminal symbols table
        enum class terminal_symbol_t{  // The symbols:-
			TS_EXPO,			//!< code for "^"
    		TS_TIMES,			//!< code for "*"
        	TS_DIV,				//!< code for "/"
			TS_MOD,				//!< code for "%"
			TS_PLUS,			//!< code for "+"
            TS_MINUS,	        //!< code for "-"
            TS_ZERO,            //!< code for "0"
            TS_NON_ZERO_DIGIT,  //!< code for digits "1"->"9"
			TS_OPENING,			//!< code for "("
			TS_CLOSING,			//!< code for ")"
            TS_WS,              //!< code for a white-space
            TS_TAB,             //!< code for tab
            TS_EOS,             //!< code for "End Of String"
            TS_INVALID	        //!< invalid token
        };

        //==== Private members.
        
        std::string expr;					//!< The source expression to be parsed
        std::string::iterator it_curr_symb;	//!< Pointer to the current char inside the expression.
        std::vector< Token > token_list;	//!< Resulting list of tokens extracted from the expression.

        terminal_symbol_t lexer( char c_ ) const;
        //std::string token_str( terminal_symbol_t s_ ) const;

        //=== Support methods.
        
        //! @brief Advances iterator to the next char in the expression.
        void next_symbol( void );                

        //! @brief Peeks the current character.
        bool peek( terminal_symbol_t c_ ) const;

        //! @brief Tries to accept the requested symbol.
        bool accept( terminal_symbol_t c_ );     
 
        //! @brief Skips any WS/Tab and tries to accept the requested symbol.    
        bool expect( terminal_symbol_t c_ );     

        //! @brief Skips any WS/Tab ans stops at the next character. 
        void skip_ws( void );                    
        
        //! @brief Checks whether we reached the end of the expression string.
        bool end_input( void ) const;            

        //=== NTS methods.

        //! @brief Validates (i.e. returns true or false) and consumes an expression from the input string.
        ResultType expression();

        //! @brief Validates (i.e. returns true or false) and consumes a term from the input string.
        //! @return true if a term has been successfuly parsed from the input; false otherwise.
        ResultType term();

        //! @brief Validates (i.e. returns true or false) and consumes an integer from the input string.
        //! @return true if an integer has been successfuly parsed from the input; false otherwise.
        ResultType integer();

        //! @brief Validates (i.e. returns true or false) and consumes a natural number from the input string.
        //! @return true if a natural number has been successfuly parsed from the input; false otherwise.
        ResultType natural_number();

        //! @brief Validates (i.e. returns true or false) and consumes a non-zero digit from the input string.
        //! @return true if a non-zero digit has been successfuly parsed from the input; false otherwise.
        bool digit_excl_zero();

        //! @brief Validates (i.e. returns true or false) and consumes a zero digit from the input string.
        //! @return true if a digit has been successfuly parsed from the input; false otherwise.
        bool digit();
};

#endif
