#include "printer.hpp"

using std::endl;

void Printer::visit(BinaryPtr p, const std::string &s){
  print_indent();
  os << s << std::endl;
  indent++;
  visit(p->lhs);
  visit(p->rhs);
  indent--;
}
void Printer::visit(UnaryPtr p, const std:: string &s){
  print_indent();
  os << s << std::endl;
  indent++;
  visit(p->p);
  indent--;
}

#define define_visit_binary(name) void Printer::visit(name##Ptr p) {visit(static_cast<BinaryPtr>(p), #name);}

define_visit_binary(Plus);
define_visit_binary(Minus);
define_visit_binary(Multiplies);
define_visit_binary(Divides);

void Printer::visit(PositivePtr p) {
  visit(static_cast<UnaryPtr>(p), "positive");
}
void Printer::visit(NegativePtr p) {
  visit(static_cast<UnaryPtr>(p), "negative");
}
void Printer::visit(NumberPtr p) {
  print_indent();
  os << p->value << endl;
}

