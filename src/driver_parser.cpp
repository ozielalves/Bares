#include <iostream>
#include <iomanip>
#include <vector>

#include "../include/parser.hpp"
#include "../include/infix2postfix.hpp"

/*
std::vector<std::string> expressions =
{
    "10",
    "    12    +    -4   8",
    "32767 - 32768 + 3",
    "5 + -32766",
    "5 + -32769",
    "12 + 3",
    "-3+-5+-6",
    "12 + 3     -3 + -34 ",
    "+12",
    "1.3 * 4",
    "a + 4",
    "       ",
    "  123 *  548",
	"5+(7*1)-4/(4-2)",
	" (12 + 5) * 3",
	" 5 + ((2 + 1)/ 3)"
};
*/

std::vector< std::string > expressions =
{
	"4/(5^2)+(6^2^3)",
    "5+(7*1)-4/(4-2)",
    " (12 + 5) * 3",
    "  123 *  548",
    "5+10/(5-5)",
	"1+ 3 * ( 4 + 8 * 3 ^7)"
};

void print_error_msg( const Parser::ResultType & result, std::string str )
{
    std::string error_indicator( str.size()+1, ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
    switch ( result.type )
    {
        case Parser::ResultType::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << ">>> Unexpected end of input at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::ILL_FORMED_INTEGER:
            std::cout << ">>> Ill formed integer at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::MISSING_TERM:
            std::cout << ">>> Missing <term> at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::EXTRANEOUS_SYMBOL:
            std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::INTEGER_OUT_OF_RANGE:
            std::cout << ">>> Integer constant out of range beginning at column (" << result.at_col << ")!\n";
            break;
		case Parser::ResultType::MISSING_CLOSING_SCOPE:
            std::cout << ">>> Missing closing \")\" at column (" << result.at_col << ")!\n";
            break;
		default:
            std::cout << ">>> Unhandled error found!\n";
            break;
    }

    std::cout << "\"" << str << "\"\n";
    std::cout << " " << error_indicator << std::endl;
}


int main()
{
    Parser my_parser; // Instancia um parser.
    // Tentar analisar cada expressão da lista.
    for( const auto & expr : expressions )
    {
        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expr );
        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";
        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Parser::ResultType::OK )
            print_error_msg( result, expr );
        else
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";

         // Recuperar a lista de tokens.
        auto lista = my_parser.get_tokens();
        std::cout << ">>> Tokens: { ";
        std::copy( lista.begin(), lista.end(),
                std::ostream_iterator< Token >(std::cout, " ") );
        std::cout << "}\n";

	/*---------------- Calculating ---------------------*/
		std::vector< std::string > postfix = infix2postfix(lista);

        /*For debugging*/
		std::cout << ">>> Olhando separadamente:\n";
		for(auto & e : postfix ) {
			std::cout << e << ", ";
		}
		std::cout << "\n";
        
		auto answer = evaluate_postfix( postfix );

        if( answer.second < 0)
        {
            std::cout << "Divison by zero!\n";
            continue;
        }
        if( answer.second > 0)
        {
            std::cout << "Numeric Overflow!\n";
            continue;
        }

		std::cout << "Expression results in: " << answer.first << "\n";
    }


    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}
