// #include <string>
// #include <vector>
// #include <iostream>
// #include <grpcpp/grpcpp.h>
// #include <grpcpp/health_check_service_interface.h>
// #include "csvquery.grpc.pb.h"
// #include "parser.h"
// #include "MotorRecordVector.h"

// class CSVQueryServiceImpl final : public csvquery::CSVQueryService::CallbackService {
//   ::grpc::ServerUnaryReactor* CSVQueryBorough(::grpc::CallbackServerContext* context, 
//                                               const ::csvquery::CSVQueryRequest* request, 
//                                               ::csvquery::CSVQueryResponse* response) override {
//     std::vector<std::vector<std::string>> lines;
//     for (const auto& entry : request->entries()) {
//       lines.push_back(parseRecord(entry));
//     }

//     MotorRecordVector records = loadRecord(lines);
//     int count = 0;
//     for (const auto& current_borough : records.borough) {
//       if (current_borough == request->borough()) {
//         count++;
//       }
//     }

//     response->set_count(count);
//     grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
//     reactor->Finish(grpc::Status::OK);
//     return reactor;
//   }

//   ::grpc::ServerUnaryReactor* CSVNumberOfPersonInjured(::grpc::CallbackServerContext* context,
//                                                        const ::csvquery::CSVNumberOfPersonInjuredQueryRequest* request,
//                                                        ::csvquery::CSVNumberOfPersonInjuredQueryResponse* response) override {
//     std::vector<std::vector<std::string>> lines;
//     for (const auto& entry : request->entries()) {
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
//     std::vector<std::vector<std::string>> lines;
//     for (const auto& entry : request->entries()) {
//       lines.push_back(parseRecord(entry));
//     }

//     MotorRecordVector records = loadRecord(lines);
//     int count = 0;

//     for (const auto& killed : records.numberOfPersonKilled) {
//       if (killed >= request->count()) {
//         count++;
//       }
//     }

//     response->set_count(count);
//     grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
//     reactor->Finish(grpc::Status::OK);
//     return reactor;
//   }

// };

// void RunServer() {
//   std::string server_address("0.0.0.0:8888");
//   CSVQueryServiceImpl service;
//   grpc::EnableDefaultHealthCheckService(true);
//   grpc::ServerBuilder builder;
//   builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//   builder.RegisterService(&service);
//   std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
//   std::cout << "Server listening on " << server_address << std::endl;
//   server->Wait();
// }

// int main() {
//   RunServer();
// }

#include <string>
#include <vector>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <mpi.h>

#include "csvquery.grpc.pb.h"
#include "parser.h"
#include "MotorRecordVector.h"

