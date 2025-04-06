import grpc
import csvquery_pb2
import csvquery_pb2_grpc

import csv

CSV_FILE = 'Motor_Vehicle_Collisions_-_Crashes_20250210.csv'

def chunk_csv_to_arrays(chunk_size=10000):
    chunks = []
    current_chunk = []

    with open(CSV_FILE, 'r', encoding='utf-8') as f:
        reader = csv.reader(f)
        next(reader)

        for i, row in enumerate(reader, start=1):
            current_chunk.append(','.join(row))
            if i % chunk_size == 0:
                print(f"finish chunking {i} array")
                chunks.append(current_chunk)
                current_chunk = []

        if current_chunk:
            chunks.append(current_chunk)

    return chunks
    
def run():
    try:
        channel = grpc.insecure_channel("0.0.0.0:8888")
        stub = csvquery_pb2_grpc.CSVQueryServiceStub(channel)

        for chunk in chunk_csv_to_arrays():
            request = csvquery_pb2.CSVQueryRequest(
                entries=chunk,                
                borough="Manhattan"
            )
            response = stub.CSVQueryBorough(request)

            print(f"Received {response.count} results:")

    except grpc.RpcError as e:
        print(f"gRPC error: {e.code()} - {e.details()}")

if __name__ == '__main__':
    run()
