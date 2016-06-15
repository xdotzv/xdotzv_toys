#include <iostream>
#include <functional>
#include <sstream>
#include <cctype>
#include <stack>
#include <tuple>
#include <cassert>
#include <vector>
#include <unordered_set>



class NStateBase;

using SET = std::unordered_set<NStateBase*>;
using std::string;

class NStateBase{
public:
  virtual void theta_move(SET &) = 0;
};

class NStateAlt: public NStateBase{
private:
  NStateBase *alt1, *alt2;
public:
  NStateAlt(NStateBase *a1, NStateBase *a2):alt1(a1), alt2(a2){}

  void theta_move(SET &s) override {
    alt1->theta_move(s);
    alt2->theta_move(s);
  }
};

class NStateAtom: public NStateBase{
private:
  char c;
  NStateBase *next;
public:
  NStateBase(char c_,  NStateBase *next_): c(c_), next(next_){}
  void theta_move(SET &s) override{
    s.insert(this);
  }
};

class RE{
private:
  NStateBase* accept_state(){
    static NStateBase *accept = new NStateAtom(char, nullptr);
    return accept;
  }
  void construct(string::const_iterator beg, string::const_iterator end, NStateBase *next){
    
  }
public:
  RE(const string & s){
    
  }
};
int main(){

  return 0;
}
