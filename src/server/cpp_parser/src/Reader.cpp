#include <stdexcept>
#include "Reader.h"

CSVReader::CSVReader(std::string filename, bool firstLineHeader=true) {
	this->filename = filename;
	this->firstLineHeader = firstLineHeader;	
	this->filestream.open(this->getFilename());
	if(!this->filestream.is_open()){ 
	throw std::runtime_error("Runtime Exception: Unable to open file: " + this->getFilename());	
	}

	if(this->firstLineHeader) {
		std::string throwAway;
		getline(this->filestream, throwAway);
	}
}

void CSVReader::tearDown() {
	this->filestream.close();
}

std::string CSVReader::readLine() {
	std::string line;
	if (getline(this->filestream, line)) {
		return line;	
	} 
	return "";
}

