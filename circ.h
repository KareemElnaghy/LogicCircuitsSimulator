#include "gate.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class circ {
private:
  int numOfInputsCircuit;
  int numOfGates;
  string circFileName;
  string stimFileName;
  string libFileName;
  vector<gate> gates;
  vector<bool> stim;

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
  void adjustOutputs();
  void removeSpaces (vector<string>& x);
  void printGate();
};
