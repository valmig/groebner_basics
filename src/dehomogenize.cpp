#include <iostream>
#include <s_groebner.h>


void program_info()
{
 std::cout<<"\nCommand: dehomogenize <input-File> [output-File=gnpoly.txt]";
 std::cout<<"\n=======";
 std::cout<<"\nDehomogenizes Basis in input-File and writes it in  output-File!\n";
}



int main(int argnr,char* argv[])
{
    std::string i_name,o_name;

	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") )) {
        program_info();
        return 0;
    }

    i_name=std::string(argv[1]);

    if (argnr>=3) o_name=std::string(argv[2]);
    else o_name="gnpoly.txt";

    val::dehomogenize(i_name,o_name,1);
    
    std::cout<<std::endl;
    return 0;
}
