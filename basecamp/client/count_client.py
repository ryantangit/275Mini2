import grpc
import count_pb2 
import count_pb2_grpc

def run():
    try:
       channel = grpc.insecure_channel("ryan@10.0.0.1:8888")
       stub = count_pb2_grpc.CountServiceStub(channel)
       response = stub.Count(count_pb2.CountRequest(query="6 6 6 "))
       print(response.count)
    except grpc.RpcError as e:
        print(f"failed: {e} ")
       

if __name__ == '__main__':
    run()
