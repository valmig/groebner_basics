#include <fstream>
#include <string>
#include <complex.h>
#include <matrix.h>
#include <ideal_roots.h>
#include <MyTime.h>
#include <thread>

void program_info()
{
	std::cout<<"\nCommand: idealroots <File> [number of threads] [output-File]";
	std::cout<<"\n=======";
	std::cout<<"\nComputation of all roots of a zero-dimensional ideal!\n";
}


int main(int argnr,char* argv[])
{
	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") )) {
        program_info();
        return 0;
    }

    val::matrix<double> MR;
    val::matrix<val::complex> MC;
    int i,j,N,NC,NR;
    unsigned nthreads=3;
    val::ChronoClass Chrono;

    if (argnr>=3) {
        nthreads = val::FromString<unsigned>(std::string(argv[2]));
        if (nthreads<1) nthreads = 1;
        nthreads = val::Min(std::thread::hardware_concurrency(),nthreads);
    }

    N=val::idealroots(std::string(argv[1]),MR,MC,nthreads);
    if (!N) {
        std::cout<<std::endl;
        return 0;
    }

    NR = MR.numberofrows();
    NC = MC.numberofrows();
    std::ofstream *file=NULL;
    std::ostream *os = &std::cout;

    *os<<"\nTotal number of roots: "<<N;
    *os<<"\nNumber of real roots: "<<NR;
    *os<<"\nNumber of complex root: "<<N-NR<<" , found : "<<2*NC<<std::endl;

    if (argnr==4) {
        file = new std::ofstream(argv[3],std::ios::out | std::ios::trunc);
        if (!file) {
            std::cout<<"\nCannot write in file: "<<std::string(argv[3])<<"\n\n";
            return 0;
        }
        *os<<"\nSee file: "<<std::string(argv[3]);
        os = file;
		*os<<"\nTotal number of roots: "<<N;
		*os<<"\nNumber of real roots: "<<NR;
		*os<<"\nNumber of complex root: "<<N-NR<<" , found : "<<2*NC<<std::endl;
    }
    *os<<"\nReal roots: ";
    for (i=0;i<NR;i++) {
        *os<<std::endl;
        for (j=0;j<MR.numberofcolumns();j++) *os<<MR(i,j)<<"  ";
    }
    if (NC) *os<<std::endl<<"\nComplex roots (without conjugates) :";

    for (i=0;i<NC;i++) {
        *os<<std::endl;
        for (j=0;j<MC.numberofcolumns();j++) *os<<MC(i,j)<<"  ";
    }
    //if (argnr==3) file->close();
    std::cout<<"\n Time : "<<Chrono();

    if (file!=NULL) {file->close();delete file;}

    std::cout<<std::endl;
    return 0;
}

