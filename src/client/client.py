import grpc
import csvquery_pb2
import csvquery_pb2_grpc

def run():
    try:
        channel = grpc.insecure_channel("0.0.0.0:8888")
        stub = csvquery_pb2_grpc.CSVQueryServiceStub(channel)

        request = csvquery_pb2.CSVQueryRequest(
            entries=["123 Manhattan", "432 Manhattan", "New York"],
            borough="Manhattan"
        )

        response = stub.CSVQueryBorough(request)

        print(f"Received {response.count} results:")
        for i, result in enumerate(response.results, 1):
            print(f"{i}: {result}")

    except grpc.RpcError as e:
        print(f"gRPC error: {e.code()} - {e.details()}")

if __name__ == '__main__':
    run()
