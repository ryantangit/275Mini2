#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "count.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using count::CountService;

class CountImpl final : public CountService::Service {
	::grpc::Status Count(::grpc::ServerContext *context, const ::count::CountRequest *request, ::count::CountResponse *response) override {
		std::cout << "Received request: "<< context->peer() << std::endl;
			std::string query_string = request->query();
			int count = query_string.length();
			response->set_count(count);
			return grpc::Status::OK;
	}
};

void RunServer() {
  std::string server_address("0.0.0.0:7777");
  CountImpl service;
  grpc::EnableDefaultHealthCheckService(true);
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
	RunServer();
	return 0;
}

