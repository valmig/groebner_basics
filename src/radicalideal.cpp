#include <Glist.h>
#include <rational.h>
#include <modq.h>
#include <s_polynom.h>
#include <ideal_roots.h>
#include <MyTime.h>


void program_info()
{
	std::cout<<"\nCommand: radicalideal <File> [number of threads]";
	std::cout<<"\n=======";
	std::cout<<"\nComputes a reduced G-Basis of the radical ideal of File!\n";
}


int main(int argnr,char* argv[])
{
	int filetype,nthreads;
	std::string s;
	char sname[] = "gnpoly.txt";
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

	filetype=val::getFileType(argv[1]);

	if (argnr>2) {
		std::string snr(argv[2]);
		nthreads=val::FromString<int>(snr);
		if (nthreads<0) nthreads=1;
		val::SetNumberofThreads(nthreads);
	}
	else val::SetNumberofThreads(nthreads=3);

	val::Glist< val::s_polynom<val::modq> > Gmodq;
	val::Glist< val::s_polynom<val::integer> > Gint;
	val::ChronoClass Chrono;

	if (filetype==3) {
		val::readPolynomBasis(argv[1],Gmodq);
		Chrono();
		val::radical_ideal(Gmodq,nthreads);
	}
	else if (filetype==2) {
		val::readPolynomBasis(argv[1],Gint);
		Chrono();
		val::radical_ideal(Gint,nthreads);
	}
	else {
		std::cout<<"\nWrong type of file!\n"<<std::endl;
		return 0;
	}

	std::cout<<"\n Time : "<<Chrono();

	std::cout<<"\n\nWrite Basis in gnpoly.txt? (y/N): ";
	std::getline(std::cin,s);
	if (s=="j" || s=="J" || s=="y" || s=="Y") {
		if (filetype==3) val::saveGBasis(sname,Gmodq);
		else val::saveGBasis(sname,Gint);
	}

	std::cout<<std::endl;
	return 0;
}
