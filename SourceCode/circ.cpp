#include "circ.h"

void removeSpacesString(std::string &str) {
    // Iterate through each character in the string
    for (size_t i = 0; i < str.length(); ++i) {
        // If the character is a space, erase it
        if (str[i] == ' ') {
            str.erase(i, 1);
            // Decrement the index to adjust for the erased character
            --i;
        }
    }
}

  string convertToSimFileName(string cirFileName) {
    // Find the position of the last occurrence of '.' in the file name
    size_t dotPos = cirFileName.find_last_of('.');

    // If '.' is found and it is not the first character, replace the extension
    if (dotPos != string::npos && dotPos != 0) {
        return cirFileName.substr(0, dotPos) + ".sim";
    } else {
        // If '.' is not found or it is the first character, simply append ".sim"
        return cirFileName + ".sim";
    }
}

vector<string> circ::getCompNames()
{
  vector<string> compNames;
  ifstream file;
  file.open(libFileName);
  string line;
  vector<string> info;

  // Read each line from the file
  for(int i =0; i< countComponentsLib(); i++) {
    getline(file, line);
    info.clear();
    stringstream ss(line); // Initialize stringstream to parse the line into words
    string word;
    while (getline(ss, word, ',')) {
      info.push_back(word); // Store each word in the vector
    }
    removeSpaces(info);

    if(info.size() != 4)
    {
       cout << "Error: "<< libFileName<<" Format Unsupported"<<endl; 
      exit(EXIT_SUCCESS);
    }
    compNames.push_back(info[0]);

}
  file.close();
  return compNames;
}

circ::circ() {
  numOfInputsCircuit = 0;
  numOfGates = 0;
  circFileName = "";
  stimFileName = "";
  simFileName = "";
}

circ::circ(string circuit, string stim, string lib) {

  circFileName = circuit;   // Set circuit file name
  stimFileName = stim;      // Set stimulus file name
  libFileName = lib;        // Set library file name
  simFileName = convertToSimFileName(circFileName); // Set simulation file name
  countGates();             // Count the number of gates in the circuit
  countInputs();            // Count the number of inputs in the circuit
  gates.resize(numOfGates); // Resize the gates vector to accommodate gates
  readStim();               // Read stimulus values from the stimulus file
  readCirc();               // Read circuit from the circuit file
  readExpression();         // Read the output expression from the library file
  calculateInitialOutputs();
  //printGates();
  queueSim();               // writes stimuli values to the stim file

}

