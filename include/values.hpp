#ifndef VALUES_HPP
#define VALUES_HPP

#include "ast.hpp"
#include <iostream>

// Value

enum class ValueType {
   number, string, fn, null
};

struct ValueLiteral;
using Value = std::shared_ptr<ValueLiteral>;

struct ValueLiteral {
   ValueType type;

   ValueLiteral(ValueType type)
      : type(type) {}
   virtual ~ValueLiteral() = default;

   void print() const {
      std::cout << as_string();
   }
   virtual std::string as_string() const = 0;
   virtual long as_number() const = 0;
   virtual bool as_bool() const = 0;
};

// Number value

struct NumberValue : public ValueLiteral {
   long number;

   NumberValue(long number)
      : number(number), ValueLiteral(ValueType::number) {}
   
   static Value make(long number) {
      return std::make_shared<NumberValue>(number);
   }

   std::string as_string() const override { return std::to_string(number); }
   long as_number() const override { return number; }
   bool as_bool() const override { return number; }
};

// String value

struct StringValue : public ValueLiteral {
   std::string string;

   StringValue(const std::string& string)
      : string(string), ValueLiteral(ValueType::string) {}
   
   static Value make(const std::string& string) {
      return std::make_shared<StringValue>(string);
   }

   std::string as_string() const override { return string; }
   long as_number() const override { return string.size(); }
   bool as_bool() const override { return !string.empty(); }
};

// Function value

struct Environment;

struct Fn : public ValueLiteral {
   std::string identifier;
   std::vector<std::string> params;
   Environment* env;
   Stmt body;

   Fn(const std::string& identifier, const std::vector<std::string>& params, Environment* env, Stmt body)
      : identifier(identifier), params(params), env(env), body(body), ValueLiteral(ValueType::fn) {}
   
   static Value make(const std::string& identifier, const std::vector<std::string>& params, Environment* env, Stmt body) {
      return std::make_shared<Fn>(identifier, params, env, body);
   }

   std::string as_string() const override { return identifier; }
   long as_number() const override { return identifier.size(); }
   bool as_bool() const override { return false; }
};

// Null value

struct Null : public ValueLiteral {
   Null() : ValueLiteral(ValueType::null) {}
   static Value make() { return std::make_shared<Null>(); }

   std::string as_string() const override { return ""; }
   long as_number() const override { return 0; }
   bool as_bool() const override { return false; }
};

#endif
