
#include <iostream>
#include "script_engine.h"
#include "error.h"

//-----------------------------------------------------------------------------
// Name: main(int argc, char *argv[])
//-----------------------------------------------------------------------------
int main(int argc, char *argv[]) {

	script_engine scripter;
	
	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>"  << std::endl;
		return 1;
	}

	// load the program to execute
	if(scripter.load_program(argv[1])) {
		return 1;
	}
	
	scripter.tokenize();
	scripter.prescan();
	return scripter.start("main");
}
