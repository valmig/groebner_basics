#include <modq.h>
#include <integer.h>
#include <Glist.h>
#include <s_polynom.h>
#include <s_groebner.h>
#include <matrix.h>
#include <val_utils.h>
#include <fstream>
#include <string>

void program_info()
{
	std::cout<<"\nCommand: groebnerwalk <File> [k=2] [l=2] [target-order=-1/matrix-file]";
	std::cout<<"\n=======";
	std::cout<<"\nGroebner Basis conversion via a perturbed Groebner Walk!";
	std::cout<<"\nk, l are the perturbation degrees of the initial and target cone.\n";
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
	int m,n,k=2,l=2,neworder=-1;
	std::string s;
	val::matrix<int> Mt;
	char sname[] = "gnpoly.txt";

	if (argnr>6) {
			std::cout<<"\nToo many arguments. Quit Program!!\n";
			return 0;
	}

	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") )) {
		program_info();
		return 0;
	}

	m=val::getFileType(argv[1]);

	if (argnr>=3) {
		k=val::FromString<int>(std::string(argv[2]));
	}
	if (argnr>=4) {
		l=val::FromString<int>(std::string(argv[3]));
	}
	if (argnr==5) {
		n=getdim(argv[1],m);
		Mt=readmatrix(argv[4],n);
		if (!Mt.isempty()) neworder=-1000;
		else {
			neworder=val::FromString<int>(std::string(argv[4]));
			if (neworder <-2 || neworder>0) {
				neworder=-1;
			}
		}
	}

	std::cout<<"\nPerturbation degrees: "<<k<<"  "<<l<<"\nneworder = "<<neworder<<"\n";;

	val::Glist< val::s_polynom<val::modq> > Gmodq;
	val::Glist< val::s_polynom<val::integer> > Gint;

	if (m==3) {
		val::readPolynomBasis(argv[1],Gmodq);
		val::groebnerwalk(Gmodq,neworder,k,l,Mt);
	}
	else if (m==2) {
		val::readPolynomBasis(argv[1],Gint);
		val::groebnerwalk(Gint,neworder,k,l,Mt);
	}
	else {
		std::cout<<"\nWrong Type of File!\n";
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
