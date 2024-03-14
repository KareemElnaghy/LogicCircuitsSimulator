#include "circ.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
  string circuitFileName = "circuit1.txt";
  string stimFileName = "stimuli.txt";

  circ circuit1(circuitFileName, stimFileName);
  //circuit1.printGate();
  return 0;
}