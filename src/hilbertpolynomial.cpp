#include <rational.h>
#include <pol.h>
#include <s_groebner.h>

void program_info()
{
	std::cout<<"\nCommand: hilbertpolynomial <File>";
	std::cout<<"\n=======";
	std::cout<<"\nComputes the affine hilbert-polynomial of the initial terms in File!\n";
}

int main(int argnr,char* argv[])
{
	if (argnr>2) {
		std::cout<<"\nToo many arguments. Quit program!!\n";
	return 0;
	}

	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") )) {
		program_info();
		return 0;
	}
	std::cout<<"\nAffine hilbert-polynomial: \n"<<val::Hilbertpolynomial(argv[1])<<std::endl;

	std::cout<<std::endl;
	return 0;
}
