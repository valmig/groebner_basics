#include <modq.h>
#include <integer.h>
#include <Glist.h>
#include <s_polynom.h>
#include <s_groebner.h>
#include <function_parser.h>
#include <pol_arithmetic.h>
#include <fstream>
#include <val_filesys.h>


void program_info()
{
 std::cout<<"\nCommand: homgroebner <File> [Number of Threads]";
 std::cout<<"\n=======";
 std::cout<<"\nComputes a Groebner Basis via homogenization!";
}


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
		std::cout<<std::endl;		
		return 0;
	}

	m=val::getFileType(argv[1]);

	//element<modq>::newtype=0;
	val::Glist< val::s_polynom<val::modq> > Gmodq;
	val::Glist< val::s_polynom<val::integer> > Gint;

	if (argnr>2) {
		std::string snr(argv[2]);
		int nthreads=val::FromString<int>(snr);

		if (nthreads<0) nthreads=1;
		val::SetNumberofThreads(nthreads);
	}
	else val::SetNumberofThreads(3);

	if (checkinputtype(std::string(argv[1]))) {
		makepolynomiallist(std::string(argv[1]),Gint);
		std::string hfile = "000000temp.txt";
		val::saveGBasis(hfile,Gint);
		std::cout<<"\nComputing groebner basis...";
		val::homgroebner(hfile,Gint);
		val::Delete_File(hfile);
	}	 
	else if (m==3) {
		val::homgroebner(argv[1],Gmodq);
	}
	else if (m==2) val::homgroebner(argv[1],Gint);
	else {
		std::cout<<"\nWrong type of file!\n";
		return 0;
	}

	std::cout<<"\n\nWrite Basis in gnpoly.txt? (y/N): ";
	std::getline(std::cin,s);
	if (s=="j" || s=="J" || s=="y" || s=="Y") {
		if (m==3) val::saveGBasis(sname,Gmodq);
		else val::saveGBasis(sname,Gint);
	}

	std::cout<<std::endl;
	return 0;
}
