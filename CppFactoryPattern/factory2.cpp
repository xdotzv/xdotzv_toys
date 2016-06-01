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
  static BaseFoo* Create(int i_, int j_){ return new FooA(i_, j_); }
  FooA(int i_, int j_):i(i_), j(j_){}
  void foo() override{
    cout << "hello from fooA with " << i << " " << j << endl;
  }
private:
  int i,j;
};

class FooB: public BaseFoo{
public:
  static BaseFoo* Create(string s_){ return new FooB(s_); }
  FooB(string s_):s(s_){}
  void foo() override{
    cout << "hello from fooB " << s << endl;
  }
private:
  string s;
};

class FooFactory{
public:  
  //typedef function<BaseFoo*()> createFuncT;
  template<typename... Args>
  static BaseFoo* Create(const string &name, Args&&... args);

  template<typename F>
  static bool Register(const string &name, F func);

private:
  static unordered_map<string, void*> register_map;
};

unordered_map<string, void*> FooFactory::register_map = {};

template<typename... Args>
BaseFoo* FooFactory::Create(const string &name, Args&&... args){
  auto it = register_map.find(name);
  if(it != register_map.end()){
    typedef BaseFoo* (funcT)(Args...);
    return reinterpret_cast<funcT*>(it->second)(args...);
  }
  return nullptr;
}

template<typename F>
bool FooFactory::Register(const string &name, F func){
  auto ret = register_map.insert({name, reinterpret_cast<void*>(func)});
  return ret.second;
}

int main() {

  vector<shared_ptr<BaseFoo>> objs;
  
  FooFactory::Register("FooA", FooA::Create);
  FooFactory::Register("FooB", FooB::Create);

  objs.push_back(shared_ptr<BaseFoo>(FooFactory::Create("FooA", 1, 3)));
  objs.push_back(shared_ptr<BaseFoo>(FooFactory::Create("FooB", string(":D"))));
  
  for(auto p: objs){
    p->foo();
  }
  return 0;
}
