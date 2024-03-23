#include "gate.h"

using namespace std;

gate::gate() {  // Default constructor initialises all attributes to their default values
  component_name = "";
  num_of_inputs = 0;
  output_expression = "";
  delay_ps = 0;
  output.value = false;
  output.timeStamp = 0;
  output.name = "";
  precedence = {{'~', 3}, {'&', 2}, {'|', 1}};  // initialised precedence so that the not has the highest precedence and or has the lowest precedence
}

void gate::set_inputs(int i, string name, bool value) {  // Setter function for inputs
  if (inputs.size() != num_of_inputs) {
    inputs.resize(num_of_inputs);
  }
  inputs[i].name = name;
  inputs[i].value = value;
}

void gate::setInputTime(int i, int time) {    // Setter function for input time stamps
  if (inputs.size() != num_of_inputs) {
    inputs.resize(num_of_inputs);
  }
  inputs[i].timeStamp = time;
}

vector<data> gate::get_inputs() { return inputs; }    // Getter function returns inputs vector

void gate::setOutputName(string name) { output.name = name; }  // Setter function for output name

void gate::setOutputValue(bool val) { output.value = val; }    // Setter function for output value

void gate::setOutputTime(int time) { output.timeStamp = time; } // Setter function for output time stamp

data gate::get_output() { return output; }    // Getter function returns output

void gate::set_component_name(string name) { component_name = name; }  // Setter function for component name

string gate::get_component_name() { return component_name; }    // Getter function returns component name

void gate::set_num_of_inputs(int num_of_inputs) {    // Setter function for number of inputs
  this->num_of_inputs = num_of_inputs;
}

int gate::get_num_of_inputs() { return num_of_inputs; }    // Getter function returns number of inputs

void gate::set_output_expression(string output_expression) {  // Setter function for output expression
  this->output_expression = output_expression;
}

string gate::get_output_expression() { return output_expression; }   // Getter function returns output expression

void gate::set_delay_ps(int delay_ps) { this->delay_ps = delay_ps; }  // Setter function for delay in ps

void gate::printInputs() {  // Prints the inputs of the gate
  if (num_of_inputs == 0) {
    cout << "No inputs" << endl;
    return;
  }
  for (int i = 0; i < num_of_inputs; i++) {
    cout << inputs[i].name << " " << inputs[i].value << " "
         << inputs[i].timeStamp << endl;
  }
}

void gate::evaluateOutput() {    // Evaluates the output of the gate
  vector<string> x = convertExpression();    // Stores vector of elements in postfix notation in x
  bool val = evaluateExpression(x);         // Stores the value of the expression considering the value of the inputs in val 
  setOutputValue(val);                      // Sets the value of the output to val
}

int gate::getOperatorPrecedence(char op) { 
  auto it = precedence.find(op);   // Find the operator in the map
  if (it != precedence.end()) {    // If found, return the precedence value
    return it->second;
  }
  return 0;
}

vector<string> gate::convertExpression() {
  stack<char> operatorStack; // Stack to store operators during conversion
  vector<string> postfixExpression; // Vector to store the postfix expression
  string temp; // Temporary string to store operands

  // Iterate through each character in the output expression
  for (char character : output_expression) {
    if (isalpha(character) || isdigit(character)) {
      temp.push_back(character); // If the character is an operand, append it to the temporary string
    } else {
      if (!temp.empty()) {
        postfixExpression.push_back(temp); // Push the operand to the postfix expression vector
        temp.clear();
      }
      if (character == '(') {
        operatorStack.push(character); // Push '(' onto the operator stack
      } else if (character == ')') {
        // If the character is ')', pop operators from the stack until '(' is encountered
        while (!operatorStack.empty() && operatorStack.top() != '(') {
          postfixExpression.push_back(string(1, operatorStack.top()));
          operatorStack.pop();
        }
        operatorStack.pop(); // Discard '('
      } else {
        // If the character is an operator, pop operators with higher or equal precedence from the stack
        // and push them to the postfix expression
        while (!operatorStack.empty() &&
               getOperatorPrecedence(character) <= getOperatorPrecedence(operatorStack.top())) {
          postfixExpression.push_back(string(1, operatorStack.top()));
          operatorStack.pop();
        }
        operatorStack.push(character); // Push the current operator onto the stack
      }
    }
  }

  if (!temp.empty()) {
    postfixExpression.push_back(temp); // Push the remaining operand to the postfix expression vector
    temp.clear();
  }

  // Pop remaining operators from the stack and push them to the postfix expression
  while (!operatorStack.empty()) {
    postfixExpression.push_back(string(1, operatorStack.top()));
    operatorStack.pop();
  }

  return postfixExpression; // Return the postfix expression
}

bool gate::evaluateExpression(const vector<string> &postfixExpression) {
  stack<bool> operandStack; // Stack to store operands during evaluation

  // Iterate through each element in the postfix expression
  for (int i = 0; i < postfixExpression.size(); i++) {
    if (isalpha(postfixExpression[i][0])) {
      // If the element is an input variable
      int inputIndex = postfixExpression[i][1] - '1'; // Convert input character to index
      bool inputValue = get_inputs()[inputIndex].value; // Get the value of the input
      operandStack.push(inputValue); // Push the input value onto the operand stack
    } else {
      // If the element is an operator
      if (postfixExpression[i] == "~") {
        // Negation operator
        if (!operandStack.empty()) {
          bool operand = operandStack.top();
          operandStack.pop();
          operandStack.push(!operand); // Push the negated operand onto the stack
        } else {
          cout << "Error: Missing operand for negation operator '~'" << endl;
          return false;
        }
      } else {
        // Binary operators: '&' and '|'
        bool operand2 = operandStack.top(); // Get the second operand
        operandStack.pop();
        bool operand1 = operandStack.top(); // Get the first operand
        operandStack.pop();
        if (postfixExpression[i] == "&") {
          operandStack.push(operand1 && operand2); // Perform AND operation and push the result onto the stack
        } else if (postfixExpression[i] == "|") {
          operandStack.push(operand1 || operand2); // Perform OR operation and push the result onto the stack
        }
      }
    }
  }

  return operandStack.top(); // Return the result of the expression evaluation
}


void gate::evaluateTimestamp() {      
  auto maxElement = max_element(inputs.begin(), inputs.end(), data::compare);    // Finds the maximum time stamp among the inputs
  evaluateOutput();    // Evaluates the output of the gate
  setOutputTime(maxElement->timeStamp + delay_ps);    // Sets the output time stamp to the maximum time stamp plus the delay
}
