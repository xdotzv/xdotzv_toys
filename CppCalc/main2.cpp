#include <iostream>
#include <functional>
#include <sstream>
#include <cctype>
#include <stack>
#include <tuple>
#include <cassert>
#include <vector>


template<typename T>
class Exp{
public:
  virtual T eval() = 0;
};

template<typename T>
class Number: public Exp<T>{
private:
  T value;
public:
  Number(const T &v):value(v){}
  T eval() override{
    return value;
  }
};

template<typename T, T (*f)(const T&, const T&)>
class Binary: public Exp<T>{
private:
  Exp<T> *lhs, *rhs;
public:
  Binary(Exp<T> *l, Exp<T> *r): lhs(l), rhs(r){}
  T eval() override{
    return f(lhs->eval(), rhs->eval());
  }
};

namespace primitive{
  // maybe could use std::plus?
  template<typename T>
  T plus(const T &a, const T &b){
    return a+b;
  }
  template<typename T>
  T minus(const T &a, const T &b){
    return a-b;
  }
  template<typename T>
  T multiplies(const T &a, const T &b){
    return a*b;
  }
  template<typename T>
  T divides(const T &a, const T &b){
    return a/b;
  }
}

template<typename T>
using Plus = Binary<T, primitive::plus<T>>;
template<typename T>
using Minus = Binary<T, primitive::minus<T>>;
template<typename T>
using Multiplies = Binary<T, primitive::multiplies<T>>;
template<typename T>
using Divides = Binary<T, primitive::divides<T>>;


inline char next_operator(const std::string &s, int &i){
  return s[i++];
}


// end with '#'
template<typename T>
inline T next_number(const std::string &s, int &i){
  T ret = 0;

  while(isdigit(s[i])){
    ret = ret * 10 + s[i++] - '0';
  }
  return ret;
}

inline
int priority(char c){
  switch (c){
  case '+':
  case '-':
    return 1;
  case '*':
  case '/':
    return 2;
  case '(':
  case ')':
    return 0;
  }
}

using std::cout;
using std::endl;
template<typename T>

Exp<T>* construct(const std::string &str){

  std::stack<Exp<T>*> stk;
  std::stack<char> ops;
  
  std::string s("("+str+")");

  int i = 0;
  while(i < s.size()){
    while(s[i]==' ') i++;
    if(isdigit(s[i])){
      T num = next_number<T>(s, i);
      stk.push(new Number<T>(num));
    }
    else{
      auto op = next_operator(s, i);
      if(op=='(') {
        ops.push(op);
        continue;
      }
      while(priority(ops.top()) >= priority(op)){
        char c = ops.top();
        ops.pop();
        if(c=='(') break;
        auto e2 = stk.top(); stk.pop();
        auto e1 = stk.top(); stk.pop();
        Exp<T>* e;
        switch (c){
        case '+':
          e = new Plus<T>(e1, e2);
          break;
        case '-':
          e = new Minus<T>(e1, e2);
          break;
        case '*':
          e = new Multiplies<T>(e1, e2);
          break;
        case '/':
          e = new Divides<T>(e1, e2);
          break;
        }
        stk.push(e);
      }
      if(op!=')') ops.push(op);
    }
  }

  return stk.top();
}
int main(){

  
  std::vector<std::tuple<std::string, int>> test_cases;
  test_cases.push_back(std::make_tuple("1+(3-1)*(1+1)*2", 9));
  test_cases.push_back(std::make_tuple("1-1-1+1+1*2/3", 0));
  test_cases.push_back(std::make_tuple("((((( 1 )))))", 1));

  for(auto &test_case: test_cases){
    if(construct<int>(std::get<0>(test_case))->eval()!=std::get<1>(test_case))
       std::cout << "failed " << std::get<0>(test_case) << std::endl;

  }
  return 0;
}
