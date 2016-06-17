#ifndef VISITOR_H
#define VISITOR_H

class Visitor{
public:
  virtual void visit(PlusPtr) = 0;
  virtual void visit(MinusPtr) = 0;
  virtual void visit(MultipliesPtr) = 0;
  virtual void visit(DividesPtr) = 0;
  virtual void visit(PositivePtr) = 0;
  virtual void visit(NegativePtr) = 0;
  virtual void visit(NumberPtr) = 0;
};

using VisitorPtr = Visitor *;
#endif
