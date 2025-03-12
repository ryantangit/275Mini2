# Base Camp

- Base camp is to investigate communication between multiple processes running on machines.
- For the baseline, we will have a service that takes in a string, and return the number of characters there are in the string.

## Installation

### /Client
Client side is written in python. 
Installation follows conventional python package management. 

```
# Create virtual environemnt if not already
python3 -m venv venv

# Install necessary requirements onto environment
source venv/bin/activate
pip install -r requirements.txt

# Invoke script to generate necesssary protobuffer definitions
./generate_pb.sh

# After server is up and running
python3 unit_tests.py

```

### /Server
Server side is written in C++.
Installation of gRPC / CMake / Protobuffer is assumed.

```
# Create the build folder 
mkdir build && cd build

# CMake via ../server/CMakeLists.txt
cmake ..

# Build the server binary
make -j 4

# Run Async server
./count_server_async

# Optional: Run Synchronous server
./count_server

# Optional: Create compile_commands.json for clang LSP
bear -- make -j 4
```
