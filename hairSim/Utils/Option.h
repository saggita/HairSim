#ifndef OPTION_H
#define OPTION_H

class Option
{
public:

  enum Type {BOOL, INT, SCALAR, VEC, STRING};

  std::string name;
  Type type;
  bool b;
  int i;
  Scalar r;
  Vec3d v;
  std::string s;
  std::string label;

  Option()
  : name()
  , type()
  , b()
  , i()
  , r()
  , label()
  {}  
  
  Option(const std::string& opt_name, const std::string& desc, const bool& def)
    : name(opt_name)
    , type(BOOL)
    , b(def)
    , i(0)
    , r(0.0)
    , label(desc)
  {}

  Option(const std::string& opt_name, const std::string& desc, const int& def)
    : name(opt_name)
    , type(INT)
    , b(false)
    , i(def)
    , r(0.0)
    , label(desc)
  {}

  Option(const std::string& opt_name, const std::string& desc, const Scalar& def)
    : name(opt_name)
    , type(SCALAR)
    , b(false)
    , i(0)
    , r(def)
    , label(desc)
  {}

  Option(const std::string& opt_name, const std::string& desc, const std::string& def)
    : name(opt_name)
    , type(STRING)
    , b(false)
    , i(0)
    , r(0.0)
    , s(def)
    , label(desc)
  {}

  Option(const std::string& opt_name, const std::string& desc, const Vec3d& def)
    : name(opt_name)
    , type(VEC)
    , b(false)
    , i(0)
    , r(0.0)
    , v(def)
    , label(desc)
  {}

  Option(const std::string& opt_name, const std::string& desc, const char* def)
    : name(opt_name)
    , type(STRING)
    , b(false)
    , i(0)
    , r(0.0)
    , s(def)
    , label(desc)
  {}
    
};

#endif
