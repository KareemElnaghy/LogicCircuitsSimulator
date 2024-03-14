#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct data{
  string name; // Name of the input
  bool value;  // Value of the input (0 or 1)
};

class gate {
private:
  string component_name;    // Name of the gate type
  int num_of_inputs;        // Number of inputs to the gate
  string output_expression; // Expression representing the gate's output
  int delay_ps;             // Delay of the gate in picoseconds
  data output;              // Output data of the gate
  vector<data> inputs;      // Input data of the gate



map<string,string> expressions;

public:
  gate(); // Default constructor
  void update(); // Updates the output of the gate based on the inputs
  void set_inputs(int i, string name, bool value); // Sets input data for the gate
  void set_input(int value);
  vector<data> get_inputs(); // Returns the vector of input data
  void set_output(bool out); // Sets the output value of the gate
  void set_output(string name); // Sets the output name of the gate
  data get_output(); // Returns the output data of the gate
  void set_component_name(string name); // Sets the gate type name
  string get_component_name(); // Returns the gate type name
  void set_num_of_inputs(int num); // Sets the number of inputs to the gate
  int get_num_of_inputs(); // Returns the number of inputs to the gate
  void set_output_expression(string expression); // Sets the output expression of the gate
  string get_output_expression(); // Returns the output expression of the gate
  void set_delay_ps(int delay); // Sets the delay of the gate
  int get_delay_ps(); // Returns the delay of the gate
  void printInputs(); // Prints the inputs of the gate

  // void readExpression(string);
  void updateInputs(data out);
}; 

