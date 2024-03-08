#include "gate.h"
#include <iostream>
#include <string>
using namespace std;

gate::gate() {
  component_name = "";
  num_of_inputs = 0;
  output_expression = "";
  delay_ps = 0;
  output = false;
  inputs = nullptr;
}

void gate::set_inputs(int size, bool *inputs) {
  num_of_inputs = size;
  this->inputs = &inputs[num_of_inputs];
}

void gate::get_inputs() {
  for (int i = 0; i < num_of_inputs; i++) {
    cout << "Input " << i + 1 << " = " << inputs[i] << endl;
  }
}

bool gate::get_output() const { return output; }

/*updates the output of any gate based on the inputs given*/
void gate::update() {

  output = false;

  if (get_component_name() == "AND") {

    for (int i = 0; i < num_of_inputs; i++) {
      if (!inputs[i]) { // if any of the inputs to an AND gate is 0, the output
                        // is 0
        output = false;
        break;
      }
    }
  } else if (get_component_name() == "OR") {
    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i]) // if input is 1
      {
        output = true;
        break;
      }
    }
  } else if (get_component_name() == "NOT") {
    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i]) {
        inputs[i] = 0;
        output = false;
      } else {
        inputs[i] = 1;
        output = true;
      }
    }
  } else if (get_component_name() == "NAND") {
    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i]) // if input is 1
      {
        output = false;
        break;
      }
    }
  }
}

void gate::set_component_name(string component_name) {
  this->component_name = component_name;
}

string gate::get_component_name() { return component_name; }

void gate::set_num_of_inputs(int num_of_inputs) {
  this->num_of_inputs = num_of_inputs;
}

int gate::get_num_of_inputs() { return num_of_inputs; }

void gate::set_output_expression(string output_expression) {
  this->output_expression = output_expression;
}

string gate::get_output_expression() { return output_expression; }

void gate::set_delay_ps(int delay_ps) { this->delay_ps = delay_ps; }

int gate::get_delay_ps() { return delay_ps; }
