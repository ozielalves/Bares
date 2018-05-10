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

/*---------------------------------------------------------------------------*/

std::string infix2postfix( std::string infix_ ){
    
    // Stores the postfix expression.
    std::string postfix; // output
    // Stack to help the conversion.
    std::stack< char > s;

    // Going through the expression.
    for( auto ch : infix_ ){
        
        // Operand goes straight to the output symbol queue.
        if ( is_operand( ch ) ){ 
            postfix += ch;
        } else if ( is_operator( ch ) ){
            // Pop out all the element with higher priority.
            while( not s.empty() and
                   has_higher_precedence( s.top() , ch ) ){
                postfix += s.top();
                s.pop();
            }
            
            // The incoming operator always goes into the stack.
            s.push( ch );
        } else if ( is_opening_scope( ch ) ){// "("
            s.push( ch );
        }
        else if ( is_closing_scope( ch ) ){ // ")"
            // pop out all elements that are not '('.
            while( not s.empty() and not is_opening_scope( s.top() ) ){
                postfix += s.top(); // goes to the output.
                s.pop();
            }
            s.pop(); // Remove the '(' that was on the stack.
        } else{ // anything else.
            // ignore this char.
        }
    }

    // Pop out all the remaining operators in the stack.
    while( not s.empty() ){
        
        postfix += s.top();
        s.pop();
    }

    return postfix;
}

bool is_operator( char c ){
    return std::string("+-%^/*").find( c ) != std::string::npos;
}

bool is_operand( char c ){
    return ( c >= '0' and c<= '9');
}

bool is_opening_scope( char c ){
    return c == '(';
}

bool is_closing_scope( char c ){
    return c == ')';
}

bool is_right_association( char op ){
    return op == '^';
}

int get_precedence( char c ){
    
    int weight(0);
    
    switch( c ){

    /*------------------------- 3 --------------------------*/    
        case '^': weight = 3;
                  break;
    /*------------------------- 2 --------------------------*/
        case '*': weight = 2;
                  break;
        case '%': weight = 2;
                  break;
        case '/': weight = 2;
                  break;
    /*------------------------- 1 --------------------------*/
        case '+': weight = 1;
                  break;
        case '-': weight = 1;
                  break;
    /*------------------------- 0 --------------------------*/
            case '(':
                  weight = 0;
                  break;
    /*---------------------- Default -----------------------*/
        default:
            assert( false );
    }

    return weight;
}

bool has_higher_precedence( char op1, char op2 ){
    auto p1 = get_precedence( op1 );
    auto p2 = get_precedence( op2 );

    if ( p1 == p2 and is_right_association( op1 ) ){
        return false;
    }

    return p1 >= p2 ;
}

value_type char2integer( char ch ){
    return ch - '0';
}

value_type execute_operator( value_type n1, value_type n2, char opr ){
    
    value_type result(0);
    
    switch ( opr ){
    
        case '^' : result = static_cast<value_type>( pow( n1, n2 ) );
                   break;
        case '*' : result =  n1 * n2;
                   break;
        case '/' : if ( n2 == 0 )
                       throw std::runtime_error( "Division by zero!" );
                   result = n1/n2;
                   break;
        case '%' : if ( n2 == 0 )
                       throw std::runtime_error( "Division by zero!" );
                   result = n1%n2;
                   break;
        case '+' : result = n1 + n2;
                   break;
        case '-' : result = n1 - n2;
                   break;
        default: assert(false);
    }

    return result;
}

value_type evaluate_postfix( std::string postfix_ ){
    
    std::stack< value_type > s;

    for( auto ch : postfix_ ){
        if ( is_operand(ch ) ){
            s.push( char2integer(ch) );
        }
        else if ( is_operator(ch) ){
            // Recover the two operands in reverse order.
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();

            auto result = execute_operator( op1, op2, ch );

            s.push(result);
        } else{
            assert(false);
        }
    }

    return s.top();
}



