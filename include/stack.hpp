#ifndef STACK_HPP
#define STACK_HPP

// Stack

namespace stack {
   void push(long value);
   long top();
   long pop();
   unsigned long size();
   bool empty();
}

// Registers

namespace reg {
   void set(long index, long value);
   long get(long index);
}

#endif
