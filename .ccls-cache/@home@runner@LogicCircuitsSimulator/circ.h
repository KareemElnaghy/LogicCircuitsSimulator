#include "gate.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

class circ {
private:
  int numOfInputs;
  int numOfGates;
  string circFileName;
  string stimFileName;
  vector<gate> gates;
  vector<bool> stim;

public:
  circ();
  circ(string circuit, string stim);
  void parse();
  void makeExpression();
  void readStim();
  void countInputs();
  void countGates();
  void calculateOutput();
  void printGate(gate);
};
