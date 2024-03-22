#include "gate.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <stack>
#include <algorithm>
using namespace std;

class circ {
private:
  int numOfInputsCircuit;
  int numOfGates;
  string circFileName;
  string stimFileName;
  string libFileName;
  vector<gate> gates;
  map<string, bool> stimVal;
  map<string, int> stimTime;
  map<string, string> gateExpression;

public:
  circ();
  circ(string circuit, string stim, string lib);
  void parse();
  void makeExpression();
  void readStim();
  void writeSim();
  void countInputs();
  void countGates();
  void calculateOutput();
  void adjustInputs();
  void removeSpaces(vector<string>& x);
  void removeSpacesMap();
  void printGate();
  void evaluateOutput();
  void readExpression();
  void calculateDelay();
};
