#!/bin/sh

python -m grpc_tools.protoc -I../proto_def --python_out=. --pyi_out=. --grpc_python_out=. ../proto_def/csvquery.proto
