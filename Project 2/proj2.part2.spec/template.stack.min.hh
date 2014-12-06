//@author Aaron Jacobson ajacob1

#ifndef TEMPLATE_STACK_MIN_H
#define TEMPLATEA_STACK_MIN_H

#include <iostream> // provides objects like cin and cout for sending data
                    // to and from the standard streams input and output.
		    // These objects are part of the std namespace.
#include <cstdlib>  // has exit etc
#include <typeinfo> // This header file is included for using typeid.
#include <fstream>  // file streams and operations
#include <sstream>  // string streams and operations
using namespace std; // a container for a set of identifiers.
                     // Because of this line, there is no need to use std::cout
#include <stdexcept> // exceptions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wrapper.hh"

#define CHAR_SIZE  50 // a macro definition

void fatal(const char *msg);
void fatal(const char *msg, const char *info);

template <class T>
// This one says that the template parameter T is used as a type in the class.
class Node  // a separate class template
{
  public:
   T  *element; // a pointer as an element of the stack
   Node *next;  // a pointer to Node<T>
};

template <class E>
// This one says that the template parameter E is used as a type in the class.
class Stack
{
  private:
   Node<E>  *top;
  
  public:
   Stack();  // constructor
   ~Stack(); // destructor
   void  push(E *obj); // a pointer argument
   E*    pop();        // a pointer return type
   E*    peek();       // a pointer return type
   bool  isEmpty();
};

template <class E> // constructor
Stack<E>:: Stack()
{
  top = NULL;
}

template <class E>  // desstructor
Stack<E>:: ~Stack()
{
  while ( top != NULL )
   {
     Node<E> *tmp = top;
     top = top->next;
     delete tmp;
   }
}

template <class E> // This line is necessary; it says that E is a template parameter.
void Stack<E>:: push(E *obj) // a pointer argument
{
  Node<E> *one = new Node<E>;
  one->element = obj;
  one->next = top;
  top = one;
}

template <class E>  
E* Stack<E>:: pop() 
{
  if (top == NULL)
    {
      fatal("The stack is empty");
    }
  Node<E> *tmp = top;
  E *element = top->element;
  top = top->next;
  delete tmp;
  return element;
}

template <class E>
E* Stack<E>:: peek() // a pointer return type
{
  if (top == NULL)
    {
      fatal("The stack is empty");
    } 
  E *element = top->element;
  return element;
}

template <class E>
bool Stack<E>:: isEmpty()
{
  return top == NULL;
}

template <class E>         
E* findMin(Stack<E> &stack)
{
  Stack<E> tmp;
  if (stack.isEmpty())
  {
    fatal("The stack is empty");
  }
  E &ref = stack.pop(); 
  tmp.push(ref);
  E  min = ref;
  while (!stack.isEmpty())
   {
      E &element = stack.pop(); 
      tmp.push(element);
      if (element <= min)
      {
        min = element;
      }
   }

  while ( ! tmp.isEmpty() )   // restores stack
  {
    stack.push( tmp.pop() );
  }
  return min;
}

template <class E>          
E* findMax(Stack<E> &stack)
{
  Stack<E> tmp;
  if (stack.isEmpty())
  {
    fatal("The stack is empty");
  }
  E &ref = stack.pop(); 
  tmp.push(ref);
  E  min = ref;
  while (!stack.isEmpty())
  {
     E &element = stack.pop(); 
     tmp.push(element);
     if (min <= element)
     {
          min = element;
     }
  }
  while (!tmp.isEmpty())
  {
    stack.push( tmp.pop() );
  }
  return min;
}

#endif




