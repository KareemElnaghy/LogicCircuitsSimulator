#include "gate.h"
using namespace std;

gate::gate() {
  component_name = "";
  num_of_inputs = 0;
  output_expression = "";
  delay_ps = 0;
  
}

void gate::set_inputs(int i, string name, bool value) {
  if(inputs.size()!=num_of_inputs)
  { 
    inputs.resize(num_of_inputs);
  }
  inputs[i].name = name;
  inputs[i].value = value;
}

vector<data> gate::get_inputs() { return inputs; }

void gate::set_output(string name) { output.name = name; }

void gate::set_output(bool val) { output.value = val; }

data gate::get_output() {
  return output;
}

void gate::printInputs()
{
  if(num_of_inputs == 0){
    cout<<"No inputs"<<endl;return;}
  for (int i = 0; i < num_of_inputs; i++)
    {
      cout<<inputs[i].name<<" "<< inputs[i].value <<endl;
    }
}
/*updates the output of any gate based on the inputs given*/
void gate::update() {

  //output = false;
  
  if (get_component_name() == "AND") {

    for (int i = 0; i < num_of_inputs; i++) {
      if (!inputs[i].value) { // if any of the inputs to an AND gate is 0, the output
                        // is 0
        output.value = false;
        break;
      }
    }
    /*OR GATE*/
  } else if (get_component_name() == "OR") {
    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i].value) // if input is 1
      {
        output.value = true;
        break;
      }
    }
    /*NOT GATE*/
  } else if (get_component_name() == "NOT") {
    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i].value) {
        inputs[i].value = 0;
        output.value = false;
      } else {
        inputs[i].value = 1;
        output.value = true;
      }
    }
    /*NAND GATE*/
  } else if (get_component_name() == "NAND") {
    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i].value) // if input is 1
      {
        output.value = false;
        break;
      }
    }
  }
}

void gate::set_component_name(string name) {
  component_name = name;
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
