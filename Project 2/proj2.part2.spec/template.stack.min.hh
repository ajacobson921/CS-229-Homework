//@author Aaron Jacobson ajacob1
// This is template code from class, downloaded from Blackboard

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

// Notice the syntax of the definition of member function pop:
// //      template <class E>
// //      E Stack<E>:: pop() 
// // There are three E's in this declaration:
// // The first one says that the template parameter E is used as a type.
// // The second E refers to the type returned by the function.
// // And the third E (the one between angle brackets) says that
// // the function is defined inside the templated class Stack<E>.
template <class E>  // see what happens if this line is removed.
E* Stack<E>:: pop() // see what happens if <E> is removed.
{
  if ( top == NULL )
    fatal("The stack is empty");
  Node<E> *tmp = top;
  E *element = top->element;
  top = top->next;
  delete tmp;
  return element;
}

template <class E>
E* Stack<E>:: peek() // a pointer return type
{
  if ( top == NULL )
    fatal("The stack is empty");
  E *element = top->element;
  return element;
}

template <class E>
bool Stack<E>:: isEmpty()
{
  return top == NULL;
}

// Prints out an error message and stops.
void fatal(const char *msg)
{
   cerr << "Error message: " << msg << endl;
   exit(1); // stops unexpectedly.
}

// Prints out an error message along with additional information and stops.
void fatal(const char *msg, const char *info)
{
  cerr << "Error message: " << msg << " " << info << endl;
  exit(1); // stops unexpectedly.
}