void circ::printGates() {
  cout << "New Circuit" << endl;
  for (int i = 0; i < numOfGates; i++) {
    cout << gates[i].get_component_name() << endl;
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


void circ::countInputs() {
  ifstream file(circFileName);
  if (!file.is_open()) {
    cout << "Error: Unable to open "<< circFileName<< endl;
     exit(EXIT_SUCCESS);
  }

  string line;
  int lineCount = 0;
  bool foundInputs = false;

  while (getline(file, line)) {

    if (line == "COMPONENTS:") {
      break; // Stop reading when COMPONENTS is found
    }

    if (foundInputs) {
      circuitInputs.push_back(line);
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
    cout << "Error: Unable to open "<< circFileName<< endl;
         // Error message if file cannot be opened
     exit(EXIT_SUCCESS);
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

int circ::countComponentsLib() {
  ifstream file(libFileName); // Open circuit file for reading

  if (!file.is_open()) {
    cout << "Error: Unable to open "<< circFileName<< endl;
         // Error message if file cannot be opened
     exit(EXIT_SUCCESS);
  }

  string line;
  int lineCount = 0;

  // Iterate through each line in the file
  while (getline(file, line)) {
   lineCount++;
  }

  file.close(); // Close the file

  return lineCount;
}

void circ::readStim() {
  ifstream file;           // Create ifstream object
 
  file.open(stimFileName); // Open stimulus file for reading

  if (file.is_open()) { // Check if the file is successfully opened
    string line;
    while (getline(file, line)) { // Read each line from the file
       gate::dataStruct ev;
      stringstream ss(line);      // Create stringstream object to parse line
      string time;                // Temporary variable to store input name
      string name;     // Temporary variable to store logic value as string
      bool logicValue; // Variable to store the logic value
      gate::dataStruct
          temp; // Temporary data structure to store input name and logic

      // Read input name and logic value from the line
      getline(ss, time, ','); // Read input name
      getline(ss, name, ','); // Read logic value as string
      ss >> logicValue;       // Convert logic value string to bool


      removeSpacesString(name);

      ev.name = name;
      ev.value = logicValue;
      ev.timeStamp = stoi(time);

 

      pqueue.push(ev);
      simulate.push(ev);
    }


    file.close(); // Close the file after reading
  } else {
    cout << "Error: Unable to open "<< stimFileName<< endl;
          // Print error message if file cannot be opened
  }
}

void circ::readCirc() {
  // Open circuit file for reading
  fstream file(circFileName);

  // Vector to store the names of inputs/outputs that have been read
  vector<string> inputsOutputRead;
  // Get component names from library
  vector<string> compNames = getCompNames();

  // Check if the file is successfully opened
  if (!file.is_open()) {
    cout << "Error: Unable to open " << circFileName << endl;
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
      exit(EXIT_SUCCESS);
    }

    stringstream ss(line);
    string word;
    vector<string> info;

    // Parse the line into words
    while (getline(ss, word, ',')) {
      info.push_back(word);
    }

    // Remove spaces from parsed information
    removeSpaces(info);

    // Set gate component name and output
    string outputName = info[2];
    string componentName = info[1];
    bool compExists = false;

    // Check if the output is already used as an input
    for (int l = 0; l < inputsOutputRead.size(); l++) {
      if (inputsOutputRead[l] == outputName) {
        cout << "Error: Output " << outputName << " is an input to another gate" << endl;
        exit(EXIT_SUCCESS);
      }
    }

    // Check if the component exists in the library
    for (int l = 0; l < compNames.size(); l++) {
      if (compNames[l] == componentName) {
        compExists = true;
        break;
      }
    }

    // If the component doesn't exist, show an error
    if (!compExists) {
      cout << "Error: The component " << componentName << " cannot be found in " << libFileName << endl;
      exit(EXIT_SUCCESS);
    }

    // Set gate properties
    gates[i].set_component_name(info[1]);
    gates[i].setOutputName(outputName);
    gates[i].set_num_of_inputs(info.size() - 3);

    inputsOutputRead.push_back(outputName);

    // Set gate inputs
    for (int j = 3; j < info.size(); j++) {
      bool found = false;
      bool error = false;
      string inputName = info[j];
      // Check if the input is also an output of another gate
      for (int k = 0; k < circuitInputs.size(); k++) {
        if (inputName == outputName) {
          error = true;
          break;
        } else if (inputName == circuitInputs[k]) {
          found = true;
          gates[i].set_inputs(j - 3, inputName, 0);
          gates[i].setInputTime(j - 3, 0);
          inputsOutputRead.push_back(inputName);
          break;
        }
      }

      // Show an error if the input is both an input and output
      if (error) {
        cout << "Error: Input " << inputName << " is both an Input and Output to a gate" << endl;
        exit(EXIT_SUCCESS);
      }

      // Check if the input is already read as an output
      if (!found) {
        for (int k = 0; k < inputsOutputRead.size(); k++) {
          if (inputsOutputRead[k] == inputName) {
            found = true;
            gates[i].set_inputs(j - 3, inputName, 0);
            gates[i].setInputTime(j - 3, 0);
            break;
          }
        }
      }

      // Show an error if the input is not found
      if (!found) {
        cout << "Error: Unable to find input name " << inputName << "." << endl;
        exit(EXIT_SUCCESS);
      }
    }
    gates[i].set_num_of_inputs(info.size() - 3);
  }

  file.close();
}





priority_queue<gate::dataStruct> circ::adjust() {

  priority_queue<gate::dataStruct> pq;
  for (int i = 0; i < gates.size(); ++i) {
    gate &currentGate = gates[i]; // Current gate
    // Iterate over the inputs of the current gate
    for (int j = 0; j < currentGate.get_num_of_inputs(); ++j) {
      string inputName = currentGate.get_inputs()[j].name; // Name of the input
      // Iterate over all previous gates to find the output to update current
      // input
      for (int k = 0; k < i; ++k) {
        if (gates[k].get_output().name == inputName) {
          gate::dataStruct event;
          gate::dataStruct data;
          // Update the input value of the current gate with the output value of
          // the previous gate
          event.name = inputName;
          event.value = gates[k].get_output().value;
          event.timeStamp = gates[k].get_output().timeStamp;
         

          currentGate.set_inputs(j, inputName, gates[k].get_output().value);
          currentGate.setInputTime(j, gates[k].get_output().timeStamp);
          break; // Stop searching for the input in previous gates once found
        }
      }
    }
  }
  return pq;
}

void circ::queueSim() {
  // Loop until the priority queue is empty

  bool fileError = false;
  
  while (!pqueue.empty()) {
    // Get the top event from the priority queue
    gate::dataStruct event = pqueue.top();
    pqueue.pop();

    // Iterate over all gates
    for (int i = 0; i < gates.size(); i++) {
      // Iterate over all inputs of the gate
      for (int j = 0; j < gates[i].get_inputs().size(); j++) {
        // Check if the input matches the event
        if (gates[i].get_inputs()[j].name == event.name) {
          // Store the previous output value
          bool prevResult = gates[i].get_output().value;
          // Set the input value of the gate
          gates[i].setInputValue(j, event.value);
          // Evaluate the output of the gate
          gates[i].evaluateOutput();

          // Check if the output value has changed
          if (gates[i].get_output().value != prevResult) {
            // Create a new event with the output value
            gate::dataStruct temp;
            temp.value = gates[i].get_output().value;
            temp.name = gates[i].get_output().name;
            // Calculate the timestamp for the event
            temp.timeStamp = event.timeStamp + gates[i].get_delay_ps();
            // Set the output time of the gate
            gates[i].setOutputTime(temp.timeStamp);
            // Adjust priority queue based on new event
            priority_queue<gate::dataStruct> pq = adjust();
            // Push the event to the simulation queue and priority queue
            simulate.push(temp);
            pqueue.push(temp);
            // Push events from adjusted priority queue to simulation queue
            while (!pq.empty()) {
              gate::dataStruct ev = pq.top();
              pq.pop();
              simulate.push(ev);
            }
          }
        }
      }
    }
  }

  // Write simulation results to file
  ofstream file;
  file.open(simFileName);
  if (!file.is_open()) {
    cout << "Error: Failed to open file " << simFileName << " for writing." << endl;
    fileError = true; 
  } else {
    while (!simulate.empty() && !fileError) {
      gate::dataStruct event = simulate.top();
      simulate.pop();
      file << event.timeStamp << ", " << event.name << ", " << event.value << endl;
      if (file.fail()) {
        cout << "Error: Failed to write to file " << simFileName << endl;
        fileError = true; 
      }
    }
    if(!fileError)
    {
      cout<<"Simulation results have been written to "<<simFileName<<endl;
    }
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

void circ::calculateInitialOutputs() {
  // Iterate through all gates in the circuit
  for (int i = 0; i < gates.size(); i++) {
    // Calculate timestamp for the current gate
    gates[i].evaluateOutput();
    // Adjust gates in the circuit based on the new timestamps
    adjust();
  }
}





