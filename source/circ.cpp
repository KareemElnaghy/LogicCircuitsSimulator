#include "circ.h"


circ::circ() {
  // numOfInputsCircuit = 0;
  numOfGates = 0;
  circFileName = "";
  stimFileName = "";
}

circ::circ(string circuit, string stim, string lib) {

  circFileName = circuit;   // Set circuit file name
  stimFileName = stim;      // Set stimulus file name
  libFileName = lib;        // Set library file name
  countGates();             // Count the number of gates in the circuit
  gates.resize(numOfGates); // Resize the gates vector to accommodate gates
  readStim();               // Read stimulus values from the stimulus file
  readCirc();               // Read circuit from the circuit file
  readExpression();         // Read the output expression from the library file
  writeSim();               // writes stimuli values to the stim file
  // printGates();
}

void circ::printGates() {
  cout << "New Circuit" << endl;
  for (int i = 0; i < numOfGates; i++) {
    cout << gates[i].get_component_name() << endl;
    // cout << gates[i].get_num_of_inputs() << endl;
    gates[i].printInputs();
    cout << "Name: " << gates[i].get_output().name
         << " Value: " << gates[i].get_output().value
         << " Time: " << gates[i].get_output().timeStamp << endl;

    cout << endl;
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

void circ::readCirc() {
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
      gates[i].set_inputs(j - 3, inputName, 0);
    }

    for (int j = 3; j < info.size(); j++) {
      string inputName = info[j];

      gates[i].setInputTime(j - 3, 0);
    }

    gates[i].set_num_of_inputs(info.size() - 3);
  }

  file.close();
}

void circ::adjust() {
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
  vector<data> sim;
  // Determine the simulation file name based on the circuit file name
  if (circFileName == "circuit1.cir")
    simFileName = "simCircuit1.sim";
  else if (circFileName == "circuit2.cir")
    simFileName = "simCircuit2.sim";
  else if (circFileName == "circuit3.cir")
    simFileName = "simCircuit3.sim";
  else if (circFileName == "circuit4.cir")
    simFileName = "simCircuit4.sim";
  else
    simFileName = "simCircuit5.sim";

  // Open the simulation file for writing
  file.open(simFileName, ios::out);

  // Check if the file was successfully opened
  if (!file.is_open()) {
    cout << "Error creating sim file."
         << endl; // Display error message if file creation failed
  } else {
    // Iterate through each gate in the circuit


    for (auto it = stimTime.begin(); it != stimTime.end(); it++) {
      auto it1 = stimTime.find(it->first);
      for (int j = 0; j < gates.size(); j++) {
        data temp;

        temp.name = it->first;
        temp.value = it->second;
        temp.timeStamp = it1->second;

        gates[j].updateInputs(temp);
        adjust();
        calculateAllTimestamps();
      }

      for (int i = 0; i < gates.size(); i++) {
        data temp1, temp2;
        for (int j = 0; j < gates[i].get_num_of_inputs(); j++) {
          temp2.value = gates[i].get_inputs()[j].value;
          temp2.name = gates[i].get_inputs()[j].name;
          temp2.timeStamp = gates[i].get_inputs()[j].timeStamp;
          sim.push_back(temp2);
        }
        temp1.value = gates[i].get_output().value;
        temp1.name = gates[i].get_output().name;
        temp1.timeStamp = gates[i].get_output().timeStamp;
        sim.push_back(temp1);
        // Sort the vector
        sort(sim.begin(), sim.end(), data::compare);

        removeDuplicates(sim);
        sort(sim.begin(), sim.end(), data::compare);
      }
      for (int k = 0; k < sim.size(); k++) {
        file << sim[k].timeStamp << ", " << sim[k].name << ", " << sim[k].value
             << endl;
      }
      sim.clear();

      file << "=============" << endl;
    }

    // Close the simulation file
    file.close();
  }
}

void circ::readExpression() {
  ifstream file;
  file.open(libFileName);
  string line, word;
  vector<string> info;
  map<string, int> gateDelay;

  // Read each line from the file
  while (getline(file, line)) {
    info.clear();
    stringstream ss(
        line); // Initialize stringstream to parse the line into words
    while (getline(ss, word, ',')) {
      info.push_back(word); // Store each word in the vector
    }
    removeSpaces(info); // Remove any spaces from the words
    // Store gate expressions and delays in respective maps
    for (int i = 0; i < info.size(); i++) {
      gateExpression[info[0]] = info[2];
      gateDelay[info[0]] = stoi(info[3]);
    }
  }

  // Set output expressions and delays for each gate
  for (int i = 0; i < gates.size(); i++) {
    string name = gates[i].get_component_name();
    auto it = gateExpression.find(name);
    if (it != gateExpression.end())
      gates[i].set_output_expression(it->second);
    auto it1 = gateDelay.find(name);
    if (it1 != gateDelay.end())
      gates[i].set_delay_ps(it1->second);
  }

  file.close(); // Close the file
}

void circ::calculateAllTimestamps() {
  // Iterate through all gates in the circuit
  for (int i = 0; i < gates.size(); i++) {
    // Calculate timestamp for the current gate
    gates[i].evaluateTimestamp();
    // Adjust gates in the circuit based on the new timestamps
    adjust();
  }
}

void circ::removeDuplicates(vector<data> &x) {
  // Iterate through the vector of data
  for (int i = 0; i < x.size(); i++) {
    // Compare each element with subsequent elements to find duplicates
    for (int j = i + 1; j < x.size(); j++) {
      // If a duplicate is found, remove it from the vector
      if (x[i] == x[j])
        x.erase(x.begin() + j);
    }
  }
}
