#include "circ.h"
#include <cctype>
#include <fstream>
#include <functional>
#include <map>
#include <stack>
#include <vector>

circ::circ() {
  numOfInputsCircuit = 0;
  numOfGates = 0;
  circFileName = "";
  stimFileName = "";
}

void circ::printGate() {
  for (int i = 0; i < numOfGates; i++) {
    cout << gates[i].get_component_name() << endl;
    //cout << gates[i].get_num_of_inputs() << endl;
    gates[i].printInputs();
    cout << "Name: " << gates[i].get_output().name << " Value: " << gates[i].get_output().value << endl<<endl;
  }
}

circ::circ(string circuit, string stim, string lib) {

  circFileName = circuit;   // Set circuit file name
  stimFileName = stim;      // Set stimulus file name
  libFileName = lib;        // Set library file name
  countInputs();            // Count the number of inputs in the circuit
  countGates();             // Count the number of gates in the circuit
  gates.resize(numOfGates); // Resize the gates vector to accommodate gates
  readStim();               // reads stimulus file
  parse();                  // Parse the circuit file to initialize gate objects
  writeSim();               // writes stimuli values to the stim file
  
  for (int i = 0; i < gates.size(); i++) {
    adjustOutputs();
    gates[i].update();
    
  }
  
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
  ifstream file(circFileName); // Open circuit file for reading
  if (!file.is_open()) {
    cout << "Error: Unable to open file."
         << endl; // Error message if file cannot be opened
    return;
  }

  string line;
  int lineCount = 0;
  bool foundComponents = false;

  // Iterate through each line in the file
  while (getline(file, line)) {
    if (foundComponents) {
      lineCount++; // Increment line count after finding COMPONENTS
    } else if (line == "COMPONENTS:") {
      foundComponents = true; // Mark that COMPONENTS is found
    }
  }

  file.close(); // Close the file

  numOfGates = lineCount; // Update the count of gates
}

void circ::readStim() {
  ifstream file;           // Create ifstream object
  file.open(stimFileName); // Open stimulus file for reading

  if (file.is_open()) { // Check if the file is successfully opened
    string line;
    while (getline(file, line)) { // Read each line from the file
      stringstream ss(line);      // Create stringstream object to parse line
      string temp1;               // Temporary variable to store input name
      string temp2;    // Temporary variable to store logic value as string
      bool logicValue; // Variable to store the logic value

      // Read input name and logic value from the line
      getline(ss, temp1, ','); // Read input name
      getline(ss, temp2, ','); // Read logic value as string
      ss >> logicValue;        // Convert logic value string to bool

      stim.push_back(logicValue); // Store logic value into stim vector
    }
    file.close(); // Close the file after reading
  } else {
    cout << "Error: Unable to open stimuli file."
         << endl; // Print error message if file cannot be opened
  }
}

void circ::parse() {
  ifstream file(circFileName); // Open circuit file for reading

  int inputCounter = 0; // Counter for tracking input index

  // Check if the file is successfully opened
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

  // Check if COMPONENTS section is found
  if (!checkComponent) {
    cout << "Error: COMPONENTS section not found." << endl;
    file.close();
    return;
  }

  // Read gate information from the file
  for (int i = 0; i < numOfGates; i++) {
    // Read line containing gate information
    if (!getline(file, line)) {
      cout << "Error: Unable to read gate information from file." << endl;
      break;
    }

    stringstream ss(line);
    string word;
    vector<string> info;

    // Parse the line into words
    while (getline(ss, word, ',')) {
      info.push_back(word);
    }

    // Check if there are enough components to represent a gate
    if (info.size() < 3) {
      cout << "Error: Invalid gate information in file." << endl;
      break;
    }

    // Set gate component name and output
    gates[i].set_component_name(info[1]);
    gates[i].set_output(info[2]);
    gates[i].set_num_of_inputs(info.size() - 3);

    // Set gate inputs
    for (int j = 3; j < info.size(); j++) {
      gates[i].set_inputs(j - 3, info[j], stim[inputCounter]);
      inputCounter++;
    }

    gates[i].set_num_of_inputs(info.size() - 3);
  }

  file.close(); // Close the file
}

// void circ::makeExpression() { // setting expression of each gate used in the
//                               // circuit to an expression with 0s and 1s instead
//                               // of i1,i2,etc..

//   ifstream library;
//   library.open(libFileName);
//   string line, word, gateType;
//   bool found = false;
//   vector<string> gateInfo;

//   for (int i = 0; i < gates.size(); i++) {
//     while (getline(library, line) && found == false) {
//       gateInfo.clear();
//       stringstream ss(line);
//       while (getline(ss, word, ',')) {
//         gateInfo.push_back(word);
//       }
//       if (gateInfo[0] == gates[i].get_component_name()) {
//         found = true;
//         gates[i].set_output_expression(gateInfo[2]);
//         gates[i].set_delay_ps(stoi(gateInfo[3]));
//       }
//     }

