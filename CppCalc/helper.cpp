#include "helper.h"
#include <iostream>

inline void print_indent(){
  for(int i=1; i<=indent; ++i) cout << ' ';
}

void Traveler::visit(ExpPtr p){
  p->accept(*this);
}

void Traveler::visit(BinaryPtr p, const std::string &s){
  print_indent();
  std::cout << s << std::endl;
  indent++;
  visit(p->lhs);
  visit(p->rhs);
  indent--;
}
void Traveler::visit(UnaryPtr p, const std:: string &s){
  print_indent();
  std::cout << s << std::endl;
  
}

#define define_visit_binary(name) void Traveler::visit(name##Ptr p) {visit(static_cast<BinaryPtr>(p), #name);}

define_visit_binary(Plus);
define_visit_binary(Minus);
define_visit_binary(Multiplies);
define_visit_binary(Divides);

void Traveler::visit(PositivePtr p) {
  visit(static_cast<UnaryPtr>(p), "positive");
}
void Traveler::visit(NegativePtr p) {
  visit(static_cast<UnaryPtr>(p), "negative");
}
void Traveler::visit(NumberPtr p) {
  print_indent();
  cout << p->value << endl;
}
