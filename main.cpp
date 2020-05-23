#include <iostream>
#include <mpi.h>

class Communication {

public:
	
	Communication(int argc, char **argv) {

		MPI_Init(&argc, &argv);

		MPI_Comm_rank(MPI_COMM_WORLD, &_myrank);

		MPI_Comm_size(MPI_COMM_WORLD, &_numprocs);

		if (_myrank==0) 
			std::cout << "Communication constructed with " << _numprocs << " procs!\n";

	}

	~Communication() {
		MPI_Finalize();
	}

private:

	int _myrank {0};
	int _numprocs {1};
};

int main(int argc, char **argv) {

	Communication comm(argc,argv);

	return 0;

}
