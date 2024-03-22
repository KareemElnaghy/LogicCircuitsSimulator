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
         << " Value: " << gates[i].get_output().value
         << " Time: " << gates[i].get_output().timeStamp << endl;

    // cout << gates[i].get_output_expression() << endl;
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
 // cout << "Test 1" << endl;
  readStim();
 // cout << "Test 2" << endl;
  // reads stimulus file
  parse();
 // cout << "Test 3" << endl; // Parse the circuit file to initialize gate objects
  readExpression();
 // cout << "Test 4" << endl;
  evaluateOutput();
  //cout << "Test 5" << endl;

  calculateDelay();
 // cout << "Test 6" << endl;
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

  file.close();
}

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
          currentGate.setInputTime(j, gates[k].get_output().timeStamp);
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
    // data out = gates[gates.size() - 1].get_output();
    for (int i = 0; i < numOfGates; i++) {
      // Get the output data of the current gate
      data out = gates[i].get_output();

      // Get the input data of the current gate
      vector<data> in = gates[i].get_inputs();

      // Write the input data of the current gate to the simulation file
      for (int j = 0; j < in.size(); j++) {
        file << in[j].timeStamp << ", " << in[j].name << ", " << in[j].value
             << endl; // Write input name and value
      }

      // Write the output data of the current gate to the simulation file
      for (int j = 0; j < gates[i].get_gate_change_time().size(); j++) {
        file << gates[i].get_gate_change_time()[j] << ", " << out.name << ", "
             << out.value << endl; // Write input name and value
      }
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
  map<string, int> gateDelay;

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
    gateDelay[info[0]] = stoi(info[3]);
  }

  // for(auto it = gateExpression.begin(); it != gateExpression.end(); it++)
  //   cout<<it->first<<" "<<it->second<<endl;

  for (int i = 0; i < gates.size(); i++) {
    string name = gates[i].get_component_name();
    auto it = gateExpression.find(name);
    if (it != gateExpression.end())
      gates[i].set_output_expression(it->second);
    auto it1 = gateDelay.find(name);
    if (it1 != gateDelay.end())
      gates[i].set_delay_ps(it1->second);
  }

  file.close();
}

void circ::evaluateOutput() {
  for (int i = 0; i < gates.size(); i++) {
    vector<string> temp = gates[i].convertInfixToPostfix();
    // for (int i = 0; i < temp.size(); i++)
    //   cout << "Temp " << i << " " << temp[i] << endl;
    bool val = gates[i].evaluatePostfixExpression(temp);
    // cout<<"TEST: "<<val<<endl;
    gates[i].setOutputValue(val);
    adjustInputs();
  }
}

void circ::calculateDelay() {

  vector<int> tempTimes;

  for (int i = 0; i < gates.size(); i++) {
    if (i > 0) {
      for (int k = i - 1; k >= 0; k--)
        gates[i].updateInputs(gates[k].get_output());
    }

    tempTimes.resize(gates[i].get_inputs().size());
    for (int j = 0; j < gates[i].get_inputs().size(); j++) {

      tempTimes[j] = gates[i].get_inputs()[j].timeStamp;

    }              // copying the time stamps of the inputs into a temp vector
    int index = 0; // represents the index of the gate_change_time vector
    while (!tempTimes.empty()) {
      auto minIter = min_element(tempTimes.begin(), tempTimes.end());
      if (index > 0 && (gates[i].get_inputs()[index].timeStamp -
           gates[i].get_inputs()[index - 1].timeStamp) <
              gates[i].get_delay_ps() &&
          (gates[i].get_inputs()[index].timeStamp -
           gates[i].get_inputs()[index - 1].timeStamp) != 0) {
        gates[i].setOutputTime(gates[i].get_gate_change_time()[index - 1] +
                               gates[i].get_delay_ps());
        
        gates[i].set_gate_change_time(
            index, gates[i].get_gate_change_time()[index - 1] +
                       gates[i].get_delay_ps());
      } else {
        int newTime = (*minIter) + gates[i].get_delay_ps();
        gates[i].setOutputTime(newTime);
        ;
        if (index > 0 && (gates[i].get_gate_change_time()[index - 1]) >=
                             (gates[i].get_gate_change_time()[index])) {
          gates[i].set_gate_change_time(
              index, gates[i].get_gate_change_time()[index - 1]);
          gates[i].setOutputTime(gates[i].get_gate_change_time()[index - 1]);
        } else
          gates[i].set_gate_change_time(index, newTime);
      }
      tempTimes.erase(minIter);

      index++; // looping over the inputs to find minimum time when an input was
               // changed and setting the gate_change_time vector at the
               // corresponding index and looping over until all inputs are
               // considered
    }
  } // this will result in each gate having a vector of gate_change_time values
    // that represent the times when the gate will change its output ordered
    // ascendingly
}
