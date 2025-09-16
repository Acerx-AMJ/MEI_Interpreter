#include "stack.hpp"

// Includes

#include <stack>
#include <unordered_map>

std::stack<long> stck;
std::unordered_map<long, long> registers;

// Stack

namespace stack {
   void push(long value) {
      stck.push(value);
   }

   long top() {
      return (stck.empty() ? 0 : stck.top());
   }

   long pop() {
      if (stck.empty()) {
         return 0;
      }
      long value = stck.top();
      stck.pop();
      return value;
   }

   unsigned long size() {
      return stck.size();
   }

   bool empty() {
      return stck.empty();
   }
}

// Registers

namespace reg {
   void set(long index, long value) {
      registers[index] = value;
   }

   long get(long index) {
      return registers[index];
   }
}
