#include "circ.h"
#include <fstream>

circ::circ() {}

void circ::makeStimMap()
{

  file.open(stimFileName);
  if (file.is_open()) {
    while (getline(file, line)) {

      stringstream ss(line);
      string info;


}

void circ::parse(string circuitFileName, string stimFileName) {
  ifstream file;
  string line;
  <string> infoVec; //vector that stores info on 
  bool checkComponent;


    file.open(circuitFileName);

    for (int i = 0; i < gates.size(); i++) {
      if (file.is_open()) {
        while (getline(file, line)) {
          if (line == "COMPONENTS:") {
            checkComponent = true;
            continue;
          }
          if (checkComponent) {

            stringstream ss(line);
            string word;
            vector<string> words;

            while (getline(ss, word, ',')) // important: modify the word taken so it takes only up to the gate name excluding the number of inputs (AND3 will be AND) have no brain cells at night to do this ~mark
            {
              words.push_back(word);
            }

            gates[i].set_component_name(words[1]);
            gate[i].output(infoVec[2]);

            for (int j = 3; j < words.size(); j++) {
              inputs.push_back(words[i]);
            }
          }
        }
      }

      file.close();
    }

    void circ::makeExpression() { // setting expression of each gate used in the 
                                  // circuit to an expression with 0s and 1s instead of i1,i2,etc...
      ifstream library;
      library.open("library.lib");
      string line, word, gateType;
      bool found = false;
      vector<string> gateInfo;
      for (int i = 0; i < gates.size(); i++) {
        while (getline(library, line) && found == false) {
          gateInfo.clear();
          stringstream ss(line);
          while (getline(ss, word, ',')) {
            gateInfo.push_back(word);
          }
          if (words[0] == gates[i].get_component_name()) {
            found = true;
            gates[i].set_num_of_inputs(gateInfo[1]);
            gates[i].set_output_expression(gateInfo[2]);
            gates[i].set_delay_ps(gateInfo[3]);
          }
        }
        for (int i = 0; i < gates[i].get_num_of_inputs(); i++) {
          string newExpression;
          int index = gates[i].get_output_expression().find((to_string(i))[0]);
          newExpression = gates[i].get_output_expression().replace(
              index - 1, index, inputs[i]);
          gates[i].set_output_expression(newExpression);
        }
      }
    }

  