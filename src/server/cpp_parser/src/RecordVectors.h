#ifndef RECORDSVECTORS_H 
#define RECORDSVECTORS_H

#include <iostream>
#include <vector>
#include "MotorRecordVector.h"

class RecordVectors{
	private:
		MotorRecordVector records;
	public:
		RecordVectors() {
				const int chunk = 2156579;
				records.crashDate.reserve(chunk);			
				records.crashTime.reserve(chunk);
				records.borough.reserve(chunk);
				records.zipCode.reserve(chunk);
				records.latitude.reserve(chunk);
				records.longitude.reserve(chunk);
				records.location.reserve(chunk);	
				records.onStreeName.reserve(chunk);
				records.crossStreetName.reserve(chunk);
				records.offStreetName.reserve(chunk);
				records.numberOfPersonInjured.reserve(chunk);
				records.numberOfPersonKilled.reserve(chunk);
				records.numberOfPedestrianInjured.reserve(chunk);
				records.numberOfPedestrianKilled.reserve(chunk);
				records.numberOfCyclistInjured.reserve(chunk);
				records.numberOfCyclistKilled.reserve(chunk);
				records.numberOfMotoristInjured.reserve(chunk);
				records.numberOfMotoristKilled.reserve(chunk);
				records.contributingFactorVehicle1.reserve(chunk);
				records.contributingFactorVehicle2.reserve(chunk);
				records.contributingFactorVehicle3.reserve(chunk);
				records.contributingFactorVehicle4.reserve(chunk);
				records.contributingFactorVehicle5.reserve(chunk);
				records.collisonId.reserve(chunk);
				records.vehicleTypeCode1.reserve(chunk);
				records.vehicleTypeCode2.reserve(chunk);
				records.vehicleTypeCode3.reserve(chunk);
				records.vehicleTypeCode4.reserve(chunk);
				records.vehicleTypeCode5.reserve(chunk);
		}
		std::vector<std::string> parseRecord(std::string recordString);
		void addRecord(std::vector<std::string> recordAttributes);
};

#endif
