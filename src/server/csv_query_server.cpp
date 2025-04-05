#include <string>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "csvquery.grpc.pb.h"

class CSVQueryServiceImpl final : public csvquery::CSVQueryService::CallbackService {
	::grpc::ServerUnaryReactor * CSVQueryBorough(::grpc::CallbackServerContext* context, 
			const ::csvquery::CSVQueryRequest* request, 
			::csvquery::CSVQueryResponse* response) {
	}
};



int main() {
	std::cout << "hello" << std::endl;
}
