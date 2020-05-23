#include <iostream>
#include <assert.h>
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

	int getRank() {return _myrank;}

	int handShake(int source, int dest, int *buf, int tag) {

		int ierr = 0;

		assert(source >= 0);
		assert(dest >= 0);
		assert(source < _numprocs);
		assert(dest < _numprocs);

		if (_myrank == source) 
			ierr = MPI_Send(buf, sizeof(buf), MPI_INT, dest, tag, MPI_COMM_WORLD);

		if (_myrank == dest) {
			MPI_Status *status;
			ierr = MPI_Recv(buf, sizeof(buf), MPI_INT, source, tag, MPI_COMM_WORLD, status);
		}

		return ierr;
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

	int a[16];

	if (comm.getRank() == 0) {
		a[0] = 1; 
		a[1] = 2; 
		a[2] = 3; 
		a[3] = 4; 
	}

	int ierr = 0;

	ierr = comm.handShake(0,1,a,0);

	return 0;

}
