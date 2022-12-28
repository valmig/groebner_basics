#include <s_groebner.h>
#include <iostream>


void program_info()
{
	std::cout<<"\nCommand: isinG <Hfile> <GBfile>";
	std::cout<<"\n=======";
	std::cout<<"\nCheck if polynomials in Hfile are reduced to 0 by the basis of GBfile.";
}



int main(int argnr,char* argv[])
{
    std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") ))  program_info();
    else if (argnr<3 || argnr >3) {
        std::cout<<"\nWrong number of arguments!";
    }
    else val::isinG(std::string(argv[1]),std::string(argv[2]));

    std::cout<<std::endl;
    return 0;
}
