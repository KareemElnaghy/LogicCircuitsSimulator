#include "gate.h"
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <stack>
using namespace std;

gate::gate() {
  component_name = "";
  num_of_inputs = 0;
  output_expression = "";
  delay_ps = 0;
  output.value = false;
  output.timeStamp = 0;
  output.name = "";
  
}

void gate::set_inputs(int i, string name, bool value) {
  if(inputs.size()!=num_of_inputs)
  { 
    inputs.resize(num_of_inputs);
  }
  inputs[i].name = name;
  inputs[i].value = value;
}

void gate::setInputTime(int i, int time)
{
  if(inputs.size()!=num_of_inputs)
  { 
    inputs.resize(num_of_inputs);
  }

  inputs[i].timeStamp = time;
}

vector<data> gate::get_inputs() { return inputs; }

void gate::setOutputName(string name) { output.name = name; }

void gate::setOutputValue(bool val) { output.value = val; }

data gate::get_output() {
  return output;
}

void gate::printInputs()
{
  if(num_of_inputs == 0){
    cout<<"No inputs"<<endl;return;}
  for (int i = 0; i < num_of_inputs; i++)
    {
      cout<<inputs[i].name<<" "<< inputs[i].value <<" "<<inputs[i].timeStamp<<endl;
    }
}

void gate::update() {    /*updates the output of any gate based on the inputs given*/
   
  
  if (get_component_name() == "AND2") {

    for (int i = 0; i < num_of_inputs; i++) {
      if (!inputs[i].value) { // if any of the inputs to an AND gate is 0, the output
                        // is 0
        output.value = false;
        
      }
      else
      {
        output.value = true;
      }
    }
    /*OR GATE*/
  } else if (get_component_name() == "OR2") {
    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i].value) // if input is 1
      {
        output.value = true;
      
      }
    }
    /*NOT GATE*/
  } else if (get_component_name() == "NOT") {
    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i].value) {
        output.value = false;
      } else {
        output.value = true;
      }
    }
    /*NAND GATE*/
  } else if (get_component_name() == "NAND2" || get_component_name() == "NAND3") {

    for (int i = 0; i < num_of_inputs; i++) {
      if (inputs[i].value) // if input is 1
      {
        
        output.value = false;

      }
      else{
        output.value = true;
      }
    }
  } else if (get_component_name() == "NOR3") {
      output.value = true; // Assume output is true initially
      for (int i = 0; i < num_of_inputs; i++) {
          if (inputs[i].value) { // if any input is true, output is false
              output.value = false;
              break;
          }
      } output.value = true;
  } else if (get_component_name() == "XOR3") {
      int trueCount = 0;
      for (int i = 0; i < num_of_inputs; i++) {
          if (inputs[i].value) { // Count the number of true inputs
              trueCount++;
          }
      }
      // XOR gate outputs true if the number of 1 is odd
      output.value = (trueCount % 2 == 1);
  }

}

void gate::updateInputs(data out)
{
  for(int i =0; i<num_of_inputs;i++)
  {
    if(inputs[i].name == out.name)
    {
      inputs[i].value = out.value;
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

vector<int> gate::get_gate_change_time() { return gate_change_time; }

void gate::set_gate_change_time(int i, int time) {gate_change_time[i]=time;}

// void gate::readExpression(string filename)
// {
//   ifstream file;
//   file.open(filename);
  
// string line;
//   while (std::getline(file, line)) {
//   std::stringstream ss(line);
//   std::string gateType, numInputsStr, expression, delayStr;
//     getline(ss, gateType, ',');
//   if (gateType == component_name)
//   {
//       std::getline(ss, numInputsStr, ',');
//       std::getline(ss, expression, ',');
//       std::getline(ss, delayStr, ',' ); 
//       // int numInputs = std::stoi(numInputsStr);
//      int delay = std::stoi(delayStr);
//       //g.set_component_name(gateType);
//      // g.set_num_of_inputs(numInputs);
//       set_output_expression(expression); cout<<expression<<endl;
//       set_delay_ps(delay);
//       //gates.push_back(g);
//   }}

  
//   file.close();
// }


