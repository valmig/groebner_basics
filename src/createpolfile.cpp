#include <rational.h>
#include <Glist.h>
#include <s_polynom.h>
#include <s_groebner.h>
#include <function_parser.h>
#include <pol_arithmetic.h>
#include <fstream>

void program_info()
{
 std::cout<<"\nCommand: createpolfile <input-File> [output-File = gnpoly.txt]";
 std::cout<<"\n=======";
 std::cout<<"\nCreates a list of polynomials from input-File and writes it to output-File!\n";
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

    while (file.get(s)) {
        //file>>s;
        if (s == ';') {
            F.push_back(valfunction(sf));
            //std::cout<<"\n sf = "<<sf;
            sf="";
        }
        else if (s == '_' || s == '\n') continue; // ignore '_'
        else sf += s;
    }
    if (sf!="") {
		F.push_back(valfunction(sf));
		//std::cout<<"\n sf = "<<sf;
	}
		
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
	std::string fname = "gnpoly.txt";
	val::Glist<val::s_polynom<val::integer>> G;
	
	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") )) {
		program_info();
		return 0;
	}
	
	if (argnr == 3) {
		fname = std::string(argv[2]);
	}
	if (argnr>3) {
		std::cout<<"\nToo many arguments. Quit program!!\n";
		return 0;
	}
	
	makepolynomiallist(std::string(argv[1]),G);
	val::saveGBasis(fname,G);
	std::cout<<"\nPolynomials saved in "<<fname;
	std::cout<<std::endl;
	return 0;
}
