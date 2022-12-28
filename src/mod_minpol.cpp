#include <rational.h>
#include <s_groebner.h>
#include <MyTime.h>
#include <s_polynom.h>
#include <Glist.h>
#include <pol.h>
#include <fstream>
#include <ideal_roots.h>


void program_info()
{
	std::cout<<"\nCommand: mod_minpol <File> [Output-File]";
	std::cout<<"\n=======";
	std::cout<<"\nComputes the minimal polynomial of a zero dim. ideal with modular methods!";
	std::cout<<"\nBasis in File must be a Groebner Basis in Q[X]!\n";
}

template <class T>
int savepolynom(char *arg,const val::pol<T> &f)
{
	std::ofstream file(arg,std::ios::out | std::ios::trunc);
	if (!file) {
		std::cout<<("\nCannot write in File!!!\n");
		return 0;
	}
	file<<f;
	std::cout<<"\nWrite polynomial in "<<std::string(arg)<<"\n";
	return 1;
}



int main(int argnr,char* argv[])
{
	int m,k,n,comment=1; 
	val::ChronoClass Chrono;
	if (argnr>3) {
		std::cout<<"\nToo many arguments. Quit program!!";
		return 0;
	}

	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") )) {
		program_info();
		return 0;
	}

	m=val::getFileType(argv[1]);


	if (m!=2) {
		std::cout<<"\nWrong type of file!"<<std::endl;
		return 0;
	}

	val::Glist< val::s_polynom<val::integer> > Gint;
	val::Glist<int> Primes;


	val::pol<val::rational> f;
	val::readPolynomBasis(argv[1],Gint);
	n=val::s_expo::getdim();

	if (!val::iszerodimensional(Gint)) {
		std::cout<<"\n Basis is not zero-dimensional!\n\n";
		return 0;
	}

	std::cout<<"\nInput variable (n = "<<n<<" ): ";
	std::cin>>k;
	if (k<0 || k>=n) k=n-1;
	Chrono();


	comment=1;
	f=val::modint_minimalpolynom(Gint,k,Primes,4,comment);

	if (!comment) std::cout<<"\n f = \n"<<f;
	std::cout<<"\nTime: "<<Chrono();
	std::cout<<"\ndegree of minimal polynomial: "<<f.degree()<<std::endl;
	if (argnr==3) savepolynom(argv[2],f);

	std::cout<<std::endl;
	return 0;
}