//     for (int j = 0; j < gates[j].get_num_of_inputs(); j++) {
//       string newExpression;
//       int index = gates[i].get_output_expression().find((to_string(j))[0]);
//       newExpression = gates[j].get_output_expression().replace(
//           index - 1, index, (gates[j].get_inputs()[j]).name);
//       gates[j].set_output_expression(newExpression);
//     }
//   }
//   library.close();
// }

// void circ::calculateOutput() {
//   stack<char> operators;
//   stack<bool> operands;
//   for (int i = 0; i < gates.size(); i++) {
//     string expression = gates[i].get_output_expression();
//     vector<data> inputs = gates[i].get_inputs();
//     vector<bool> inputValues;
//     for (int i = 0; i < inputs.size(); i++) {
//       inputValues.push_back(inputs[i].value);
//     }

//     for (char c : expression) {
//       if (c == '(' || c == '&' || c == '|') {
//         operators.push(c);
//       } else if (c == ')') {
//         // Evaluate the expression inside the parentheses
//         while (operators.top() != '(') {
//           char op = operators.top();
//           operators.pop();

//           if (op == '&') {
//             bool right = operands.top();
//             operands.pop();
//             bool left = operands.top();
//             operands.pop();
//             operands.push(left && right);
//           } else if (op == '|') {
//             bool right = operands.top();
//             operands.pop();
//             bool left = operands.top();
//             operands.pop();
//             operands.push(left || right);
//           }
//         }
//         operators.pop(); // Remove '('
//       } else if (c == '~') {
//         operators.push(c);
//       } else if (isalnum(c)) {
//         string inputName(1, c);
//         while (isalnum(expression[++c])) {
//           inputName += expression[c];
//         }
//         if (inputValues.find(inputName) != inputs.end()) {
//           operands.push(inputs.at(inputName));
//         } else {
//           cerr << "Undefined input: " << inputName << endl;
//           gates[i].set_output(false);
//         }
//         bool found = false; // replaced with for loop
//         for (int j = 0; j < inputs.size(); j++) {
//           if (inputs[j].name == inputName) {
//             operands.push(inputs[j].value);
//             found = true;
//             break;
//           }
//         }

//         if (!found) {
//           cout << "Undefined input: " << inputName << endl;
//           gates[i].set_output(false);
//         }
//       }
//     }

//     // Evaluate remaining expressions
//     while (!operators.empty()) {
//       char op = operators.top();
//       operators.pop();
//       if (op == '~') {
//         operands.top() = !operands.top();
//       } else if (op == '&') {
//         bool right = operands.top();
//         operands.pop();
//         bool left = operands.top();
//         operands.pop();
//         operands.push(left && right);
//       } else if (op == '|') {
//         bool right = operands.top();
//         operands.pop();
//         bool left = operands.top();
//         operands.pop();
//         operands.push(left || right);
//       }
//     }
//     gates[i].set_output(operands.top()); // cout<<"Test: "<<operands.top();
//   }
// }


void circ::adjustOutputs()
{
  for (int i = 0; i < gates.size(); ++i) {
      gate& currentGate = gates[i]; // Current gate
      // Iterate over the inputs of the current gate
      for (int j = 0; j < currentGate.get_num_of_inputs(); ++j) {
          string inputName = currentGate.get_inputs()[j].name; // Name of the input
          // Iterate over all previous gates to find the output to update current input
          for (int k = 0; k < i; ++k) {
              if (gates[k].get_output().name == inputName) {
                  // Update the input value of the current gate with the output value of the previous gate
                  currentGate.set_inputs(j, inputName, gates[k].get_output().value);
                  break; // Stop searching for the input in previous gates once found
              }
          }
      }
  }
}


void circ::writeSim() {
  fstream file; // File stream object for writing to the simulation file
  string simFileName; // Variable to store the simulation file name

  // Determine the simulation file name based on the circuit file name
  if (circFileName == "circuit1.txt")
    simFileName = "simCircuit1.txt";
  else if (circFileName == "circuit2.txt")
    simFileName = "simCircuit2.txt";
  else if (circFileName == "circuit3.txt")
    simFileName = "simCircuit3.txt";
  else if (circFileName == "circuit4.txt")
    simFileName = "simCircuit4.txt";
  else
    simFileName = "simCircuit5.txt";

  // Open the simulation file for writing
  file.open(simFileName, ios::out);

  // Check if the file was successfully opened
  if (!file.is_open()) {
    cout << "Error creating sim file." << endl; // Display error message if file creation failed
  } else {
    // Iterate through each gate in the circuit
    for (int i = 0; i < numOfGates; i++) {
      // Get the output data of the current gate
      data out = gates[i].get_output();

      // Get the input data of the current gate
      vector<data> in = gates[i].get_inputs();

      // Write the input data of the current gate to the simulation file
      for (int j = 0; j < in.size(); j++) {
        file << in[j].name << ", " << in[j].value << endl; // Write input name and value
      }

      // Write the output data of the current gate to the simulation file
      file << out.name << ", " << out.value << endl; // Write output name and value
    }
  }

  // Close the simulation file
  file.close();
}
