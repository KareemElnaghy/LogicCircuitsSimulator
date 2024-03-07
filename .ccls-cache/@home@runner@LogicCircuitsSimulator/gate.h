#include <iostream>
using namespace std;

class gate{ // 
private:
string component_name;
int num_of_inputs;
string output_expression;
int delay_ps;
bool output;

public:
gate();
string get_component_name();
void set_component_name(string name);
int get_num_of_inputs();
void set_num_of_inputs(int num);
string get_output_expression();
void set_output_expression(string expression);
int get_delay_ps();
void set_delay_ps(int delay);
bool get_output() const;
void update();
};


