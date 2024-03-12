#include "circ.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

circ::circ(){
  
}

void circ::parse(string fileName)
{
  ifstream file;
  string line;
  file.open(fileName);

  if(file.is_open())
  {
    while(getline(file, line))
      {
        stringstream ss(line);
        string word;
        vector<string> words;

        while(getline(ss, word, ','))
          {
            words.push_back(word);
          }
      }
  }

 
  
  file.close();
  
  
}
void circ::calculateOutput(){
  ifstream library;
  library.open("library.lib");
  string line, expression, gateType;
  bool found=false;
  for (int i=0;i<gates.size();i++){
    while (getline(library,line) && found==false){
      stringstream ss(line);
      getline(ss,gateType,',');
      if (gateType==gates[i].get_component_name()){
        found=true;
        getline(ss,expression);
      }
    }
  }
}