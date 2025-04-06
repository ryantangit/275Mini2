#include <string>
#include <vector>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "csvquery.grpc.pb.h"

class CSVQueryServiceImpl final : public csvquery::CSVQueryService::CallbackService {
	::grpc::ServerUnaryReactor * CSVQueryBorough(::grpc::CallbackServerContext* context, 
			const ::csvquery::CSVQueryRequest* request, 
			::csvquery::CSVQueryResponse* response) {
	
		response->set_count(1);
		response->add_results("hello");
		response->add_results("there");
		grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
    reactor->Finish(grpc::Status::OK);
    return reactor;	
	}
	
};

void RunServer() {
  std::string server_address("0.0.0.0:8888");
	CSVQueryServiceImpl service;
	grpc::EnableDefaultHealthCheckService(true);
	grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main() {
	RunServer();
}
