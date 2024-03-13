#include "circ.h"
#include <fstream>
#include <stack>
#include <map>
#include <functional>

circ::circ() {}

void circ::readStim(string stimFileName)
{
ifstream file;
  file.open(stimFileName);    //opens stim file
  int i = 0;
  if (file.is_open()) {    //reads stim file
      string line;
      while (getline(file, line)) {
          stringstream ss(line);
          string temp1;//temp variables to store first info
          string temp2;
          bool logicValue;  //variable that will store the logic value

          // Read input name and logic value from the line
          getline(ss, temp1, ',');
          getline(ss, temp2, ',');
          
          ss >> logicValue;   //assigns logic value to variable

          
          stim[i] = logicValue;    //stores logic value into stim vector
        i++;
      }
      file.close();


}}

void circ::parse(string circuitFileName, string stimFileName) {
  ifstream file;
  string line;
  bool checkComponent;


    file.open(circuitFileName);  //opens circuit file

    for (int i = 0; i < gates.size(); i++) {
      if (file.is_open()) {
        while (getline(file, line)) {  //iterate until we get 
          if (line == "COMPONENTS:") {
            checkComponent = true;
            continue;
          }
          if (checkComponent) {

            stringstream ss(line);
            string word;
            vector<string> words;

            while (getline(ss, word, ',')) // important: modify the word taken so it takes only up to the gate name excluding the number of inputs (AND3 will be AND) have no brain cells at night to do this ~mark
            {
              words.push_back(word);
            }

            gates[i].set_component_name(words[1]);
            gates[i].set_output(words[2]);
            for (int j = 3; j < words.size(); j++) {
              gates[i].set_inputs(j - 3, words[j], stim[i]);
            }
          }
        }
      }

      file.close();
    }
}

  void circ::makeExpression() { // setting expression of each gate used in the 
                                // circuit to an expression with 0s and 1s instead of 
                                // i1,i2,etc...
    ifstream library;
    library.open("library.lib");
    string line, word, gateType;
    bool found = false;
    vector<string> gateInfo;
    for (int i = 0; i < gates.size(); i++) {
      while (getline(library, line) && found == false) {
        gateInfo.clear();
        stringstream ss(line);
        while (getline(ss, word, ',')) {
          gateInfo.push_back(word);
        }
        if (gateInfo[0] == gates[i].get_component_name()) {
          found = true;
          gates[i].set_num_of_inputs(stoi(gateInfo[1]));
          gates[i].set_output_expression(gateInfo[2]);
          gates[i].set_delay_ps(stoi(gateInfo[3]));
        }
      }
      for (int i = 0; i < gates[i].get_num_of_inputs(); i++) {
        string newExpression;
        int index = gates[i].get_output_expression().find((to_string(i))[0]);
        newExpression = gates[i].get_output_expression().replace(index - 1, index, (gates[i].get_inputs()[i]).name);
        gates[i].set_output_expression(newExpression);
      }
    }
  }

  void circ::calculateOutput() {
    stack<char> operators;
    stack<bool> operands;
    for (int i=0;i<gates.size();i++){
      string expression = gates[i].get_output_expression();
      vector<data> inputs=gates[i].get_inputs();
      vector<bool> inputValues;
      for (int i=0;i<inputs.size();i++){
        inputValues.push_back(inputs[i].value);
      }
    
      for (char c : expression) {
        if (c == '(' || c == '&' || c == '|') {
          operators.push(c);
        } 
        else if (c == ')') {
          // Evaluate the expression inside the parentheses
          while (operators.top() != '(') {
            char op = operators.top();
            operators.pop();
  
            if (op == '&') {
              bool right = operands.top();
              operands.pop();
              bool left = operands.top();
              operands.pop();
              operands.push(left && right);
            } 
            else if (op == '|') {
              bool right = operands.top();
              operands.pop();
              bool left = operands.top();
              operands.pop();
              operands.push(left || right);
            }
          }
          operators.pop(); // Remove '('
        } 
        else if (c == '~') {
          operators.push(c);
        } 
        else if (isalnum(c)) {
          string inputName(1, c);
          while (isalnum(expression[++c])) {
            inputName += expression[c];
          }
          if (inputValues.find(inputName) != inputs.end()) {
            operands.push(inputs.at(inputName));
          } else {
            cerr << "Undefined input: " << inputName << endl;
            gates[i].set_output(false);
          }
        }
      }
    
      // Evaluate remaining expressions
      while (!operators.empty()) {
        char op = operators.top();
        operators.pop();
        if (op == '~') {
          operands.top() = !operands.top();
        } 
        else if (op == '&') {
          bool right = operands.top();
          operands.pop();
          bool left = operands.top();
          operands.pop();
          operands.push(left && right);
        } 
        else if (op == '|') {
          bool right = operands.top();
          operands.pop();
          bool left = operands.top();
          operands.pop();
          operands.push(left || right);
        }
      }
    gates[i].set_output(operands.top());
  }
}
