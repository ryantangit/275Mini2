#include <string>
#include <iostream>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "count.grpc.pb.h"

class CountServiceImpl final: public count::CountService::CallbackService {
	::grpc::ServerUnaryReactor * Count(::grpc::CallbackServerContext* context, const ::count::CountRequest* request, ::count::CountResponse* response) override {
		std::string query_string = request->query();
		int count = query_string.length();
		response->set_count(count);
		grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
    reactor->Finish(grpc::Status::OK);
    return reactor; 
	}
};

void RunServer() {
  std::string server_address("0.0.0.0:8888");
	CountServiceImpl service;
	grpc::EnableDefaultHealthCheckService(true);
	grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
	RunServer();	
	return 0;
}
