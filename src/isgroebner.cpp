#include <s_polynom.h>
#include <MyTime.h>
#include <s_groebner.h>
#include <Glist.h>
#include <integer.h>
#include <modq.h>
#include <d_array.h>
//#include <s_groebner/common_bb.h>



void program_info()
{
	std::cout<<"\nBefehl: isgroebner <File> [number of threads]";
	std::cout<<"\n=======";
	std::cout<<"\nCheck if basis in File is a groebner basis!\n";
}



int main(int argnr,char* argv[])
{
	int k;
	val::s_polynom<val::modq>::nreduction=0;

	if (argnr>3) {
		std::cout<<"\nToo many arguments. Quit program!!"<<std::endl;
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

	k=val::getFileType(argv[1]);

	if (k!=2 && k!=3) {
		std::cout<<"\nWrong type of file"<<std::endl;
		return 0;
	}
	if (k==3) {
		val::Glist< val::s_polynom<val::modq> > G;

		val::isgroebner(argv[1],G,1);
	}
	else {
		val::Glist< val::s_polynom<val::integer> > G;
		val::isgroebner(argv[1],G,1);
	}
	
	std::cout<<std::endl;
	return 0;
}
