#include "circ.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
  string circuit1FileName = "circuit1.cir";
  string circuit2FileName = "circuit2.cir";
  string circuit3FileName = "circuit3.cir";
  string circuit4FileName = "circuit4.cir";
  string circuit5FileName = "circuit5.cir";
  string stimFileName = "stimuli.stim";
  string libFileName = "library.lib";

  circ circuit1(circuit1FileName, stimFileName, libFileName);
  circ circuit2(circuit2FileName, stimFileName, libFileName);
  circ circuit3(circuit3FileName, stimFileName, libFileName);
  circ circuit4(circuit4FileName, stimFileName, libFileName);
  circ circuit5(circuit5FileName, stimFileName, libFileName);

  return 0;
}