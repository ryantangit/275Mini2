import unittest
import grpc
import count_pb2 
import count_pb2_grpc
import asyncio


class TestPythongRPC(unittest.TestCase):
    def test_sanity(self):
        try:
            channel = grpc.insecure_channel("0.0.0.0:8888")
            stub = count_pb2_grpc.CountServiceStub(channel)
            response = stub.Count(count_pb2.CountRequest(query="1"))
            self.assertEqual(response.count, 1)
        except grpc.RpcError as e:
            print(f"failed: {e} ")

    async def async_two_clients(self):
        channel = grpc.aio.insecure_channel("0.0.0.0:8888")
        stub = count_pb2_grpc.CountServiceStub(channel)
        test_string = "a" * 1000
        response_list = []  
        for _ in range(50):
            response = await stub.Count(count_pb2.CountRequest(query=test_string))
            response_list.append(response.count)
       
        for response in response_list:
            self.assertEqual(response, 1000)

    def test_async_test(self):
       asyncio.run(self.async_two_clients())

    async def async_list_count(self):
        channel = grpc.aio.insecure_channel("0.0.0.0:8888")
        stub = count_pb2_grpc.CountServiceStub(channel)
        request_list = ["hello", "world", "ryan"]
        request_list_count = 14
        response = await stub.CountList(count_pb2.CountListRequest(query=request_list))
        self.assertEqual(response.count, request_list_count)
    
    def test_async_test_countList(self):
        asyncio.run(self.async_list_count())
          
if __name__ == '__main__':
    unittest.main()
