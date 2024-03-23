#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

struct data {
  std::string name; // Name of the input
  bool value;       // Value of the input (0 or 1 as true or false)
  int timeStamp;    // Value of the time stamp

  //Comparison operator for data struct
  bool operator==(const data &other) const {
    return (timeStamp == other.timeStamp && name == other.name &&
            value == other.value);
  }

  // Static comparison function used for sorting
  static bool compare(const data &a, const data &b) {
    return a.timeStamp < b.timeStamp;
  }
};

class gate {
private:
  string component_name;           // Name of the gate type
  int num_of_inputs;               // Number of inputs to the gate
  string output_expression;        // Expression representing the gate's output
  int delay_ps;                    // Delay of the gate in picoseconds
  data output;                     // Output data of the gate
  vector<data> inputs;             // Input data of the gate
  map<string, string> expressions; // Map to store gate expressions
  map<char, int> precedence;       // Map to store operator precedence

public:
  gate();                              // Default constructor
  void set_inputs(int i, string name, bool value); // Set input data (name and val) for the gate
  void setInputTime(int i, int time);    // Set input timestamp for the gate
  vector<data> get_inputs();            // Returns the vector of input data
  void setOutputValue(bool out);        // Sets the output value of the gate
  void setOutputName(string name);      // Sets the output name of the gate
  void setOutputTime(int time);         // Set output timestamp for the gate
  data get_output();                    // Returns the output data of the gate
  void set_component_name(string name); // Sets the gate type name
  string get_component_name();          // Returns the gate type name
  void set_num_of_inputs(int num);      // Sets the number of inputs to the gate
  int get_num_of_inputs();             // Returns the number of inputs to the gate
  void set_output_expression(string expression); // Sets the output expression of the gate
  string get_output_expression();       // Returns the output expression of the gate
  void set_delay_ps(int delay);         // Sets the delay of the gate
  void printInputs();                   // Prints the inputs of the gate
  void evaluateOutput();                // Evaluates the output of the gate
  int getOperatorPrecedence(char op);   // Returns the precedence of an operator
  vector<string> convertExpression();   // Converts the gate expression to postfix notation
  bool evaluateExpression(const vector<string> &postfix); // Evaluates the output expression
  void evaluateTimestamp();             // Evaluates the timestamp of the gate
};
