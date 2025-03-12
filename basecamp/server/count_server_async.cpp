#include <iostream>
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "count.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;

class CountImpl final {
	private:
  	std::unique_ptr<grpc::ServerCompletionQueue> cq_;
		count::CountService::AsyncService service_;
  	std::unique_ptr<Server> server_;
		class CallData {
		 public:
			// Take in the "service" instance (in this case representing an asynchronous
			// server) and the completion queue "cq" used for asynchronous communication
			// with the gRPC runtime.
			CallData(count::CountService::AsyncService* service, grpc::ServerCompletionQueue* cq)
					: service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
				// Invoke the serving logic right away.
				Proceed();
			}
			void Proceed() {
				if (status_ == CREATE) {
					// Make this instance progress to the PROCESS state.
					status_ = PROCESS;
					// As part of the initial CREATE state, we *request* that the system
					// start processing SayHello requests. In this request, "this" acts are
					// the tag uniquely identifying the request (so that different CallData
					// instances can serve different requests concurrently), in this case
					// the memory address of this CallData instance.
					service_->RequestCount(&ctx_, &request_, &responder_, cq_, cq_,
																		this);
				} else if (status_ == PROCESS) {
					// Spawn a new CallData instance to serve new clients while we process
					// the one for this CallData. The instance will deallocate itself as
					// part of its FINISH state.
					new CallData(service_, cq_);

					// The actual processing.
					std::string query_string = request_.query();
					int count = query_string.length();
					reply_.set_count(count);

					// And we are done! Let the gRPC runtime know we've finished, using the
					// memory address of this instance as the uniquely identifying tag for
					// the event.
					status_ = FINISH;
					responder_.Finish(reply_, grpc::Status::OK, this);
				} else {
					if (status_ != FINISH) {
						std::cout << ("Status not equal to Finish but in Finish phase");
					}
					// Once in the FINISH state, deallocate ourselves (CallData).
					delete this;
				}
			}
		 private:
			// The means of communication with the gRPC runtime for an asynchronous
			// server.
			count::CountService::AsyncService* service_;
			// The producer-consumer queue where for asynchronous server notifications.
			grpc::ServerCompletionQueue* cq_;
			// Context for the rpc, allowing to tweak aspects of it such as the use
			// of compression, authentication, as well as to send metadata back to the
			// client.
			grpc::ServerContext ctx_;

			// What we get from the client.
			count::CountRequest request_;
			// What we send back to the client.
			count::CountResponse reply_;

			// The means to get back to the client.
			grpc::ServerAsyncResponseWriter<count::CountResponse> responder_;

			// Let's implement a tiny state machine with the following states.
			enum CallStatus { CREATE, PROCESS, FINISH };
			CallStatus status_;  // The current serving state.
		};

	public:
		~CountImpl() {
			server_->Shutdown(); 	
			cq_->Shutdown();
		}


		// There is no shutdown handling in this code.
		void Run() {
  		std::string server_address("0.0.0.0:8765");
			ServerBuilder builder;
			// Listen on the given address without any authentication mechanism.
			builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
			// Register "service_" as the instance through which we'll communicate with
			// clients. In this case it corresponds to an *asynchronous* service.
			builder.RegisterService(&service_);
			// Get hold of the completion queue used for the asynchronous communication
			// with the gRPC runtime.
			cq_ = builder.AddCompletionQueue();
			// Finally assemble the server.
			server_ = builder.BuildAndStart();
			std::cout << "Server listening on " << server_address << std::endl;

			// Proceed to the server's main loop.
			HandleRpcs();
		}

		void HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service_, cq_.get());
    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
			
			//Blocking Call
			if (!cq_->Next(&tag, &ok)) {
				std::cout << "Completion Queue Next call errored";
			}
			if (!ok) {
				std::cout << "Event retrieved from Completion Queue is not valid";
			}
      static_cast<CallData*>(tag)->Proceed();
    }
  }
};

int main(int argc, char** argv) {
	CountImpl server;
	server.Run();
	return 0;
}

