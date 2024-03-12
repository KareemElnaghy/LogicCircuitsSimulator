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
  vector<gate> gates;
  map<string, bool> stimMap;
public:
  circ();
  void parse(string circuitFileName, string stimFileName);
  void makeExpression();
  void makeStimMap();
};
