#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100 // Maximum size for the stack

/*
 * add, subtract, multiply, divide:
 * Basic arithmetic operations
 */
double add(double a, double b) { return a + b; }

/*
 * subtract:
 * Subtract two numbers
 */
double subtract(double a, double b) { return a - b; }

/*
 * multiply:
 * Multiply two numbers
 */
double multiply(double a, double b) { return a * b; }

/*
 * divide:
 * Divide two numbers
 */
double divide(double a, double b) { return a / b; }

// Stack structure for operands
typedef struct {
  double items[MAX]; // Array to hold stack items
  int top;           // Index of the top item in the stack
} Stack;

// Function to initialize the stack
void initStack(Stack *s) {
  s->top = -1; // Set the top index to -1 indicating the stack is empty
}

// Function to check if the stack is empty
int isEmpty(Stack *s) {
  return s->top == -1; // Return true if top index is -1
}

// Function to push an item onto the stack
void push(Stack *s, double item) {
  if (s->top < MAX - 1) {        // Check for stack overflow
    s->items[++(s->top)] = item; // Increment top and add item
  } else {
    printf("Stack overflow\n"); // Error message for overflow
  }
}

// Function to pop an item from the stack
double pop(Stack *s) {
  if (!isEmpty(s)) {             // Check if the stack is not empty
    return s->items[(s->top)--]; // Return the top item and decrement top
  } else {
    printf("Stack underflow\n"); // Error message for underflow
    return 0;                    // Return 0 in case of underflow
  }
}

// print file from current directory
void printFile(char *filename) {
  FILE *file = fopen(filename, "r"); // Open the file in read mode
  if (file == NULL) {               // Check if the file was opened successfully
    printf("Error opening file\n"); // Print an error message
    return;                         // Return from the function
  }

  char line[MAX]; // Array to store each line of the file

  // Read and print each line of the file
  while (fgets(line, sizeof(line), file)) {
    printf("%s", line); // Print the line
  }

  fclose(file); // Close the file
}

// Function to get the precedence of operators
int precedence(char op) {
  switch (op) {
  case '+':
  case '-':
    return 1; // Lower precedence for addition and subtraction
  case '*':
  case '/':
    return 2; // Higher precedence for multiplication and division
  default:
    return 0; // Return 0 for non-operators
  }
}

// Function to perform arithmetic operations
double applyOp(double a, double b, char op) {
  switch (op) {
  case '+':
    return add(a, b); // Addition
  case '-':
    return subtract(a, b); // Subtraction
  case '*':
    return multiply(a, b); // Multiplication
  case '/':
    return divide(a, b); // Division
  default:
    return 0; // Return 0 for invalid operator
  }
}

// Function to evaluate the mathematical expression
double evaluate(char *expression) {
  Stack values;       // Stack to hold numeric values
  Stack ops;          // Stack to hold operators
  initStack(&values); // Initialize the values stack
  initStack(&ops);    // Initialize the operators stack

  // Loop through each character in the expression
  for (int i = 0; i < strlen(expression); i++) {
    // Skip whitespace characters
    if (isspace(expression[i]))
      continue;

    // If the character is a digit, parse the number and push it to the values
    // stack
    if (isdigit(expression[i])) {
      double val = 0; // Initialize value
      while (i < strlen(expression) && isdigit(expression[i])) {
        val = val * 10 + (expression[i] - '0'); // Build the number
        i++;                                    // Move to the next character
      }
      push(&values, val); // Push the parsed number onto the values stack
      i--;                // Adjust for the extra increment
    }
    // If the character is an operator
    else {
      // While the top of the ops stack has the same or greater precedence
      while (!isEmpty(&ops) &&
             precedence(ops.items[ops.top]) >= precedence(expression[i])) {
        // Pop two values and one operator to apply the operation
        double val2 = pop(&values); // Second operand
        double val1 = pop(&values); // First operand
        char op = pop(&ops);        // Operator
        // Apply the operator to the operands and push the result back to the
        // values stack
        push(&values, applyOp(val1, val2, op));
      }
      // Push the current operator onto the ops stack
      push(&ops, expression[i]);
    }
  }

  // Apply remaining operators in the ops stack
  while (!isEmpty(&ops)) {
    double val2 = pop(&values); // Second operand
    double val1 = pop(&values); // First operand
    char op = pop(&ops);        // Operator
    // Apply the operator to the operands and push the result back to the values
    // stack
    push(&values, applyOp(val1, val2, op));
  }

  // The final result will be the only value left in the values stack
  return pop(&values);
}

int main() {
  char expression[MAX]; // Array to hold the input expression
  printf("Enter a mathematical expression: ");
  fgets(expression, sizeof(expression), stdin); // Read the input expression
  // Remove newline character from input
  expression[strcspn(expression, "\n")] = 0;

  printFile("./calculator.c"); // Print the contents of the file

  // Evaluate the expression and store the result
  double result = evaluate(expression);
  // Print the result with two decimal places
  printf("Result: %.2f\n", result);
  return 0; // Indicate successful completion
}
