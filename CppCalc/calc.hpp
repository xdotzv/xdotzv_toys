#ifndef CALC_H
#define CALC_H
#include "visitor.hpp"
#include <functional>

class Calc: public Visitor{

public:
  int result;
  void visit(PlusPtr) override;
  void visit(MinusPtr) override;
  void visit(MultipliesPtr) override;
  void visit(DividesPtr) override;
  void visit(PositivePtr) override;
  void visit(NegativePtr) override;
  void visit(NumberPtr) override;

  void visit(BinaryPtr , std::function<int(int, int)>);
  void visit(UnaryPtr , std::function<int(int)>);


  void visit(ExpPtr ptr){
    ptr->accept(this);
  }
};

#endif
