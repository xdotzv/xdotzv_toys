#include "calc.hpp"



void Calc::visit(BinaryPtr ptr, std::function<int(int, int)> f){
  visit(ptr->lhs);
  int lhs = result;
  visit(ptr->rhs);
  int rhs = result;
  result = f(lhs, rhs);
}
void Calc::visit(PlusPtr ptr){
  visit(static_cast<BinaryPtr>(ptr), [](int x, int y){return x+y;});
}
void Calc::visit(MinusPtr ptr){
  visit(static_cast<BinaryPtr>(ptr), [](int x, int y){return x-y;});
}
void Calc::visit(MultipliesPtr ptr){
  visit(static_cast<BinaryPtr>(ptr), [](int x, int y){return x*y;});
}
void Calc::visit(DividesPtr ptr){
  visit(static_cast<BinaryPtr>(ptr), [](int x, int y){return x/y;});
}

void Calc::visit(UnaryPtr ptr, std::function<int(int)> f){
  visit(ptr->p);
  result = f(result);
}
void Calc::visit(PositivePtr ptr) {
  visit(static_cast<UnaryPtr>(ptr), [](int x){return x;});
}
void Calc::visit(NegativePtr ptr) {
  visit(static_cast<UnaryPtr>(ptr), [](int x){return -x;});
}
void Calc::visit(NumberPtr ptr) {
  result = ptr->value;
}

