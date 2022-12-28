#include <modq.h>
#include <rational.h>
#include <Glist.h>
#include <s_polynom.h>
#include <s_groebner.h>
#include <function_parser.h>
#include <pol_arithmetic.h>
#include <fstream>

void program_info()
{
	std::cout<<"\nCommand: groebner <File> [Number of Threads]";
	std::cout<<"\n=======";
	std::cout<<"\nComputes a reduced  Groebner-Basis over Q or Fp!";
	std::cout<<"\nPolynomials are of s_polynom type.\n";
}

// returns 1 if polynomials have to be parsed! 0 otherwise
int checkinputtype(const std::string &filename)
{
    std::fstream file(filename,std::ios::in);
    
    if (!file) {
        val::WriteText("\nCannot open file!\n");
        exit(-1);
    }
    
    char s;
    while (file) {
		file>>s;
		if (s == 'x' || s == 'y' || s == 'z') return 1;
	}
	return 0;
}

void makepolynomiallist(const std::string &filename,val::Glist<val::s_polynom<val::integer>> &G)
{
    using namespace val;
    std::fstream file(filename,std::ios::in);

    if (!file) {
        val::WriteText("\nCannot open file!\n");
        exit(-1);
    }

    Glist<valfunction> F;
    std::string sf;
    char s;
    int dim = 0;

    while (file) {
        file>>s;
        if (s == ';') {
            F.push_back(valfunction(sf));
            sf="";
        }
        else sf += s;
    }
    if (sf!="") F.push_back(valfunction(sf));
    if (F.isempty()) return;
    
    // Get max number of variables:
    for (const auto &f : F) dim = val::Max(dim,f.numberofvariables());
    //
    s_expo::setdim(dim);
    s_expo::setordtype(-2);

    s_polynom<rational> fr;

    // Create G:
    if (!G.isempty()) G.dellist();
    for (const auto &f : F) {
        fr = f.gets_polynom();
        if (!fr.iszero()) G.sinsert(val::primitivpart(fr));
    }
    std::cout<<"\n Polynomial list created!";
    std::cout<<"\n Number of variables: "<<s_expo::getdim();
    std::cout<<"\n Number of polynomials: "<<G.length()<<std::endl;
    return;
}


int main(int argnr,char* argv[])
{
	int m;
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

	if (argnr>2) {
		std::string snr(argv[2]);
		int nthreads=val::FromString<int>(snr);

		if (nthreads<0) nthreads=1;
		val::SetNumberofThreads(nthreads);
	}
	else val::SetNumberofThreads(3);

	val::Glist< val::s_polynom<val::modq> > Gmodq;
	val::Glist< val::s_polynom<val::integer> > Gint;


	m=val::getFileType(argv[1]);

	if (checkinputtype(std::string(argv[1]))) {
		makepolynomiallist(std::string(argv[1]),Gint);
		std::cout<<"\nComputing groebner basis...";
		val::groebner(Gint,1);
	}	 
	else if (m==3) {
		std::cout<<"\nComputing groebner basis...";
		val::groebner(std::string(argv[1]),Gmodq);
	}
	else if (m==2) {
		std::cout<<"\nComputing groebner basis...";
		val::groebner(std::string(argv[1]),Gint);
	}
	else {
		std::cout<<"\nWrong type of file!\n";
		return 0;
	}

	std::cout<<"\n\nWrite basis in file gnpoly.txt? (y/N): ";
	std::getline(std::cin,s);
	if (s=="j" || s=="J" || s=="y" || s=="Y") {
		if (m==3) val::saveGBasis(sname,Gmodq);
		else val::saveGBasis(sname,Gint);
	}

	return 0;
}
