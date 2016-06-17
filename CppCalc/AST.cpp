#include "AST.hpp"
#include "visitor.hpp"

//Exp Binary Unary are abstract classes

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
