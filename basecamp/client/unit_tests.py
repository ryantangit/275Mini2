import unittest
import grpc
import count_pb2 
import count_pb2_grpc

class TestPythongRPC(unittest.TestCase):
    def test_sanity(self):
        try:
            channel = grpc.insecure_channel("0.0.0.0:8765")
            stub = count_pb2_grpc.CountServiceStub(channel)
            response = stub.Count(count_pb2.CountRequest(query="1"))
            self.assertEqual(response.count, 1)
        except grpc.RpcError as e:
            print(f"failed: {e} ")
if __name__ == '__main__':
    unittest.main()
