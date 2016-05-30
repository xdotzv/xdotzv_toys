#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

using namespace std;

class BaseFoo{
 public:
  virtual void foo() = 0;
};

class FooA: public BaseFoo{
public:
  static BaseFoo* Create(){ return new FooA; }
  void foo() override{
    cout << "hello from fooA" << endl;
  }
};

class FooB: public BaseFoo{
public:
  static BaseFoo* Create(){ return new FooB; }
  void foo() override{
    cout << "hello from fooB" << endl;
  }
};

class FooFactory{
public:  
  typedef function<BaseFoo*()> createFuncT;
  static BaseFoo* Create(const string &name);
  static bool Register(const string &name, createFuncT func);
private:
  static unordered_map<string, createFuncT> register_map;
};


unordered_map<string, FooFactory::createFuncT> FooFactory::register_map = {};

BaseFoo* FooFactory::Create(const string &name){
  auto it = register_map.find(name);
  if(it != register_map.end())
    return it->second();
  return nullptr;
}

bool FooFactory::Register(const string &name, createFuncT func){
  auto ret = register_map.insert({name, func});
  return ret.second;
}

int main() {

  vector<shared_ptr<BaseFoo>> objs;
  
  FooFactory::Register("FooA", FooA::Create);
  FooFactory::Register("FooB", FooB::Create);

  objs.push_back(shared_ptr<BaseFoo>(FooFactory::Create("FooA")));
  objs.push_back(shared_ptr<BaseFoo>(FooFactory::Create("FooB")));
  
  for(auto p: objs){
    p->foo();
  }
  return 0;
}
