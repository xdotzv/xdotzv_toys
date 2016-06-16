#include <string>
#include <stdexcept>
#include <cstdlib>
#include <cassert>

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
class Visitor;
using VisitorPtr = Visitor *;


struct Exp{
  virtual void  accept(VisitorPtr ) = 0;
};
using ExpPtr = Exp *;

struct Binary: public Exp{
  ExpPtr lhs, rhs;
};

struct Plus: public Binary{
  void accept(VisitorPtr ) override;
};

struct Minus: public Binary{
  void accept(VisitorPtr ) override;
};

struct Multiplies: public Binary{
  void accept(VisitorPtr ) override;
};

struct Divides: public Binary{
  void accept(VisitorPtr ) override;
};

struct Unary: public Exp{
  ExpPtr p;
  Unary(ExpPtr _p):p(_p){}
};

struct Negative: public Unary{
  void accept(VisitorPtr ) override;
};
struct Positive: public Unary{
  void accept(VisitorPtr ) override;
};

struct Number: public Exp{
  int value;
  void accept(VisitorPtr) override;
  Number(int _value):value(_value){}
};


using PlusPtr = Plus*;
using MinusPtr = Minus*;
using MultipliesPtr = Multiplies*;
using DividesPtr = Divides*;
using PositivePtr = Positive*;
using NegativePtr = Negative*;
using NumberPtr = Number*;

class Visitor{
public:
  void visit(PlusPtr) = 0;
  void visit(MinusPtr) = 0;
  void visit(MultipliesPtr) = 0;
  void visit(DividesPtr) = 0;
  void visit(PositivePtr) = 0;
  void visit(NegativePtr) = 0;
  void visit(NumberPtr) = 0;
};
void Plus::accept(VisitorPtr visitor) {
  visitor->visit(this);
}
void Minus::accept(VisitorPtr visitor) {
  visitor->visit(this);
}
void Multiplies::accept(VisitorPtr visitor) {
  visitor->visit(this);
}
void Divides::accept(VisitorPtr visitor) {
  visitor->visit(this);
}
void Negative::accept(VisitorPtr visitor) {
  visitor->visit(this);
}
void Positive::accept(VisitorPtr visitor) {
  visitor->visit(this);
}
void Number::accept(VisitorPtr visitor) {
  visitor->visit(this);
}

class Parser{
private:
  string data;
  string::size_type index;

  bool more(){
    while(index < data.size() && data[index]==' ') ++index;
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
      value = value * 10 + peek() - '0';
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
      if(peek()=='*')
        lhs = new Multiplies(lhs, term());
      else if(peek()=='/')
        lhs = new Divides(lhs, term());
      else exit(1);
    }

    return lhs;
  }
  ExpPtr exp(){
    ExpPtr lhs = term();
    while(more()){
      if(peek()=='+')
        lhs = new Plus(lhs, term());
      else if(peek()=='-')
        lhs = new Minus(lhs, term());
      else exit(1);
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
  Parser p("123");
  ExpPtr exp = p.parse();
  return 0;
}
