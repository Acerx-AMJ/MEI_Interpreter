#include "environment.hpp"

// Environment

Environment::Environment(Environment* parent)
   : parent(parent) {}

Environment::Environment()
   : parent(nullptr) {
   set("No", NumberValue::make(0));
   set("Yes", NumberValue::make(1));
   set("Nil", Null::make());

   set("Number_t", NumberValue::make(0));
   set("String_t", NumberValue::make(1));
   set("Fun_t", NumberValue::make(2));
   set("Array_t", NumberValue::make(3));
   set("Nil_t", NumberValue::make(4));
}

// Functions

void Environment::set(const std::string& identifier, Value value) {
   vars[identifier] = value;
}

Value Environment::get(const std::string& identifier) {
   auto& env = resolve(identifier);
   return env.vars[identifier];
}

Environment& Environment::resolve(const std::string& identifier) {
   if (vars.find(identifier) != vars.end()) {
      return *this;
   }

   if (!parent) {
      std::cerr << "Variable '" << identifier << "' does not exist.\n";
      std::exit(1);
   }
   return parent->resolve(identifier);
}
