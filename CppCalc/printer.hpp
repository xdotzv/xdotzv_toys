#ifndef PRINTER_H
#define PRINTER_H
#include "visitor.hpp"
#include <iostream>

class Printer: public Visitor{
private:
  int indent;
  std::ostream& os;
  void print_indent(){
    for(int i=1; i<=indent; i++) os << "  ";
  }
public:
  Printer(std::ostream &__os):indent(0), os(__os){
  };
  void visit(PlusPtr) override;
  void visit(MinusPtr) override;
  void visit(MultipliesPtr) override;
  void visit(DividesPtr) override;
  void visit(PositivePtr) override;
  void visit(NegativePtr) override;
  void visit(NumberPtr) override;

  //abstract class type 
  void visit(BinaryPtr, const std::string &);
  void visit(UnaryPtr, const std::string &);
  void visit(ExpPtr ptr){
    ptr->accept(this);
  }
};

#endif
