#ifndef Template_G_H  // A header guard is introduced to avoid including
#define Template_G_H  // this header file twice in any source code file.

// All template code is in a one header file.

template <class E>
// This one says that the template parameter E is used as a type in the class.
class Stack
{
  private:
  struct Node // a struct type for Node
  {
     E  &element; // a reference to type E
     Node *next;
     Node(E &elem) : element(elem) {} // inline constructor for reference initialization
  };
  Node  *top;
  // Node  *top = NULL; // not allowed
  
  public:
   Stack();
   ~Stack();
   void  push(E &obj); // a reference argument
   E&    pop();        // a reference return type
   E&    peek();       // a reference return type
   bool  isEmpty();
};

template <class E> // constructor
Stack<E>:: Stack()
{
  top = NULL;
}

template <class E> // desstructor
Stack<E>:: ~Stack()
{
  while ( top != NULL )
   {
     Node *tmp = top;
     top = top->next;
     delete tmp;
   }
}

template <class E> // This line is necessary; it says that E is a template parameter.
void Stack<E>:: push(E &obj)
{
  Node *one = new Node(obj);
  one->next = top;
  top = one;
}

// Notice the syntax of the definition of member function pop:
//	template <class E>
//	E Stack<E>:: pop() 
// There are three E's in this declaration:
// The first one says that the template parameter E is used as a type.
// The second E refers to the type returned by the function.
// And the third E (the one between angle brackets) says that
// the function is defined inside the templated class Stack<E>.
template <class E>  // see what happens if this line is removed.
E& Stack<E>:: pop() // see what happens if <E> is removed.
{
  if ( top == NULL )
    fatal("The stack is empty");
  Node *tmp = top;
// E element = top->element; // an error: causing the reference to a local variable to be returned.
  E &element = top->element; // a reference type on the left, causing the original reference to be returned.
  top = top->next;
  delete tmp;
  return element;
}

template <class E>
E& Stack<E>:: peek()
{
  if ( top == NULL )
    fatal("The stack is empty");
  E &element = top->element;
//  cout << "Type of E: " << typeid(E).name() << endl;
  return element;
}

template <class E>
bool Stack<E>:: isEmpty()
{
  return top == NULL;
}

// An example of a function template
template <class T>          // The template parameter T is used as a type.
T findMin(Stack<T> &stack)
{
  Stack<T> tmp;
  if ( stack.isEmpty() )
    fatal("The stack is empty");
  T &ref = stack.pop(); // an error occurs if T ref is used. Why?
  tmp.push(ref);
  T  min = ref;
  while ( ! stack.isEmpty() )
   {
     T &element = stack.pop(); // the scope of element is limited to one iteration. 
     tmp.push(element);
     if ( element <= min )
        min = element;
   }
// T &fake = element; // an error.

  while ( ! tmp.isEmpty() )   // restores stack
     stack.push( tmp.pop() );

  return min;
}

// Another example of a function template, which returns a smallest reference in the stack.
template <class T>          // The template parameter T is used as a type.
T& findMinRef(Stack<T> &stack)
{
  Stack<T> tmp, back;
  if ( stack.isEmpty() )
    fatal("The stack is empty");
  T &ref = stack.pop();
  tmp.push(ref);
  back.push(ref);
  T  min = ref;
  while ( ! stack.isEmpty() )
   {
     T &element = stack.pop(); // the scope of element is limited to one iteration. 
     tmp.push(element);
     back.push(element);
     if ( element <= min )
        min = element;
   }

  while ( ! tmp.isEmpty() )   // restores stack
     stack.push( tmp.pop() );

  while ( ! back.isEmpty() )  // finds a reference with the same contents.
   {
     T &elem = back.pop();
     if ( elem <= min )
       return elem;
   }
  return ref;
}

#endif
