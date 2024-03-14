#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct data{
  string name;
  bool value;
};

class gate {
private:
  string component_name; // gate types
  int num_of_inputs;     // number of inputs to every gate
  string output_expression;
  int delay_ps;
  data output;  // gate output
  vector<data> inputs; // input pointer to gate, will be pointing to an array of size
  

public:
  gate();
  void update(); /*updates the output of any gate based on the inputs given*/
  void set_inputs(int i, string name, bool value);
  vector<data> get_inputs();
  void set_output(bool out);
  void set_output(string name);
  void get_output() const;
  string get_component_name();
  void set_component_name(string name);
  int get_num_of_inputs();
  void set_num_of_inputs(int num);
  string get_output_expression();
  void set_output_expression(string expression);
  int get_delay_ps();
  void set_delay_ps(int delay);
  void printInputs();
};
