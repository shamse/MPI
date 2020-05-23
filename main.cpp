#include <iostream>
#include <assert.h>
#include <mpi.h>
using namespace std;

class Communication {

public:
	
	Communication(int argc, char **argv) {

		MPI_Init(&argc, &argv);

		MPI_Comm_rank(_comm, &_myrank);

		MPI_Comm_size(_comm, &_numprocs);

		if (_myrank==0) 
			std::cout << "Communication constructed with " << _numprocs << " procs!\n";

	}

	int getRank() {return _myrank;}

	int deliver(int source, int dest, int *buf, int nbuf, int tag) {

		int ierr;

		assert(source >= 0);
		assert(dest >= 0);
		assert(source < _numprocs);
		assert(dest < _numprocs);

		if (_myrank == source)  {
			ierr = MPI_Send(buf, nbuf, MPI_INT, dest, tag, _comm);
			errorCheck(ierr);
		}

		if (_myrank == dest) {
			ierr = MPI_Recv(buf, nbuf, MPI_INT, source, tag, _comm, MPI_STATUS_IGNORE);
			errorCheck(ierr);
		}

		return ierr;
	}

	void errorCheck(int ierr) {

		switch (ierr) {
		case MPI_SUCCESS:
		case MPI_ERR_COMM:
		/*
			 place holder
		 */
		default :
			break;
		}

	}

	~Communication() {
		MPI_Finalize();
	}

private:

	int _myrank {0};
	int _numprocs {1};
	MPI_Comm _comm {MPI_COMM_WORLD};
};

int main(int argc, char **argv) {

	Communication comm(argc,argv);

	int a[16];
	int nbuf = 4;

	if (comm.getRank() == 0) {
		a[0] = 1; 
		a[1] = 2; 
		a[2] = 3; 
		a[3] = 4; 
	}

	int ierr = 0;

	ierr = comm.deliver(0,1,a,nbuf,0);

	if (comm.getRank() == 1) {
		cout << '[' << comm.getRank() << "] ";
		for (int i=0; i < nbuf; i++)
			cout << a[i] << ' ';
		cout << endl;
	}

	return 0;

}
