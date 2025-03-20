#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
		MPI_Init(&argc, &argv);

		int rank, size;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		if (rank == 0) {
			std::cout << "It is I, the server" << std::endl;
			for (int i = 1; i < size; i++) {
				std::string message = "hello";
				MPI_Send(message.data(), message.size(), MPI_CHAR, i, 0 , MPI_COMM_WORLD);
			}
		} else {
				char buffer[6];
				buffer[5] = '\0';
				MPI_Recv(buffer, 5, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				std::cout << buffer << " from process " << rank << std::endl;
		}
		MPI_Finalize();
		return 0;
}

