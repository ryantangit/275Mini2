#include "RecordVectors.h"


std::vector<std::string> RecordVectors::parseRecord(std::string recordString) {
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
}

void intAttribute(std::vector<int> recordValueVector, std::string attribute) {
	int missingInt = -1;
	try {
		int converted = std::stoi(attribute);
		recordValueVector.push_back(converted);
	} catch(const std::invalid_argument& e) {
		recordValueVector.push_back(missingInt);
	}
}

void dblAttribute(std::vector<double> recordValueVector, std::string attribute) {
	double missingDbl = -1.0;
	try {
		double recordValue = std::stod(attribute);
		recordValueVector.push_back(recordValue);
	} catch(const std::invalid_argument& e) {
		recordValueVector.push_back(missingDbl);
	}
}

void RecordVectors::addRecord(std::vector<std::string> recordAttributes) {
	MotorRecordVector motorRecordVector;
	motorRecordVector.crashDate.push_back(recordAttributes[0]);
	motorRecordVector.crashTime.push_back(recordAttributes[1]);
	motorRecordVector.borough.push_back(recordAttributes[2]);
	intAttribute(motorRecordVector.zipCode, recordAttributes[3]);	
	dblAttribute(motorRecordVector.latitude, recordAttributes[4]);
	dblAttribute(motorRecordVector.longitude, recordAttributes[5]);
	motorRecordVector.location.push_back(recordAttributes[6]);
	motorRecordVector.onStreeName.push_back(recordAttributes[7]);
	motorRecordVector.crossStreetName.push_back(recordAttributes[8]);
	motorRecordVector.offStreetName.push_back(recordAttributes[9]);
	intAttribute(motorRecordVector.numberOfPersonInjured, recordAttributes[10]);
	intAttribute(motorRecordVector.numberOfPersonKilled, recordAttributes[11]);
	intAttribute(motorRecordVector.numberOfPedestrianInjured, recordAttributes[12]);
	intAttribute(motorRecordVector.numberOfPedestrianKilled, recordAttributes[13]);
	intAttribute(motorRecordVector.numberOfCyclistInjured, recordAttributes[14]);
	intAttribute(motorRecordVector.numberOfCyclistKilled, recordAttributes[15]);
	intAttribute(motorRecordVector.numberOfMotoristInjured, recordAttributes[16]);
	intAttribute(motorRecordVector.numberOfMotoristKilled, recordAttributes[17]);
	motorRecordVector.contributingFactorVehicle1.push_back(recordAttributes[18]);
	motorRecordVector.contributingFactorVehicle2.push_back(recordAttributes[19]);
	motorRecordVector.contributingFactorVehicle3.push_back(recordAttributes[20]);
	motorRecordVector.contributingFactorVehicle4.push_back(recordAttributes[21]);
	motorRecordVector.contributingFactorVehicle5.push_back(recordAttributes[22]);
	intAttribute(motorRecordVector.collisonId, recordAttributes[23]);
	motorRecordVector.vehicleTypeCode1.push_back(recordAttributes[24]);
	motorRecordVector.vehicleTypeCode2.push_back(recordAttributes[25]);
	motorRecordVector.vehicleTypeCode3.push_back(recordAttributes[26]);
	motorRecordVector.vehicleTypeCode4.push_back(recordAttributes[27]);
	motorRecordVector.vehicleTypeCode5.push_back(recordAttributes[28]);
}
