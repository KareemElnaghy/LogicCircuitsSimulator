#include "circ.h"

circ::circ() {
  numOfInputsCircuit = 0;
  numOfGates = 0;
  circFileName = "";
  stimFileName = "";
}

void circ::printGate() {
  cout << "Circuit" << endl;
  for (int i = 0; i < numOfGates; i++) {
    cout << gates[i].get_component_name() << endl;
    // cout << gates[i].get_num_of_inputs() << endl;
    gates[i].printInputs();
    cout << "Name: " << gates[i].get_output().name
         << " Value: " << gates[i].get_output().value << endl;

    cout<<gates[i].get_output_expression()<<endl;
  }
}

void circ::removeSpaces(vector<string> &x) {
  for (int i = 0; i < x.size(); i++) {
    string str = "";
    bool hasSpaces = false; // variable to track if there are spaces in the word
    for (char c : x[i]) {
      if (c != ' ') {
        str += c;
      } else {
        hasSpaces = true; // Sets varaible to true if there are spaces
      }
    }
    // If there were spaces in the word, update it in the vector
    if (hasSpaces) {
      x[i] = str;
    }
  }
}

void circ::removeSpacesMap() {
  vector<string> inNames;
  for (auto it = stimVal.begin(); it != stimVal.end(); it++) {
    inNames.push_back(it->first);
  }

  removeSpaces(inNames);

  auto it1 = stimVal.begin();
  // Iterate over both the map and the vector simultaneously
  for (const auto &newName : inNames) {
    // If there are still elements in the map
    if (it1 != stimVal.end()) {
      // Insert a new element with the new key and the same value
      stimVal[newName] = it1->second;

      // Erase the old element
      stimVal.erase(it1++);
    } else {
      // If there are no more elements in the map, break the loop
      break;
    }
  }

  auto it2 = stimTime.begin();
  for (const auto &newName : inNames) {
    // If there are still elements in the map
    if (it2 != stimTime.end()) {
      // Insert a new element with the new key and the same value
      stimTime[newName] = it2->second;

      // Erase the old element
      stimTime.erase(it2++);
    } else {
      // If there are no more elements in the map, break the loop
      break;
    }
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

  for (int i = 0; i < gates.size(); i++) {
    gates[i].update();
    adjustInputs();
  }
  readExpression();

  writeSim(); // writes stimuli values to the stim file
  printGate();
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
      string time;                // Temporary variable to store input name
      string name;     // Temporary variable to store logic value as string
      bool logicValue; // Variable to store the logic value

      // Read input name and logic value from the line
      getline(ss, time, ','); // Read input name
      getline(ss, name, ','); // Read logic value as string
      ss >> logicValue;       // Convert logic value string to bool

      stimVal[name] = logicValue;
      stimTime[name] = stoi(time);
    }

    removeSpacesMap();

    file.close(); // Close the file after reading
  } else {
    cout << "Error: Unable to open stimuli file."
         << endl; // Print error message if file cannot be opened
  }
}

void circ::parse() {
  ifstream file(circFileName); // Open circuit file for reading
  // int inputCounter = 0; // Counter for tracking input index

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

    removeSpaces(info);

    // Set gate component name and output
    gates[i].set_component_name(info[1]);
    gates[i].setOutputName(info[2]);
    gates[i].set_num_of_inputs(info.size() - 3);

    // Set gate inputs
    for (int j = 3; j < info.size(); j++) {
      string inputName = info[j];
      if (stimVal.find(inputName) != stimVal.end()) {
        gates[i].set_inputs(j - 3, inputName, stimVal[inputName]);
      } else {
        gates[i].set_inputs(j - 3, inputName, 0);
      }
    }

    for (int j = 3; j < info.size(); j++) {
      string inputName = info[j];
      if (stimTime.find(inputName) != stimTime.end()) {
        gates[i].setInputTime(j - 3, stimTime[inputName]);
      } else {
      }
    }

    gates[i].set_num_of_inputs(info.size() - 3);
  }

  file.close(); // Close the file
}

// void circ::makeExpression() { // setting expression of each gate used in the
//                               // circuit to an expression with 0s and 1s
//                               instead
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

