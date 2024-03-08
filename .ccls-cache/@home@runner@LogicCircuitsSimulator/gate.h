#include "circuit.cir"
#include "library.lib"
#include "stimuli.stim"
#include <iostream>
#include <string>
using namespace std;

class gate {
private:
  string component_name; // gate types
  int num_of_inputs;     // number of inputs to every gate
  string output_expression;
  int delay_ps;
  bool output;  // gate output
  bool *inputs; // input pointer to gate, will be pointing to an array of size
                // num_of_inputs

public:
  gate();
  void update(); /*updates the output of any gate based on the inputs given*/
  void set_inputs(int, bool *);
  int get_inputs();
  bool get_output() const;
  string get_component_name();
  void set_component_name(string name);
  int get_num_of_inputs();
  void set_num_of_inputs(int num);
  string get_output_expression();
  void set_output_expression(string expression);
  int get_delay_ps();
  void set_delay_ps(int delay);
};
