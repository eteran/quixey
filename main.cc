
#include <iostream>
#include "script_engine.h"
#include "error.h"

//-----------------------------------------------------------------------------
// Name: main(int argc, char *argv[])
//-----------------------------------------------------------------------------
int main(int argc, char *argv[]) {

	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>"  << std::endl;
		return 1;
	}

	try {
		script_engine scripter;
	
		// load the program to execute
		if(scripter.load_program(argv[1])) {
			return 1;
		}
		
		return scripter.start("main");
	} catch(const error &e) {
		std::cerr << "-------------------------------" << std::endl;
		std::cerr << "An Error Occurred: " << e.what() << std::endl;
		if(e.line_number >= 0) {
			std::cerr << "On Line Number: " << e.line_number << std::endl;
			std::cerr << "In File: " << e.filename << std::endl;
		}
		std::cerr << "-------------------------------" << std::endl;
	}
}
