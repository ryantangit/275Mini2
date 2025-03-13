import grpc
import count_pb2 
import count_pb2_grpc
import asyncio
import time


# Comparing implementation speed of async code in callback and completion queue.

sync_address = "0.0.0.0:7777"
call_back_address = "0.0.0.0:8888"
completion_queue_address = "0.0.0.0:8765"

def cpu_task(n):
    if n <= 2:
        return 1
    else:
        return cpu_task(n-2) + cpu_task(n-1)


async def async_server(address):
        start = time.perf_counter()
        channel = grpc.aio.insecure_channel(address)
        stub = count_pb2_grpc.CountServiceStub(channel)
        test_string = "a" * 1000
        for _ in range(10000):
           await stub.Count(count_pb2.CountRequest(query=test_string))
        end = time.perf_counter()
        print(f"{address} Time for callback {end - start:.6f} seconds")

def sync_server(address):
    start = time.perf_counter()
    channel = grpc.insecure_channel(address)
    stub = count_pb2_grpc.CountServiceStub(channel)
    test_string = "a" * 1000
    for _ in range(10000):
       stub.Count(count_pb2.CountRequest(query=test_string))
    end = time.perf_counter()
    print(f"{address} Time for callback {end - start:.6f} seconds")

async def test_concurrently():
    await asyncio.gather(async_server(call_back_address), async_server(completion_queue_address))

async def test_call_back():
    print("testing call back")
    await async_server(call_back_address)

async def test_completion_queue():
    print("testing completion queue")
    await async_server(completion_queue_address)

def test_sync():
    print("testing sync")
    sync_server(sync_address)

async def cpu_async_call_back():
    print("testing cpu + call back")
    start = time.perf_counter()
    loop = asyncio.get_running_loop()
    cpu_task_run = loop.run_in_executor(None, cpu_task, 40)
    cpu_task_run2 = loop.run_in_executor(None, cpu_task, 40)
    await asyncio.gather(test_call_back(), cpu_task_run)
    end = time.perf_counter()
    print(f"Time for total cpu + call back {end - start:.6f} seconds")

def cpu_sync():
    print("testing cpu + sync")
    start = time.perf_counter()
    test_sync()
    cpu_task(40)
    cpu_task(40)
    end = time.perf_counter()
    print(f"Time for total cpu + sync {end - start:.6f} seconds")

if __name__ == "__main__":
    #test_sync()
    #asyncio.run(test_concurrently())
    #asyncio.run(test_call_back())
    #asyncio.run(test_completion_queue())
    #asyncio.run(cpu_async_call_back())
    cpu_sync()

