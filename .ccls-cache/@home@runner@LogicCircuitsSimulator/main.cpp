#include "circ.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
  string circuit1FileName = "circuit1.txt";
   string circuit2FileName = "circuit2.txt";
   string circuit3FileName = "circuit3.txt";
   string circuit4FileName = "circuit4.txt";
   string circuit5FileName = "circuit5.txt";
  string stimFileName = "stimuli.txt"; 
  string libFileName = "library.txt";

  circ circuit1(circuit1FileName, stimFileName, libFileName);
  circ circuit2(circuit2FileName, stimFileName, libFileName);
  circ circuit3(circuit3FileName, stimFileName, libFileName);
  circ circuit4(circuit4FileName, stimFileName, libFileName);
  circ circuit5(circuit5FileName, stimFileName, libFileName);
  

  

  return 0;
}