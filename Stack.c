/* COP3502C Midterm Assignment
This program is written by: Shivani Kumar */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 100 /* stack size */

char stack[SIZE];
int top = -1;
int stack_int[SIZE];
int top_int = -1;

void push (char x)
{ /* if something is pushed */
  stack[++top] = x;
}

int pop()
{ /* if something is popped */
  return (stack[top--]);
}

int isEmpty() /* when we are checking if the stack is empty */
{
  if (top < 0)
  { /* if top goes back to being -1 and not 0, then the stack is empty */
    return 1;
  }
  else
  { /* but if top is greater than 0, than that means the stack isn't empty */
    return 0;
  }
}

int isFull() /* when we want to check if stack is full */
{
  if (top >= SIZE -1)
  { /* if stack's top is greater than the size (minus 1) than we know it's full */
    return 1;
  }
  else
  {/* but if it isn't equal to or greater than, then we know its not full */
    return 0;
  }
}

char peek()
{/* this is just to peek at the top of the stack (to see parenthesis) */
  return stack[top];
}

void push_int(char x) /* same concept as push but for evaluation */
{
  top_int++;
  stack_int[top_int] = x;
}

int pop_int() /* same concept as pop but for evaluation */
{
    int retval;

    retval = stack_int[top_int];
    top_int--;

    return retval;
}

int power(int b, int p)
{
  int i, total = 1;
  for(i = 0; i<p; i++)
  { /* to get base to whatever power needed */
    total = total * b;
  }
  return total;
}

int isBalancedParenthesis(char *c)
{ /* if it starts with ( and ends with ) then it will be empty */
  int i = 0;
  while (c[i])
  {
    if (c[i] == '(')
    { /* if it starts with a ( it gets pushed */
      push(c[i]);
    }
    if (c[i] == ')')
    {
      if (top < 0) /* if there was not beginning ( then it returns 0 */
        return 0;
      else
      { /* gets popped if there is a matching ending ) to it */
        pop();
      }
    }
    i++;
  }
  if (top < 0)
    { /* if it's empty, it is balanced */
      return 1;
    }
  else
    { /* it it is not empty, it is not balanced */
      return 0;
    }
}

int convertToInt(char c)
{ /* subtracting '0' makes char to int */
  int x;
  x = c - '0';
  return x;
}

int isOperator(char c)
{
  if (c == '+' || c == '=' || c == '*' || c == '^' || c == '/' || c == '%')
  { /* if it is an operator */
    return 1;
  }
  else
  {
    return 0;
  }
}

int getOperatorPriority(char c)
{
    if (c == '(') /* parenthesis */
      return 1;
    else if ((c == '+') || (c == '-')) /* addition or subtraction */
      return 2;
    else if ((c == '*') || (c == '%') || (c == '/')) /* multiplication & division */
      return 3;
    else if (c == '^') /* power has highest priority */
      return 4;
    else
      return -1;
}

void calculate(int a, int b, char op)
{ /* using a switch bc there will be way too many if statements */
  int val;
  switch(op)
  {
    case '+':
      val = a + b;
      break;
    case '-':
      val = a - b;
      break;
    case '*':
      val = a * b;
      break;
    case '/':
      val = a / b;
      break;
    case '^':
      val = power(a, b);
      break;
    case '%':
      val = a % b;
      break;
  }
  push_int(val);
}

void doubleDigits(char* infix)
{ /* from notes: separates numbers and adds them to global variable array */
  char* p;
  p = infix;

  while (*p)
  {
    if (isdigit(*p))
    {
      long value = strtol(p, &p, 10);
    }
    else
    {
      p++;
    }
  }
}

char* menu()
{/* we have to declare the infix we are getting from user */
  char *infix;
  char input;

  printf("\nMenu:\n");
  printf("Enter e to enter an infix\n");
  printf("Enter x to exit the program\n");

  while (1)
  {
    scanf("%c", &input); /* getting the user's input */
    if (input == 'x')
    {
      return ("exit");
    }
    else if (input == 'e')
    {
      printf("Enter infix or type in 'exit' to exit: ");
      scanf(" %[^\n]s", infix);
      doubleDigits(infix);
      return infix;
    }
  }
}

char* convertToPostfix(char *infix)
{
  char *postfix;
  char tmp, top;
  int i = 0, k = 0;

  while((tmp = infix[i++]) != '\0')
  {
    if (isdigit(tmp)) /* we are appending it to postfix if it is a digit*/
    {
      postfix[k] = tmp;
      k++;
    }
    else if (tmp == '(') /* if starting of parenthesis, then push */
    {
      push(tmp);
    }
    else if (tmp == ')') /* if ')' then we pop and add to the postfix till we reach ')' */
    {
        while(!isEmpty() && peek() != '(')
        {
          postfix[k]= pop();
          k++;
        }
        top = pop(); /* pops the ( symbol */
    }
    /* the top of the stack is greater than or equal to the current operator
      and then push to stack */
    else if (getOperatorPriority(tmp) > getOperatorPriority(stack[top]))
    {
      push(tmp);
    }
    else
    {
  /* while it is greater than / equal to the current, then you pop postfix */
      while(getOperatorPriority(stack[top]) > getOperatorPriority(tmp))
      {
        postfix[k] = pop();
        k++;
      }
      push(tmp);
    }
  }

  while (!isEmpty()) /* emptying the last values if empty isn't empty */
  {
    postfix[k] = pop();
    k++;
  }
  /* add null character to be able to terminate the postfix expression */
  postfix[k] = '\0';
  return postfix;
}

int evaluate(char *postfix) /* evaluates the postfix */
{
  char tmp;
  int a, b, i = 0;

  while((tmp = postfix[i++]) != '\n' && tmp != '\0')
    {
    if (isOperator(tmp) != 1)
    {
      tmp = convertToInt(tmp);
      push_int(tmp);
    }
    else if (tmp == '+' || tmp == '-' || tmp == '*' || tmp == '/' || tmp == '%' || tmp == '^')
    { /* it goes through the process of being calculated */
      a = pop_int();
      b = pop_int();
      calculate(a, b, tmp);
    }
  }
  return pop_int();
}

int main(void) /* function provided by prof */
{
  char *str, *postFix;

  while(strcmp(str = menu(), "exit") != 0)
  {
    if (isBalancedParenthesis(str))
    {
      printf("Parenthesis Balanced.\n");
      postFix = convertToPostfix(str);
      printf("Postfix is %s\n", postFix);
      printf("Result is %d\n", evaluate(postFix));
    }
    else
    {
      printf("Parenthesis imbalanced");
    }
  }
return 0;
}
