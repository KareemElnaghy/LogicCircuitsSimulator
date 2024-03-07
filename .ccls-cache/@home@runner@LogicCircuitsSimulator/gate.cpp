#include "gate.h"
#include <iostream>
using namespace std;

gate::gate() {
  component_name = "";
  num_of_inputs = 0;
  output_expression = "";
  delay_ps = 0.0;
  output = false;
}

bool gate::get_output() const { return output; }

// CHANGE LATER -- IMPLEMEMTATION FOR AND GATE only
void gate::update() {
  output = false;

  if (get_component_name() == "AND") {

    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i] ==
          0) { // if any of the inputs to an AND gate is 0, the output is 0
        output = false;
        break;
      }
    }


  }
}
