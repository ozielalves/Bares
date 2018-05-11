#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>   // std::string
#include <iostream> // std::ostream

/// Represents a token.
struct Token
{
    public:
        enum class token_t : int
        {
            OPERAND = 0,	//!< A type representing numbers.
            OPERATOR,		//!< A type representing "+", "-", "*", "/", "%", "^".
			SCOPE			//!< A type representing "(", ")".
        };

        std::string value;	//!< The token value as a string.
        token_t type;		//!< The token type, which is either token_t::OPERAND, token_t::OPERATOR or token_t::SCOPE.
		int precedence;		//!< The token precedence for each type.

        /// Construtor default.
        explicit Token( std::string value_="", token_t type_ = token_t::OPERAND, int precedence_=0u )
            : value( value_ )
            , type( type_ )
			, precedence( precedence_ )
        {/* empty */}

        /// Just to help us debug the code.
        friend std::ostream & operator<<( std::ostream& os_, const Token & t_ )
        {
            std::string types[] = { "OPERAND", "OPERATOR", "SCOPE" };

            os_ << "<" << t_.value << "," << types[(int)(t_.type)] << "," << t_.precedence << ">";

            return os_;
        }
};

#endif
