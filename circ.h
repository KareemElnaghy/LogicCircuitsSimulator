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
  vector<bool> stim;
public:
  circ();
  void parse(string circuitFileName, string stimFileName);
  void makeExpression();
  void readStim(string stimFileName);
  void calculateOutput();
};