void circ::adjustInputs() {
  for (int i = 0; i < gates.size(); ++i) {
    gate &currentGate = gates[i]; // Current gate
    // Iterate over the inputs of the current gate
    for (int j = 0; j < currentGate.get_num_of_inputs(); ++j) {
      string inputName = currentGate.get_inputs()[j].name; // Name of the input
      // Iterate over all previous gates to find the output to update current
      // input
      for (int k = 0; k < i; ++k) {
        if (gates[k].get_output().name == inputName) {
          // Update the input value of the current gate with the output value of
          // the previous gate
          currentGate.set_inputs(j, inputName, gates[k].get_output().value);
          break; // Stop searching for the input in previous gates once found
        }
      }
    }
  }
}

void circ::writeSim() {
  fstream file;       // File stream object for writing to the simulation file
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
    cout << "Error creating sim file."
         << endl; // Display error message if file creation failed
  } else {
    // Iterate through each gate in the circuit
    for (int i = 0; i < numOfGates; i++) {
      // Get the output data of the current gate
      data out = gates[i].get_output();

      // Get the input data of the current gate
      vector<data> in = gates[i].get_inputs();

      // Write the input data of the current gate to the simulation file
      for (int j = 0; j < in.size(); j++) {
        file << in[j].timeStamp << "," << in[j].name << ", " << in[j].value
             << endl; // Write input name and value
      } 
      
      //we need to consider that each output will be output onto sim multiple times for different input times!!!

      // Write the output data of the current gate to the simulation file
      file << out.timeStamp << "," << out.name << ", " << out.value
           << endl; // Write output name and value
    }
  }

  // Close the simulation file
  file.close();
}

void circ::readExpression() {
    ifstream file;
    file.open(libFileName);
    string line, word;
    vector<string> info;

    while (getline(file, line)) {
        // Parse the line into words
        info.clear();
        stringstream ss(line); // Move stringstream initialization here
        while (getline(ss, word, ',')) {
            info.push_back(word);
  
        }
        removeSpaces(info);
        for (int i = 0; i < info.size(); i++)
        gateExpression[info[0]] = info[2];
    }

  // for(auto it = gateExpression.begin(); it != gateExpression.end(); it++)
  //   cout<<it->first<<" "<<it->second<<endl;
  
  for (int i = 0; i < gates.size(); i++) {
      string name = gates[i].get_component_name();
      auto it = gateExpression.find(name);
      if (it != gateExpression.end())
          gates[i].set_output_expression(it->second);
   
  }

  
    file.close();
}

// bool circ::evaluatePostfix(const string &expression) {
//   stack<bool> operands;
//   unordered_map<char, function<bool(bool, bool)>> operators = {
//       {'&', [](bool a, bool b) { return a && b; }},
//       {'|', [](bool a, bool b) { return a || b; }},
//       {'~', [](bool a, bool) { return !a; }}};

//   for (char c : expression) {
//     if (isalpha(c)) { // Operand
//       // Convert character to index and push the corresponding input onto the
//       // stack
//       int index = c - 'i';
//       operands.push(inputs[index]);
//     } else if (c == '~') { // Unary operator
//       bool operand = operands.top();
//       operands.pop();
//       operands.push(operators[c](operand, false));
//     } else { // Binary operator
//       bool operand2 = operands.top();
//       operands.pop();
//       bool operand1 = operands.top();
//       operands.pop();
//       operands.push(operators[c](operand1, operand2));
//     }
//   }
//   return operands.top();
// }

void circ::calculateDelay() { // still need to call it before outputting into sim file
  vector<int> tempTimes; 
  for (int i = 0; i < gates.size(); i++) {
    for (int j = 0; j < gates[i].get_inputs().size(); j++) {
      tempTimes[j] = gates[i].get_inputs()[j].timeStamp;
    } //copying the time stamps of the inputs into a temp vector
    int index = 0; //represents the index of the gate_change_time vector
    while (!tempTimes.empty()) {
      auto minIter = min_element(tempTimes.begin(), tempTimes.end());
      gates[i].set_gate_change_time(index,
                                    (*minIter) + gates[i].get_delay_ps());
      tempTimes.erase(minIter);
      index++; //looping over the inputs to find minimum time when an input was changed and setting the gate_change_time vector at the corresponding index and looping over until all inputs are considered
    }
  } //this will result in each gate having a vector of gate_change_time values that represent the times when the gate will change its output ordered ascendingly 
}
