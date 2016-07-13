#include <string>
#include <stdexcept>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "AST.hpp"
#include "printer.hpp"
#include "calc.hpp"

using std::string;


/*
  <exp>           ::= <term> { '+' <term>}
                   |  <term> { '-' <term>}
  <term>          ::= <signed-factor> { '*' <signed-factor>}
                   |  <signed-factor> { '/' <signed-factor>}
  <signed-factor> ::= - <unsigned-factor>
                   | + <unsigned-factor>
                   | <unsigned-factor>
  <factor>        ::= <digit> | '(' <exp> ')'
*/


// abstract class
#define debug(s) std::cout << s << std::endl;
class Parser{
private:
  string data;
  string::size_type index;

  bool more(){
    while(index < data.size() && data[index]==' ') {
      ++index;
    }
    return index < data.size();
  }

  // only called when there is more
  char peek(){
    return data[index];
  }

  void eat(char c){
    assert(more() && peek()==c);
    ++index;
  }
  static bool isdigit(char c){
    return c >= '0' && c <= '9';
  }


  ExpPtr number(){
    assert(more());
    int value = 0;
    while(more() && isdigit(peek())){
      char c = peek();
      value = value * 10 + c - '0';
      eat(c);
    }
    return new Number(value);
  }

  ExpPtr unsigned_factor(){
    assert(more());
    if(peek()=='('){
      eat('(');
      ExpPtr ret = exp();
      eat(')');
      return ret;
    }
    return number();
  }

  ExpPtr signed_factor(){
    assert(more());

    if(peek()=='-'){
      eat('-');
      return new Negative(unsigned_factor());
    }
    else if(peek()=='+'){
      eat('+');
      return new Positive(unsigned_factor());
    }
    else{
      return new Positive(unsigned_factor());
    }
  }
  ExpPtr term(){
    ExpPtr lhs = signed_factor();
    while(more()){
      if(peek()=='*'){
        eat('*');
        lhs = new Multiplies(lhs, term());
      }
      else if(peek()=='/'){
        eat('/');
        lhs = new Divides(lhs, term());
      }
      else break;
    }

    return lhs;
  }
  ExpPtr exp(){
    ExpPtr lhs = term();
    while(more()){
      if(peek()=='+'){
        eat('+');
        lhs = new Plus(lhs, term());
      }
      else if(peek()=='-'){
        eat('-');
        lhs = new Minus(lhs, term());
      }
      else break;
    }
    return lhs;
  }

public:
  Parser(const string &_data):data(_data), index(0){}

  ExpPtr parse(){
    return exp();
  }
};


int main(){
  Parser p("1+2-3-(4*-1+6)");
  ExpPtr exp = p.parse();
  Printer printer(std::cout);
  printer.visit(exp);
  Calc calc;
  calc.visit(exp);
  std::cout << calc.result << std::endl;
  return 0;
}
