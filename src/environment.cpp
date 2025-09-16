#include "environment.hpp"

// Environment

Environment::Environment(Environment* parent)
   : parent(parent) {}

Environment::Environment()
   : parent(nullptr) {
   set("NOP", NumberValue::make(0));
   set("YES", NumberValue::make(1));
   set("NIL", Null::make());

   set("NMT", NumberValue::make(0));
   set("STT", NumberValue::make(1));
   set("FNT", NumberValue::make(2));
   set("NLT", NumberValue::make(3));
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
