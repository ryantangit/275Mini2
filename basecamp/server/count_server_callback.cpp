#include <string>
#include <iostream>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "count.grpc.pb.h"
#include <mpi.h>

class CountServiceImpl final: public count::CountService::CallbackService {
	::grpc::ServerUnaryReactor * Count(::grpc::CallbackServerContext* context, const ::count::CountRequest* request, ::count::CountResponse* response) override {
		std::string query_string = request->query();
		int count = query_string.length();
		response->set_count(count);
		grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
    reactor->Finish(grpc::Status::OK);
    return reactor; 
	}

	::grpc::ServerUnaryReactor* CountList(::grpc::CallbackServerContext* context, 
			const ::count::CountListRequest* request, 
			::count::CountResponse* response) override {
		std::cout << "CountList Requested" << std::endl;
		int count = 0;
		int openMPI_world_size;
		MPI_Comm_size(MPI_COMM_WORLD ,&openMPI_world_size);

		int worker_node = 1;
		for (const auto& word: request->query()) {
			int word_size = word.size();
			MPI_Send(&word_size, 1, MPI_INT, worker_node, 0, MPI_COMM_WORLD);
			MPI_Send(word.data(), word_size, MPI_CHAR, worker_node, 0, MPI_COMM_WORLD);
			int counted;
			MPI_Recv(&counted, 1, MPI_INT, worker_node, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			count += counted;
			worker_node = (worker_node + 1) % openMPI_world_size;
			if (worker_node == 0) {
				worker_node += 1;
			}
		}

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
	
	MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		RunServer();	
	} else {
		// Understand, this counting in the worker nodes is unnecessary, because server can handle it via strlen/string.size().
		// But it's for demonstrative purposes that OpenMPI actually works.
		while (true) {
			// Wait to receive how long the message length will be. 
			int message_count = 0;
			MPI_Recv(&message_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			std::vector<char> message(message_count);
			MPI_Recv(message.data(), message_count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			std::cout << "Worker Node: " << rank << " received message: " << message.data() << " size: " << message_count << std::endl;
			MPI_Send(&message_count, 1, MPI_INT , 0, 0, MPI_COMM_WORLD);
		}
	}
	return 0;
}
