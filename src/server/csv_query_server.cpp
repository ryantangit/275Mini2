// #include <string>
// #include <vector>
// #include <iostream>
// #include <grpcpp/grpcpp.h>
// #include <grpcpp/health_check_service_interface.h>
// #include "csvquery.grpc.pb.h"
// #include "parser.h"
// #include "MotorRecordVector.h"


// class CSVQueryServiceImpl final : public csvquery::CSVQueryService::CallbackService {
// 	::grpc::ServerUnaryReactor * CSVQueryBorough(::grpc::CallbackServerContext* context, 
// 			const ::csvquery::CSVQueryRequest* request, 
// 			::csvquery::CSVQueryResponse* response) {
		
// 		std::vector<std::vector<std::string>> lines;
// 		for (const auto& entry: request->entries()) {
// 			std::cout << entry << std::endl;
// 			lines.push_back(parseRecord(entry));
// 		}		
	
// 		MotorRecordVector records = loadRecord(lines);
// 		int count = 0;
// 		for (const auto& current_borough: records.borough) {
// 			if (current_borough	== request->borough()) {
// 				count += 1;
// 				response->set_count(count);
// 			}
// 		}

// 		grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
//     reactor->Finish(grpc::Status::OK);
//     return reactor;	
// 	}
	
// };

// void RunServer() {
//   std::string server_address("0.0.0.0:8888");
// 	CSVQueryServiceImpl service;
// 	grpc::EnableDefaultHealthCheckService(true);
// 	grpc::ServerBuilder builder;
//   builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//   builder.RegisterService(&service);
//   std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
//   std::cout << "Server listening on " << server_address << std::endl;
//   server->Wait();
// }

// int main() {
// 	RunServer();
// }


#include <string>
#include <vector>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "csvquery.grpc.pb.h"
#include "parser.h"
#include "MotorRecordVector.h"

class CSVQueryServiceImpl final : public csvquery::CSVQueryService::CallbackService {
  ::grpc::ServerUnaryReactor* CSVQueryBorough(::grpc::CallbackServerContext* context, 
                                              const ::csvquery::CSVQueryRequest* request, 
                                              ::csvquery::CSVQueryResponse* response) override {
    std::vector<std::vector<std::string>> lines;
    for (const auto& entry : request->entries()) {
      std::cout << entry << std::endl;
      lines.push_back(parseRecord(entry));
    }

    MotorRecordVector records = loadRecord(lines);
    int count = 0;
    for (const auto& current_borough : records.borough) {
      if (current_borough == request->borough()) {
        count++;
      }
    }

    response->set_count(count);
    grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
    reactor->Finish(grpc::Status::OK);
    return reactor;
  }

//   ::grpc::ServerUnaryReactor* CSVNumberOfPersonInjured(::grpc::CallbackServerContext* context,
//                                                        const ::csvquery::CSVNumberOfPersonInjuredQueryRequest* request,
//                                                        ::csvquery::CSVNumberOfPersonInjuredQueryResponse* response) override {
//     std::vector<std::vector<std::string>> lines;
//     for (const auto& entry : request->entries()) {
//       std::cout << entry << std::endl;
//       lines.push_back(parseRecord(entry));
//     }

//     MotorRecordVector records = loadRecord(lines);
//     int count = 0;
//     for (const auto& injuries : records.numberOfPersonInjured) {
//       try {
//         if (injuries >= request->count()) {
//           count++;
//         }
//       } catch (const std::exception& e) {
//         std::cerr << "Error parsing injury count: " << injuries << " — " << e.what() << std::endl;
//       }
//     }

//     response->set_count(count);
//     grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
//     reactor->Finish(grpc::Status::OK);
//     return reactor;
//   }

//   ::grpc::ServerUnaryReactor* CSVNumberOfPersonKilled(::grpc::CallbackServerContext* context,
//                                                     const ::csvquery::CSVNumberOfPersonKilledQueryRequest* request,
//                                                     ::csvquery::CSVNumberOfPersonKilledQueryResponse* response) override {
// 	std::vector<std::vector<std::string>> lines;
// 	for (const auto& entry : request->entries()) {
// 		std::cout << entry << std::endl;
// 		lines.push_back(parseRecord(entry));
// 	}

// 	MotorRecordVector records = loadRecord(lines);
// 	int count = 0;

// 	for (const auto& killed : records.numberOfPersonKilled) {
// 		if (killed >= request->count()) {
// 		count++;
// 		}
// 	}

// 	response->set_count(count);
// 	grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
// 	reactor->Finish(grpc::Status::OK);
// 	return reactor;
// }

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
