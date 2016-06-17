#ifndef AST_H
#define AST_H

class Visitor;
using VisitorPtr = Visitor*;


struct Exp{
  virtual void  accept(VisitorPtr ) = 0;
};
using ExpPtr = Exp *;


struct Binary: public Exp{
  ExpPtr lhs, rhs;
  Binary(ExpPtr _lhs, ExpPtr _rhs):lhs(_lhs), rhs(_rhs){}
};

struct Plus: public Binary{
  void accept(VisitorPtr ) override;
  Plus(ExpPtr _lhs, ExpPtr _rhs):Binary(_lhs, _rhs){}
};
struct Minus: public Binary{
  void accept(VisitorPtr ) override;
  Minus(ExpPtr _lhs, ExpPtr _rhs):Binary(_lhs, _rhs){}
};
struct Multiplies: public Binary{
  void accept(VisitorPtr ) override;
  Multiplies(ExpPtr _lhs, ExpPtr _rhs):Binary(_lhs, _rhs){}
};
struct Divides: public Binary{
  void accept(VisitorPtr ) override;
  Divides(ExpPtr _lhs, ExpPtr _rhs):Binary(_lhs, _rhs){}
};


struct Unary: public Exp{
  ExpPtr p;
  Unary(ExpPtr _p):p(_p){}
};
struct Negative: public Unary{
  void accept(VisitorPtr ) override;
  Negative(ExpPtr _p):Unary(_p){}
};
struct Positive: public Unary{
  void accept(VisitorPtr ) override;
  Positive(ExpPtr _p):Unary(_p){}
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

#endif
