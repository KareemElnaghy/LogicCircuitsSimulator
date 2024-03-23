#include "circ.h"
#include <fstream>
#include <functional>
#include <map>
#include <stack>

circ::circ() {
  numOfInputsCircuit = 0;
  numOfGates = 0;
  circFileName = "";
  stimFileName = "";
}

void circ::printGate(gate g) {
  cout << g.get_component_name() << endl;
  g.get_output();

  cout<<g.get_num_of_inputs()<<endl;
 
  g.printInputs();
}

circ::circ(string circuit, string stim) {

  circFileName = circuit;
  stimFileName = stim;
  countInputs();
  countGates();
  gates.resize(numOfGates);
  parse();
  for (int i = 0; i < numOfGates; i++)
    printGate(gates[i]);
  cout << endl;
}

void circ::countInputs() {
  ifstream file(circFileName);
  if (!file.is_open()) {
    cout << "Error: Unable to open file." << endl;
    return;
  }

  string line;
  int lineCount = 0;
  bool foundInputs = false;

  while (getline(file, line)) {

    if (line == "COMPONENTS:") {
      break; // Stop reading when COMPONENTS is found
    }

    if (foundInputs) {
      lineCount++; // Increment line count after finding INPUTS
    } else if (line == "INPUTS:") {
      foundInputs = true; // Mark that INPUTS is found
    }
  }

  file.close();

  numOfInputsCircuit = lineCount;
}

void circ::countGates() {
  ifstream file(circFileName);
  if (!file.is_open()) {
    cout << "Error: Unable to open file." << endl;
    return;
  }

  string line;
  int lineCount = 0;
  bool foundComponents = false;

  while (getline(file, line)) {
    if (foundComponents) {
      lineCount++; // Increment line count after finding COMPONENTS
    } else if (line == "COMPONENTS:") {
      foundComponents = true; // Mark that COMPONENTS is found
    }
  }

  file.close();

  numOfGates =
      lineCount; // Assuming numOfGates is a member variable of your class
}

void circ::readStim() {
  ifstream file;
  file.open(stimFileName);
  // opens stim file

  if (file.is_open()) { // reads stim file
    string line;
    while (getline(file, line)) {
      stringstream ss(line);
      string temp1; // temp variables to store first info
      string temp2;
      bool logicValue; // variable that will store the logic value

      // Read input name and logic value from the line
      getline(ss, temp1, ',');
      getline(ss, temp2, ',');
      ss >> logicValue; // assigns logic value to variable

      stim.push_back(logicValue); // stores logic value into stim vector
    }
    file.close();
  }
  else
  {
    cout << "Error: Unable to open stimuli file." << endl;
  }
}

void circ::parse() {
  ifstream file(circFileName);
  readStim();
  int inputCounter = 0;
  
  
  if (!file.is_open()) {
    cout << "Error: Unable to open circuit file." << endl;
    return;
  }

  string line;
  bool checkComponent = false;

  // Skip lines until COMPONENTS is found
  while (getline(file, line)) {
    if (line == "COMPONENTS:") {
      checkComponent = true;
      break;
    }
  }

  if (!checkComponent) {
    cout << "Error: COMPONENTS section not found." << endl;
    file.close();
    return;
  }

  for (int i = 0; i < numOfGates; i++) {
    // Read line containing gate information
    if (!getline(file, line)) {
      cout << "Error: Unable to read gate information from file." << endl;
      break;
    }

    stringstream ss(line);
    string word;
    vector<string> words;

    // Parse the line into words
    while (getline(ss, word, ',')) {
      words.push_back(word);
    }

    // Check if there are enough components to represent a gate
    if (words.size() < 3) {
      cout << "Error: Invalid gate information in file." << endl;
      break;
    }

    // Set gate component name and output
    gates[i].set_component_name(words[1]);
    gates[i].set_output(words[2]);
    gates[i].set_num_of_inputs(words.size() - 3);
    
    // // Set gate inputs
    for (int j = 3; j < words.size(); j++) {
      gates[i].set_inputs(j - 3, words[j], stim[inputCounter]);
      inputCounter++;
    }

    gates[i].set_num_of_inputs(words.size() - 3);
  }

  file.close();
}

void circ::makeExpression() { // setting expression of each gate used in the
                              // circuit to an expression with 0s and 1s instead
                              // of i1,i2,etc...
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
        //gates[i].set_num_of_inputs(stoi(gateInfo[1]));
        gates[i].set_output_expression(gateInfo[2]);
        gates[i].set_delay_ps(stoi(gateInfo[3]));
      }
    }
    for (int i = 0; i < gates[i].get_num_of_inputs(); i++) {
      string newExpression;
      int index = gates[i].get_output_expression().find((to_string(i))[0]);
      newExpression = gates[i].get_output_expression().replace(
          index - 1, index, (gates[i].get_inputs()[i]).name);
      gates[i].set_output_expression(newExpression);
    }
  }
}

void circ::calculateOutput() {
  stack<char> operators;
  stack<bool> operands;
  for (int i = 0; i < gates.size(); i++) {
    string expression = gates[i].get_output_expression();
    vector<data> inputs = gates[i].get_inputs();
    vector<bool> inputValues;
    for (int i = 0; i < inputs.size(); i++) {
      inputValues.push_back(inputs[i].value);
    }

    for (char c : expression) {
      if (c == '(' || c == '&' || c == '|') {
        operators.push(c);
      } else if (c == ')') {
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
          } else if (op == '|') {
            bool right = operands.top();
            operands.pop();
            bool left = operands.top();
            operands.pop();
            operands.push(left || right);
          }
        }
        operators.pop(); // Remove '('
      } else if (c == '~') {
        operators.push(c);
      } else if (isalnum(c)) {
        string inputName(1, c);
        while (isalnum(expression[++c])) {
          inputName += expression[c];
        }
        // if (inputValues.find(inputName) != inputs.end()) {
        //   operands.push(inputs.at(inputName));
        // } else {
        //   cerr << "Undefined input: " << inputName << endl;
        //   gates[i].set_output(false);
        // }
        bool found = false; // replaced with for loop
        for (int j = 0; j < inputs.size(); j++) {
          if (inputs[j].name == inputName) {
            operands.push(inputs[j].value);
            found = true;
            break;
          }
        }

        if (!found) {
          cout << "Undefined input: " << inputName << endl;
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
      } else if (op == '&') {
        bool right = operands.top();
        operands.pop();
        bool left = operands.top();
        operands.pop();
        operands.push(left && right);
      } else if (op == '|') {
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

void circ::writeSim() //function to write the output of the circuit to a file
{
  fstream file; // stimuli.txt
  file.open("simCircuit.txt",ios::out);
  if (!file.is_open()) {
    cout << "Error creating sim file." << endl;
  } else {
    for(int i=0; i<numOfGates; i++)
    {
      data out = gates[i].get_output();
      vector <data> in = gates[i].get_inputs();
      for(int j = 0; j<in.size(); j++)
        {
    file<<in[j].name<<","<<in[j].value<<endl;
        }
      file<<out.name<<","<<out.value<<endl;

    }
  }
  
  
  file.close();
  
}