#include "gate.h"
#include <fstream>
#include <map>
#include <sstream>
#include <stack>
#include <unordered_map>
using namespace std;

gate::gate() {
  component_name = "";
  num_of_inputs = 0;
  output_expression = "";
  delay_ps = 0;
  output.value = false;
  output.timeStamp = 0;
  output.name = "";
}

void gate::set_inputs(int i, string name, bool value) {
  if (inputs.size() != num_of_inputs) {
    inputs.resize(num_of_inputs);
  }
  inputs[i].name = name;
  inputs[i].value = value;
}

void gate::setInputTime(int i, int time) {
  if (inputs.size() != num_of_inputs) {
    inputs.resize(num_of_inputs);
  }

  inputs[i].timeStamp = time;
}

vector<data> gate::get_inputs() { return inputs; }

void gate::setOutputName(string name) { output.name = name; }

void gate::setOutputValue(bool val) { output.value = val; }

void gate::setOutputTime(int time)
{
  output.timeStamp = time;
}

data gate::get_output() { return output; }

void gate::printInputs() {
  if (num_of_inputs == 0) {
    cout << "No inputs" << endl;
    return;
  }
  for (int i = 0; i < num_of_inputs; i++) {
    cout << inputs[i].name << " " << inputs[i].value << " "
         << inputs[i].timeStamp << endl;
  }
}



// void gate::update() { /*updates the output of any gate based on the inputs
//                          given*/

//   if (get_component_name() == "AND2") {

//     for (int i = 0; i < num_of_inputs; i++) {
//       if (!inputs[i].value) { // if any of the inputs to an AND gate is 0, the
//                               // output is 0
//         output.value = false;

//       } else {
//         output.value = true;
//       }
//     }
//     /*OR GATE*/
//   } else if (get_component_name() == "OR2") {
//     for (int i = 0; i < num_of_inputs; i++) {
//       if (inputs[i].value) // if input is 1
//       {
//         output.value = true;
//       }
//     }
//     /*NOT GATE*/
//   } else if (get_component_name() == "NOT") {
//     for (int i = 0; i < num_of_inputs; i++) {
//       if (inputs[i].value) {
//         output.value = false;
//       } else {
//         output.value = true;
//       }
//     }
//     /*NAND GATE*/
//   } else if (get_component_name() == "NAND2" ||
//              get_component_name() == "NAND3") {

//     for (int i = 0; i < num_of_inputs; i++) {
//       if (inputs[i].value) // if input is 1
//       {

//         output.value = false;

//       } else {
//         output.value = true;
//       }
//     }
//   } else if (get_component_name() == "NOR3") {
//     output.value = true; // Assume output is true initially
//     for (int i = 0; i < num_of_inputs; i++) {
//       if (inputs[i].value) { // if any input is true, output is false
//         output.value = false;
//         break;
//       }
//     }
//     output.value = true;
//   } else if (get_component_name() == "XOR3") {
//     int trueCount = 0;
//     for (int i = 0; i < num_of_inputs; i++) {
//       if (inputs[i].value) { // Count the number of true inputs
//         trueCount++;
//       }
//     }
//     // XOR gate outputs true if the number of 1s is odd
//     output.value = (trueCount % 2 == 1);
//   }
// }

void gate::updateInputs(data out) {
  for (int i = 0; i < num_of_inputs; i++) {
    if (inputs[i].name == out.name) {
      inputs[i].value = out.value;
      inputs[i].timeStamp=out.timeStamp;
    }
  }
}

void gate::set_component_name(string name) { component_name = name; }

string gate::get_component_name() { return component_name; }

void gate::set_num_of_inputs(int num_of_inputs) {
  this->num_of_inputs = num_of_inputs;
}

int gate::get_num_of_inputs() { return num_of_inputs; }

void gate::set_output_expression(string output_expression) {
  this->output_expression = output_expression;
}

string gate::get_output_expression() { return output_expression; }

void gate::set_delay_ps(int delay_ps) { this->delay_ps = delay_ps; }

int gate::get_delay_ps() { return delay_ps; }

vector<int> gate::get_gate_change_time() { return gate_change_time; }

void gate::set_gate_change_time(int i, int time) {
  gate_change_time.resize(inputs.size());
  gate_change_time[i] = time;
}

int gate::getOperatorPrecedence(char op) {
  // Function to get precedence of operators
  if (op == '~')
    return 3;
  if (op == '&')
    return 2;
  if (op == '|')
    return 1;
  return 0;
}

string gate::convertCharToString(char c) {
  // Function to convert character to string
  return string(1, c);
}

vector<string> gate::convertInfixToPostfix() {
    // Function to convert infix expression to postfix expression
    stack<char> operatorStack;
    vector<string> postfixExpression;
    string temp;

    for (char character : output_expression) {
        // Loop through each character in the expression
        if (isalpha(character) || isdigit(character)) {
            // If the character is an operand, append it to the temporary string
            temp.push_back(character);
        }
        else {
            if (!temp.empty()) {
                // If there's a non-empty temporary string, push it to the postfix expression vector
                postfixExpression.push_back(temp);
                temp.clear();
            }
            if (character == '(') {
                // If character is '(', push it onto the operator stack
                operatorStack.push(character);
            }
            else if (character == ')') {
                // If character is ')', pop operators from the stack until '(' is encountered
                while (!operatorStack.empty() && operatorStack.top() != '(') {
                    postfixExpression.push_back(convertCharToString(operatorStack.top()));
                    operatorStack.pop();
                }
                operatorStack.pop(); // Discard '('
            }
            else { // Operator
                // If character is an operator, pop operators with higher or equal precedence from the stack and push them to the postfix expression
                while (!operatorStack.empty() && getOperatorPrecedence(character) <= getOperatorPrecedence(operatorStack.top())) {
                    postfixExpression.push_back(convertCharToString(operatorStack.top()));
                    operatorStack.pop();
                }
                operatorStack.push(character);
            }
        }
    }

    if (!temp.empty()) {
        // If there's a non-empty temporary string left, push it to the postfix expression vector
        postfixExpression.push_back(temp);
        temp.clear();
    }

    while (!operatorStack.empty()) {
        // Pop remaining operators from the stack and push them to the postfix expression
        postfixExpression.push_back(convertCharToString(operatorStack.top()));
        operatorStack.pop();
    }

    return postfixExpression;
}

bool gate::evaluatePostfixExpression(const vector<string> &postfixExpression) {
  stack<bool> operandStack;

  for (int i = 0; i < postfixExpression.size(); i++) {
    // If token is an input variable
    if (isalpha(postfixExpression[i][0])) {
      // Convert input character to index
      int inputIndex = postfixExpression[i][1] - '1';

      bool inputValue = get_inputs()[inputIndex].value;

      operandStack.push(inputValue);
    } else {
      // If token is an operator
      if (postfixExpression[i] == "~") {
        // Negation operator
        if (!operandStack.empty()) {
          bool operand = operandStack.top();
          operandStack.pop();
          operandStack.push(!operand);
        } else {
          cerr << "Error: Missing operand for negation operator '~'" << endl;
          return false;
        }
      } else {
        // Binary operators: '&' and '|'

        bool operand2 = operandStack.top();
        operandStack.pop();
        bool operand1 = operandStack.top();
        operandStack.pop();
        if (postfixExpression[i] == "&") {
          operandStack.push(operand1 && operand2);
        } else if (postfixExpression[i] == "|") {
          operandStack.push(operand1 || operand2);
        }
      }
    }
  }
  // Store the evaluated output in the gate's output data

  return operandStack.top();
}
