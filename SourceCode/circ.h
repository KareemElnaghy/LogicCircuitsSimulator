
#include <cstdlib>
#include "gate.h"
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class circ {
private:
  int numOfGates;             // Total number of gates in the circuit
  int numOfInputsCircuit;     // Number of inputs in the circuit
  string circFileName;        // Name of the circuit file
  string stimFileName;        // Name of the stimuli file
  string libFileName;         // Name of the library file
  string simFileName;         // Name of the simulation file
  vector<gate> gates;         // Vector of gates in the circuit
  priority_queue <gate::dataStruct> pqueue;  // Priority queue to store input events from stim file
  priority_queue <gate::dataStruct> simulate;  // Priority queue to store all the events that will eventually be outputed in the simulation file
  vector <string> circuitInputs;        // Vector to store the input names of the circuit
  map<string, string> gateExpression; // Map to store gate expressions

public:
  circ(); // Default constructor
  circ(string circuit, string stim,string lib); // Constructor with circuit, stimuli, and library file names
  void countGates();     // Counts the number of gates in the circuit
  void countInputs();    // Counts the number of inputs in the circuit
  int countComponentsLib(); // Counts the number of components in the library
  void readStim();       // Reads the stimuli file and stores the values
  void readCirc();       // Parses the circuit file and stores the gates
  void readExpression(); // Reads the gate expressions from the library file
  priority_queue<gate::dataStruct> adjust(); // Adjusts and updates gates inputs and outputs everytime there is a change
  void calculateInitialOutputs(); // Calculates the timestamps of all gates
  void queueSim();               // Writes the simulation output to a file
  void removeSpaces(vector<string> &x); // Removes spaces from a vector of strings                          
  void printGates();        // Prints the gates in the circuit
  vector<string> getCompNames();    //  Returns a vector of component names
};

