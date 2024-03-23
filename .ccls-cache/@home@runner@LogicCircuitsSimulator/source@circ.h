#include "gate.h"
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class circ {
private:
  int numOfGates;  // Total number of gates in the circuit
  string circFileName;  // Name of the circuit file
  string stimFileName;    // Name of the stimuli file
  string libFileName;    // Name of the library file
  vector<gate> gates;    // Vector of gates in the circuit
  map<string, bool> stimVal;    // Map to store stim values
  map<string, int> stimTime;    // Map to store stim timestamps
  map<string, string> gateExpression;    // Map to store gate expressions

public:
  circ();    // Default constructor
  circ(string circuit, string stim, string lib);    // Constructor with circuit, stimuli, and library file names
  void countGates();    // Counts the number of gates in the circuit
  void readStim();    // Reads the stimuli file and stores the values
  void readCirc();    // Parses the circuit file and stores the gates
  void readExpression();  // Reads the gate expressions from the library file
  void adjust();   // Adjusts and updates gates inputs and outputs everytime there is a change
  void calculateAllTimestamps();  // Calculates the timestamps of all gates
  void writeSim();    // Writes the simulation output to a file
  void removeSpaces(vector<string> &x);    // Removes spaces from a vector of strings
  void removeSpacesMap();    // Removes spaces from a map of strings
  void removeDuplicates(vector<data> &x);    // Removes duplicate data from a vector of data
  void printGates();

};
