#ifndef READER_H
#define READER_H

#include <iostream>
#include <fstream>

class CSVReader {
	private:
		std::string filename;
		bool firstLineHeader;
		std::fstream filestream;
	public:
		CSVReader(std::string filename, bool firstLineHeader);
		void tearDown();
		std::string readLine();

		// Setters and Getters
		void setFilename(std::string filename) { this->filename = filename; }
		std::string getFilename() { return this->filename; }
		void setFirstLineHeader(bool firstLineHeader) { this->firstLineHeader = firstLineHeader; }
		bool getFirstLineHeader(){ return this->firstLineHeader; }
};	

#endif 
