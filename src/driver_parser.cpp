/**
 * @file driver_parser.cpp
 * @version 1.0
 * @date May, 10.
 * @author Daniel Guerra and Oziel Alves
 * @title  Driver Parser 
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

#include "../include/parser.hpp"
#include "../include/infix2postfix.hpp"

//! @brief Printing the error messages.
void print_error_msg( const Parser::ResultType & result, std::string str, std::ofstream & ofs_ )
{
    std::string error_indicator( str.size()+1, ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
    switch ( result.type )
    {
        case Parser::ResultType::UNEXPECTED_END_OF_EXPRESSION:
			std::cout << ">>> Unexpected end of input at column (" << result.at_col + 1 << ")!\n";
			ofs_ << "Unexpected end of input at column (" << result.at_col + 1 << ")!\n";
            break;
        case Parser::ResultType::ILL_FORMED_INTEGER:
			std::cout << ">>> Ill formed integer at column (" << result.at_col + 1 << ")!\n";
			ofs_ << "Ill formed integer at column (" << result.at_col + 1 << ")!\n";
            break;
        case Parser::ResultType::MISSING_TERM:
            std::cout << ">>> Missing <term> at column (" << result.at_col + 1 << ")!\n";
            ofs_ << "Missing <term> at column (" << result.at_col + 1 << ")!\n";
			break;
        case Parser::ResultType::EXTRANEOUS_SYMBOL:
            std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col + 1 << ")!\n";
            ofs_ << "Extraneous symbol after valid expression found at column (" << result.at_col + 1 << ")!\n";
            break;
        case Parser::ResultType::INTEGER_OUT_OF_RANGE:
            std::cout << ">>> Integer constant out of range beginning at column (" << result.at_col + 1 << ")!\n";
            ofs_ << "Integer constant out of range beginning at column (" << result.at_col + 1 << ")!\n";
            break;
		case Parser::ResultType::MISSING_CLOSING_SCOPE:
            std::cout << ">>> Missing closing \")\" at column (" << result.at_col + 1 << ")!\n";
            ofs_ << "Missing closing \")\" at column (" << result.at_col + 1 << ")!\n";
            break;
		default:
            std::cout << ">>> Unhandled error found!\n";
            ofs_ << "Unhandled error found!\n";
            break;
    }

    std::cout << "\"" << str << "\"\n";
    std::cout << " " << error_indicator << std::endl;
}


int main( int argc, char **argv )
{
/*----------------- Command Line Arguments Control -----------------*/
	if( argc != 3 )
	{
		std::cerr << "Incorrect amount of arguments. Try again!\n";
		return -1;
	}
	
	std::istringstream ss( argv[1] );
	std::string in_file;
	ss >> in_file;
	
	std::istringstream zz( argv[2] );
	std::string out_file;
	zz >> out_file;

/*---------------------------- Streams -----------------------------*/
	std::ifstream ifs;
	std::ofstream ofs;
	ifs.open( in_file.c_str() );
	ofs.open( out_file.c_str() );

/*---------------------- Treating Expressions ----------------------*/
    Parser my_parser; // Instancia um parser.
    // Tentar analisar cada expressão da lista.
	std::string expression; // String var to constantly receive new expressions.
    while( getline( ifs, expression ) )
    {
        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expression );
        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expression << "\"\n";        
        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Parser::ResultType::OK )
        {
            print_error_msg( result, expression, ofs );
            /* Won't calculate if it isn't parsed right */
        }
        else
        {
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";
		}

         // Recuperar a lista de tokens.
        auto lista = my_parser.get_tokens();
        std::cout << ">>> Tokens: { ";
        std::copy( lista.begin(), lista.end(),
                std::ostream_iterator< Token >( std::cout, " " ) );
		std::cout << "}\n";
        
	/*---------------- Calculating ---------------------*/
		// For debugging
        if( result.type != Parser::ResultType::OK ) continue;
		/// Calculation only usable if expression is successfully parsed.
		std::vector< std::string > postfix = infix2postfix( lista );
		
        /*For debugging*/
		std::cout << "\n>>> Olhando separadamente:\n";
		for(auto & e : postfix ) {
			std::cout << e << ", ";
		}
		std::cout << "\n";
        
		auto answer = evaluate_postfix( postfix );

        if( answer.second < 0)
        {
            std::cout << "Division by zero!\n";
            ofs << "Division by zero!\n";
            continue;
        }
        if( answer.second > 0)
        {
            std::cout << "Numeric overflow error!\n";
            ofs << "Numeric overflow error!\n";
            continue;
        }

		std::cout << "Expression results in: " << answer.first << "\n";
        ofs << answer.first << "\n";
    }

    std::cout << "\n>>> Normal exiting...\n";

    ifs.close();
    ofs.close();

    return EXIT_SUCCESS;
}
