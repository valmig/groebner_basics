#include <modq.h>
#include <rational.h>
#include <s_groebner.h>
#include <MyTime.h>
#include <s_polynom.h>
#include <Glist.h>
#include <pol.h>
#include <fstream>


void program_info()
{
	std::cout<<"\nCommand: minpol <File> [Output-File]";
	std::cout<<"\n=======";
	std::cout<<"\nComputes the minimal polynomial of the zero-dim. Basis in File!";
	std::cout<<"\nBasis in File must be a Groebner Basis!\n";
}

template <class T>
int savepolynom(char *arg,const val::pol<T> &f)
{
	std::ofstream file(arg,std::ios::out | std::ios::trunc);
	if (!file) {
		std::cout<<("Cannot write in file!!\n");
		return 0;
	}
	file<<f;
	std::cout<<"\nWrite polynomial in "<<std::string(arg)<<".\n";
	return 1;
}



int main(int argnr,char* argv[])
{
	int m,k,n;
	val::ChronoClass Chrono;
	val::s_polynom<val::modq>::nreduction=0;

	if (argnr>3) {
		std::cout<<"\nToo many arguments. Quit program!!\n";
		return 0;
	}

	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") )) {
		program_info();
		return 0;
	}

	m=val::getFileType(argv[1]);

	val::Glist< val::s_polynom<val::modq> > Gmodq;
	val::Glist< val::s_polynom<val::integer> > Gint;


	if (m==3) {
		val::pol<val::modq> f;
		val::readPolynomBasis(argv[1],Gmodq);
		std::cout<<"\nElements in Gmod: "<<Gmodq.length();
		n=val::s_expo::getdim();
		std::cout<<"\nInput variable (n = "<<n<<" ): ";
		std::cin>>k;
		if (k<0 || k>=n) k=n-1;
		Chrono();
		f=val::minimalpolynom(Gmodq,k);
		std::cout<<"\nTime: "<<Chrono();
		std::cout<<"\nDegree of the minimal polynomial: "<<f.degree()<<std::endl;
		if (argnr==3) savepolynom(argv[2],f);
	}
	else if (m==2) {
		val::pol<val::rational> f;
		val::readPolynomBasis(argv[1],Gint);
		n=val::s_expo::getdim();
		std::cout<<"\nInput variable (n = "<<n<<" ): ";
		std::cin>>k;
		if (k<0 || k>=n) k=n-1;
		Chrono();
		f=val::minimalpolynom(Gint,k);
		std::cout<<"\nTime: "<<Chrono();
		std::cout<<"\nDegree of the minimal polynomial: "<<f.degree()<<std::endl;
		if (argnr==3) savepolynom(argv[2],f);
	}
	else {
		std::cout<<"\nWrong type of file!"<<std::endl;
		return 0;
	}
	std::cout<<std::endl;
	return 0;
}

