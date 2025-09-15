#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// Includes

#include "values.hpp"
#include <unordered_map>

// Environment

class Environment {
   Environment* parent;
   std::unordered_map<std::string, Value> vars;

public:
   Environment(Environment* parent);
   Environment();

   void set(const std::string& identifier, Value value);
   Value get(const std::string& identifier);
   Environment& resolve(const std::string& identifier);
};

#endif
