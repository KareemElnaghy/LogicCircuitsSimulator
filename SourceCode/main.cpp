#include "circ.h"
#include <iostream>
#include <string>
using namespace std;

void simulateLogicCircuit(const string &libraryFile, const string &circuitFile,
                          const string &stimuliFile) {

  cout << "Simulating logic circuit with the following files:" << endl;
  cout << "Library file: " << libraryFile << endl;
  cout << "Circuit file: " << circuitFile << endl;
  cout << "Stimuli file: " << stimuliFile << endl;

  circ circuit(circuitFile, stimuliFile, libraryFile);

  //cout<<"Circuit has been simulated in simFile.sim"<<endl;
}

int main(int argc, char *argv[]) {

  if (argc != 4) {
    cout << "Use this Format: logicsim <library file> <circuit file> <stimuli file>"<< endl;
    return 1; 
  }

  string libraryFile = argv[1];
  string circuitFile = argv[2];
  string stimuliFile = argv[3];

  // Call the function to simulate the logic circuit
  simulateLogicCircuit(libraryFile, circuitFile, stimuliFile);

  return 0; 
}