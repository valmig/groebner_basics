#include <modq.h>
#include <integer.h>
#include <Glist.h>
#include <s_polynom.h>
#include <s_groebner.h>
#include <matrix.h>
#include <fstream>
#include <val_utils.h>


void program_info()
{
 std::cout<<"\nCommand: hilbertconversion <File> [threads = 4] [order=-1 / <matrix-File>]";
 std::cout<<"\n=======";
 std::cout<<"\nConversion of the Grobner Basis in File using Hilberts criterion!";
 std::cout<<"\nBasis in File must be a Groebner Basis.\n";
}


val::matrix<int> readmatrix(char *name,int dim)
{
    int n,i,j;
    val::matrix<int> M;
    std::ifstream file(name,std::ios::in);

    if (!file) return M;
    file>>n;
    if (n!=dim) return M;
    M= val::matrix<int>(n);
    for (i=0;i<n;i++)
        for (j=0;j<n;j++) file>>M(i,j);

    return M;
}


int getdim(char * name,int type)
{
    int n=0;
    std::ifstream file(name,std::ios::in);
    if (type==3) {
        file>>n>>n;
    }
    else file>>n;

    return n;
}



int main(int argnr,char* argv[])
{
	int m,order=-1,n;
	std::string s;
	char sname[] = "gnpoly.txt";
	val::matrix<int> M;

	if (argnr>4) {
		std::cout<<"\nToo many arguments. Quit program!!\n";
		return 0;
	}

	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") )) {
		 program_info();
		 return 0;
	}

	val::SetNumberofThreads(4);

	m=val::getFileType(argv[1]);
	n=getdim(argv[1],m);

	if (argnr >= 3) {
		int n = val::FromString<int>(std::string(argv[2]));
		if (n<1) n = 1;
		val::SetNumberofThreads(n);
	}

	if (argnr==4) {
		M=readmatrix(argv[3],n);
		if (!M.isempty()) order = -1000;
		else {
			order=val::FromString<int>(std::string(argv[3]));
			if (order<-2 || order>0) order =-1;
		}
	}
	val::Glist< val::s_polynom<val::modq> > Gmodq;
	val::Glist< val::s_polynom<val::integer> > Gint;

	if (m==3) {
		val::hilbertconversion(argv[1],Gmodq,order,M);
	}
	else if (m==2) val::hilbertconversion(argv[1],Gint,order,M);
	else {
		std::cout<<"\nWrong type of file!\n";
		return 0;
	}

	std::cout<<"\n\nWrite Basis in gnpoly.txt (y/N): ";
	std::getline(std::cin,s);
	if (s=="j" || s=="J" || s=="y" || s=="Y") {
		if (m==3) val::saveGBasis(sname,Gmodq);
		else val::saveGBasis(sname,Gint);
	}

	std::cout<<std::endl;
	return 0;
}
