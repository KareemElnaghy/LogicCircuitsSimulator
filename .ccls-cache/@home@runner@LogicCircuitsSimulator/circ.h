#include "gate.h"
#include <iostream>
#include <vector>
using namespace std;

class circ {
private:
  vector<gate> gates;

public:
  circ();
  void parse(string fileName);
  void calculateOutput();
};
