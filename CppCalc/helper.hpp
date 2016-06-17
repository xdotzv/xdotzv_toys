#ifndef HELPER_H
#define HELPER_H
#include "visitor.h"
#include <string>

class Traveler: public Visitor{
 private:
  int indent = 0;
  void print_indent();
public:
  virtual void visit(PlusPtr) override;
  virtual void visit(MinusPtr) override;
  virtual void visit(MultipliesPtr) override;
  virtual void visit(DividesPtr) override;
  virtual void visit(PositivePtr) override;
  virtual void visit(NegativePtr) override;
  virtual void visit(NumberPtr) override;


  //abstract class type
  void visit(BinaryPtr, const std::string &);
  void visit(UnaryPtr, const std::string &);
  void visit(ExpPtr);

};

#endif
