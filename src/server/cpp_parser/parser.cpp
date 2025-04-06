#include "parser.h"
#include <iostream>

void intAttribute(std::vector<int>& recordValueVector, const std::string& attribute) {
	{
		int missingInt = -1;
		try {
			int converted = std::stoi(attribute);
			recordValueVector.push_back(converted);
		} catch(const std::invalid_argument& e) {
			recordValueVector.push_back(missingInt);
		}
	}
}

void dblAttribute(std::vector<double>& recordValueVector, const std::string& attribute) {
	{
		double missingDbl = -1.0;
		try {
			double recordValue = std::stod(attribute);
			recordValueVector.push_back(recordValue);
		} catch(const std::invalid_argument& e) {
			recordValueVector.push_back(missingDbl);
		}
	}
}

std::vector<std::string> parseRecord(std::string recordString) {
	std::vector<std::string> recordAttributes;
	std::string attribute = "";

	int startIndex = 0;
	bool quotationFlag = false;
	for (int currentIndex = 0; currentIndex < recordString.length(); currentIndex++) {
		//End of Quotation
		if (recordString[currentIndex] == '"' && quotationFlag) {
			quotationFlag = false;
		//Start of Quotation
		} else if (recordString[currentIndex] == '"') {
			quotationFlag = true;
		//Comma Encounter
		} else if (!quotationFlag && recordString[currentIndex] == ',') {
			attribute = recordString.substr(startIndex, currentIndex - startIndex);
			recordAttributes.push_back(attribute);
			startIndex = currentIndex + 1;
		}
	}
	//last Attribute
	recordAttributes.push_back(recordString.substr(startIndex, recordString.length() - startIndex + 1));
	return recordAttributes;

};


MotorRecordVector loadRecord(std::vector<std::vector<std::string>> lines) {
	MotorRecordVector records; 
	size_t totalColumns = 29;
	for (int j = 0; j < totalColumns; j++) {
		for (int i = 0; i < lines.size(); i++){
			switch(j) {
				case 0:
					records.crashDate.push_back(lines[i][j]);
					break;
				case 1:
					records.crashTime.push_back(lines[i][j]);
					break;
				case 2:
					records.borough.push_back(lines[i][j]);
					break;
				case 3:
					intAttribute(records.zipCode, lines[i][j]);
					break;
				case 4:
					dblAttribute(records.latitude, lines[i][j]);
					break;
				case 5:
					dblAttribute(records.longitude, lines[i][j]);
					break;
				case 6:
					records.location.push_back(lines[i][j]);
					break;
				case 7:
					records.onStreeName.push_back(lines[i][j]);
					break;
				case 8:
					records.crossStreetName.push_back(lines[i][j]);
					break;
				case 9:
					records.offStreetName.push_back(lines[i][j]);
					break;
				case 10:
					intAttribute(records.numberOfPersonInjured, lines[i][j]);
					break;
				case 11:
					intAttribute(records.numberOfPersonKilled, lines[i][j]);
					break;
				case 12:
					intAttribute(records.numberOfPedestrianInjured, lines[i][j]);
					break;
				case 13:
					intAttribute(records.numberOfPedestrianKilled, lines[i][j]);
					break;
				case 14:
					intAttribute(records.numberOfCyclistInjured, lines[i][j]);
					break;
				case 15:
					intAttribute(records.numberOfCyclistKilled, lines[i][j]);
					break;
				case 16:
					intAttribute(records.numberOfMotoristInjured, lines[i][j]);
					break;
				case 17:
					records.contributingFactorVehicle1.push_back(lines[i][j]);
					break;
				case 18:
					records.contributingFactorVehicle1.push_back(lines[i][j]);
					break;
				case 19:
					records.contributingFactorVehicle2.push_back(lines[i][j]);
					break;
				case 20:
					records.contributingFactorVehicle3.push_back(lines[i][j]);
					break;
				case 21:
					records.contributingFactorVehicle4.push_back(lines[i][j]);
					break;
				case 22:
					records.contributingFactorVehicle5.push_back(lines[i][j]);
					break;
				case 23:
					intAttribute(records.collisonId, lines[i][j]);
					break;
				case 24:
					records.vehicleTypeCode1.push_back(lines[i][j]);
					break;
				case 25:
					records.vehicleTypeCode2.push_back(lines[i][j]);
					break;
				case 26:
					records.vehicleTypeCode3.push_back(lines[i][j]);
					break;
				case 27:
					records.vehicleTypeCode4.push_back(lines[i][j]);
					break;
				case 28:
					records.vehicleTypeCode5.push_back(lines[i][j]);
					break;
			}
		}
	}
	return records;
}
