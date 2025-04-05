#ifndef MOTORRECORDVECTOR_H
#define MOTORRECORDVECTOR_H

#include <iostream>
#include <vector>

struct MotorRecordVector {
	std::vector<std::string> crashDate;			
	std::vector<std::string> crashTime;
	std::vector<std::string> borough;
	std::vector<int> zipCode;
	std::vector<double> latitude;
	std::vector<double> longitude;
	std::vector<std::string> location;	
	std::vector<std::string> onStreeName;
	std::vector<std::string> crossStreetName;
	std::vector<std::string> offStreetName;
	std::vector<int> numberOfPersonInjured;
	std::vector<int> numberOfPersonKilled;
	std::vector<int> numberOfPedestrianInjured;
	std::vector<int> numberOfPedestrianKilled;
	std::vector<int> numberOfCyclistInjured;
	std::vector<int> numberOfCyclistKilled;
	std::vector<int> numberOfMotoristInjured;
	std::vector<int> numberOfMotoristKilled;
	std::vector<std::string> contributingFactorVehicle1;
	std::vector<std::string> contributingFactorVehicle2;
	std::vector<std::string> contributingFactorVehicle3;
	std::vector<std::string> contributingFactorVehicle4;
	std::vector<std::string> contributingFactorVehicle5;
	std::vector<int> collisonId;
	std::vector<std::string> vehicleTypeCode1;
	std::vector<std::string> vehicleTypeCode2;
	std::vector<std::string> vehicleTypeCode3;
	std::vector<std::string> vehicleTypeCode4;
	std::vector<std::string> vehicleTypeCode5;
};

#endif
