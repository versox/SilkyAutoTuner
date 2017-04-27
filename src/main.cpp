#include <iostream>
#include "AutoFile.h"
#include <vector>
#include <fstream>

int main() {
  std::ofstream outStream("output.txt");

  AutoFile af("autofile.txt");
  std::vector<AutoFile::Operation>& commands = af.readFile();

  for(unsigned int i = 0; i < commands.size(); i++) {
		if(commands[i].second) {
			//parallel
      outStream << "AddParallel";
		} else {
			//sequential
      outStream << "AddSequential";
		}
    outStream << "(new " + commands[i].first + ");\n";
	}

  outStream.close();

  return 0;
}
