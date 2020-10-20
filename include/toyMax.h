// File: toyMax.h
  
#ifndef TOYMAX_H__
#define TOYMAX_H__

#include <string>

#include <ilang/ilang++.h>

namespace ilang {

namespace toyMax {

Ila GetToymaxIla(const std::string& model_name = "toyMax");

void AddChild(Ila& m);

};

};

#endif // TOYMAX_H__