class CSVQueryServiceImpl final : public csvquery::CSVQueryService::CallbackService {
  ::grpc::ServerUnaryReactor* CSVQueryBorough(::grpc::CallbackServerContext* context,
                                              const ::csvquery::CSVQueryRequest* request,
                                              ::csvquery::CSVQueryResponse* response) override {
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
      std::vector<std::string> all_entries(request->entries().begin(), request->entries().end());
      std::string borough = request->borough();
      int chunk_size = all_entries.size() / (world_size - 1);

      for (int i = 1; i < world_size; ++i) {
        int start = (i - 1) * chunk_size;
        int end = (i == world_size - 1) ? all_entries.size() : start + chunk_size;
        int num_lines = end - start;

        MPI_Send(&num_lines, 1, MPI_INT, i, 3, MPI_COMM_WORLD); // tag 3 = borough
        for (int j = start; j < end; ++j) {
          int len = all_entries[j].size();
          MPI_Send(&len, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
          MPI_Send(all_entries[j].c_str(), len, MPI_CHAR, i, 3, MPI_COMM_WORLD);
        }

        int b_len = borough.size();
        MPI_Send(&b_len, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
        MPI_Send(borough.c_str(), b_len, MPI_CHAR, i, 3, MPI_COMM_WORLD);
      }

      int total_count = 0;
      for (int i = 1; i < world_size; ++i) {
        int partial_count;
        MPI_Recv(&partial_count, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total_count += partial_count;
      }

      response->set_count(total_count);
      grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
      reactor->Finish(grpc::Status::OK);
      return reactor;
    }

    return nullptr;
  }

  ::grpc::ServerUnaryReactor* CSVNumberOfPersonInjured(::grpc::CallbackServerContext* context,
                                                       const ::csvquery::CSVNumberOfPersonInjuredQueryRequest* request,
                                                       ::csvquery::CSVNumberOfPersonInjuredQueryResponse* response) override {
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
      std::vector<std::string> all_entries(request->entries().begin(), request->entries().end());
      int threshold = request->count();
      int chunk_size = all_entries.size() / (world_size - 1);

      for (int i = 1; i < world_size; ++i) {
        int start = (i - 1) * chunk_size;
        int end = (i == world_size - 1) ? all_entries.size() : start + chunk_size;
        int num_lines = end - start;

        MPI_Send(&num_lines, 1, MPI_INT, i, 1, MPI_COMM_WORLD); // tag 1 = injured
        for (int j = start; j < end; ++j) {
          int len = all_entries[j].size();
          MPI_Send(&len, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
          MPI_Send(all_entries[j].c_str(), len, MPI_CHAR, i, 1, MPI_COMM_WORLD);
        }

        MPI_Send(&threshold, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      }

      int total_count = 0;
      for (int i = 1; i < world_size; ++i) {
        int partial_count;
        MPI_Recv(&partial_count, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total_count += partial_count;
      }

      response->set_count(total_count);
      grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
      reactor->Finish(grpc::Status::OK);
      return reactor;
    }

    return nullptr;
  }

  ::grpc::ServerUnaryReactor* CSVNumberOfPersonKilled(::grpc::CallbackServerContext* context,
                                                      const ::csvquery::CSVNumberOfPersonKilledQueryRequest* request,
                                                      ::csvquery::CSVNumberOfPersonKilledQueryResponse* response) override {
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
      std::vector<std::string> all_entries(request->entries().begin(), request->entries().end());
      int threshold = request->count();
      int chunk_size = all_entries.size() / (world_size - 1);

      for (int i = 1; i < world_size; ++i) {
        int start = (i - 1) * chunk_size;
        int end = (i == world_size - 1) ? all_entries.size() : start + chunk_size;
        int num_lines = end - start;

        MPI_Send(&num_lines, 1, MPI_INT, i, 2, MPI_COMM_WORLD); // tag 2 = killed
        for (int j = start; j < end; ++j) {
          int len = all_entries[j].size();
          MPI_Send(&len, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
          MPI_Send(all_entries[j].c_str(), len, MPI_CHAR, i, 2, MPI_COMM_WORLD);
        }

        MPI_Send(&threshold, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
      }

      int total_count = 0;
      for (int i = 1; i < world_size; ++i) {
        int partial_count;
        MPI_Recv(&partial_count, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total_count += partial_count;
      }

      response->set_count(total_count);
      grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
      reactor->Finish(grpc::Status::OK);
      return reactor;
    }

    return nullptr;
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

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if (world_rank == 0) {
    RunServer();
  } else {
    while (true) {
      MPI_Status status;
      int num_lines;
      MPI_Recv(&num_lines, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      std::vector<std::string> local_entries;
      for (int i = 0; i < num_lines; ++i) {
        int len;
        MPI_Recv(&len, 1, MPI_INT, 0, status.MPI_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::vector<char> buffer(len);
        MPI_Recv(buffer.data(), len, MPI_CHAR, 0, status.MPI_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        local_entries.emplace_back(buffer.begin(), buffer.end());
      }

      std::vector<std::vector<std::string>> lines;
      for (const auto& entry : local_entries) {
        lines.push_back(parseRecord(entry));
      }

      MotorRecordVector records = loadRecord(lines);
      int count = 0;

      if (status.MPI_TAG == 1) { // Injured
        int threshold;
        MPI_Recv(&threshold, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (const auto& val : records.numberOfPersonInjured) {
          try {
            if (val >= threshold) count++;
          } catch (...) {}
        }
      } else if (status.MPI_TAG == 2) { // Killed
        int threshold;
        MPI_Recv(&threshold, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (const auto& val : records.numberOfPersonKilled) {
          try {
            if (val >= threshold) count++;
          } catch (...) {}
        }
      } else if (status.MPI_TAG == 3) { // Borough
        int b_len;
        MPI_Recv(&b_len, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::vector<char> borough_buffer(b_len);
        MPI_Recv(borough_buffer.data(), b_len, MPI_CHAR, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        std::string borough(borough_buffer.begin(), borough_buffer.end());

        for (const auto& current_borough : records.borough) {
          if (current_borough == borough) count++;
        }
      }

      MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }

  MPI_Finalize();
  return 0;
}