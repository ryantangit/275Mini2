import grpc
import csvquery_pb2
import csvquery_pb2_grpc

import csv
import time
import argparse
from datetime import timedelta

CSV_FILE = 'Motor_Vehicle_Collisions_-_Crashes_20250123.csv'

def chunk_csv_to_arrays(chunk_size=10000):
    chunks = []
    current_chunk = []

    with open(CSV_FILE, 'r', encoding='utf-8') as f:
        reader = csv.reader(f)
        next(reader)

        # for i, row in enumerate(reader, start=1):
        #     current_chunk.append(','.join(row))
        #     if i > 30:
        #         chunks.append(current_chunk)
        #         current_chunk = []
        #         return chunks


        for i, row in enumerate(reader, start=1):
            current_chunk.append(','.join(row))
            if i % chunk_size == 0:
                chunks.append(current_chunk)
                current_chunk = []

        if current_chunk:
            chunks.append(current_chunk)

    return chunks

def run(query_type, value):
    try:
        channel = grpc.insecure_channel("10.0.0.1:8888")
        stub = csvquery_pb2_grpc.CSVQueryServiceStub(channel)

        start_time = time.time()

        for chunk in chunk_csv_to_arrays():
            if query_type == 'borough':
                request = csvquery_pb2.CSVQueryRequest(entries=chunk, borough=value.upper())
                response = stub.CSVQueryBorough(request)

            elif query_type == 'injured':
                request = csvquery_pb2.CSVNumberOfPersonInjuredQueryRequest(entries=chunk, count=int(value))
                response = stub.CSVNumberOfPersonInjured(request)

            elif query_type == 'killed':
                request = csvquery_pb2.CSVNumberOfPersonKilledQueryRequest(entries=chunk, count=int(value))
                response = stub.CSVNumberOfPersonKilled(request)

            else:
                print("Invalid query type. Use 'borough' or 'injured'.")
                return

            print(f"Received {response.count} results.")

        end_time = time.time()
        duration = timedelta(seconds=end_time - start_time)
        print(f"⏱️ Duration: {duration}")

    except grpc.RpcError as e:
        print(f"gRPC error: {e.code()} - {e.details()}")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="CSV gRPC query runner")
    parser.add_argument('--query_type', choices=['borough', 'injured', 'killed'], required=True, help="Type of query to run")
    parser.add_argument('--value', required=True, help="Value for the query (e.g., MANHATTAN or 3)")
    args = parser.parse_args()

    run(args.query_type, args.value)
