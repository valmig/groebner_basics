#include <rational.h>
#include <modq.h>
#include <s_groebner.h>
#include <Glist.h>
#include <s_polynom.h>
#include <n_polynom.h>
#include <string>
#include <val_utils.h>
#include <fstream>
#include <thread>
#include <MyTime.h>


void program_info()
{
    std::cout<<"\nCommand: intersectideals file1 file2 [number of threads]";
    std::cout<<"\n======= Computes the intersection ideal of ideals defined in file1, file2.\n";
}

namespace intersec
{

std::string logfile = "log1.txt";

val::Glist<val::n_polynom<val::modq>> readfromfilemodq(const std::string &filename)
{
    using namespace val;
    Glist<n_polynom<modq>> G;
    std::ifstream file(filename,std::ios::in);

    if (!file) return G;
    int n,ord;
    file>>modq::q>>n>>ord;
    //modq value,zero;
    //n_expo X;
    n_polynom<modq> f;

    n_expo::setordtype(ord);
    if (ord==-1000) {
        matrix<int> A(n);
        for (int i=0;i<n;++i) {
            for (int j=0;j<n;++j) file>>A(i,j);
        }
        n_expo::setordmatrix(std::move(A));
    }
    while (file) {
        file>>f;
        if (!f.iszero()) G.sinsert(std::move(f));
        else break;
    }

    file.close();

    return G;
}



val::Glist<val::n_polynom<val::integer>> readfromfileint(const std::string &filename)
{
    using namespace val;
    Glist<n_polynom<integer>> G;
    std::ifstream file(filename,std::ios::in);

    if (!file) return G;
    int n,ord;
    file>>n>>ord;
    //modq value,zero;
    //n_expo X;
    n_polynom<integer> f;

    n_expo::setordtype(ord);
    if (ord==-1000) {
        matrix<int> A(n);
        for (int i=0;i<n;++i) {
            for (int j=0;j<n;++j) file>>A(i,j);
        }
        n_expo::setordmatrix(std::move(A));
    }
    while (file) {
        file>>f;
        //std::cout<<f<<std::endl;
        if (!f.iszero()) G.sinsert(std::move(f));
        else break;
    }

    file.close();

    return G;
}

val::matrix<int> eliminationmatrix(const val::vector<int> &selection)
{
    int dim=selection.dimension(),i,j,zeile=0,k,l=0;
    val::matrix<int> M(0,dim,dim);

    for (i=0;i<dim;i++) if (selection[i]) l++;

    if (l==dim) {
        for (i=0;i<dim;i++) M(0,i)=1;
        for (i=1;i<dim;i++) M(i,dim-i) =-1;
        return M;
    }

    k=dim -l;//,j,h;

    // Erstelle nun Matrix

    for (i=0;i<dim;i++) {
        if (!selection[i]) M(zeile,i)=1;
    }

    i=dim-1;
    for (j=k-1;j>0;j--) {
         zeile++;
         for (;i>=0;i--) {
             if (!selection[i]) {M(zeile,i)=-1;i--;break;}
        }
    }
    zeile++;
    for (i=0;i<dim;i++) {
        if (selection[i]) M(zeile,i)=1;
    }

    i=dim-1;
    for (j=l-1;j>0;j--) {
        zeile++;
        for (;i>=0;i--) {
            if (selection[i]) {M(zeile,i) =-1;i--;break;}
        }
    }

    return M;
}

void saveBasis(const std::string& filename,const val::Glist<val::n_polynom<val::integer>> &G)
{
    std::ofstream file(filename,std::ios::out | std::ios::trunc);
    file<<val::n_expo::getordtype()<<std::endl<<std::endl;

    for (const auto &f : G) {
        file<<f<<std::endl;
    }
}

int Maxintegerlength(const val::Glist<val::s_polynom<val::integer>> &G)
{
    int m=0;
    for (const auto& f : G) {
        for (const auto& mf : f) {
            m = val::Max(m,mf.actualcoef().abslength());
        }
    }
    return m;
}

} // end namespace intersec




template <class T>
val::Glist<val::s_polynom<T>> idealintersection(const val::Glist<val::n_polynom<T>> &G,const val::Glist<val::n_polynom<T>> &H,int nthreads,int comment =1)
{
    using namespace val;
    Glist<s_polynom<T>> I,Iex;
    if (G.isempty() || H.isempty()) return I;

    //n_polynomIterator<T> it;
    int maxdim=0;

    // Get maxdim:
    for (const auto& f : G) {
        for (const auto &it : f) {
            maxdim = val::Max(maxdim,it.actualterm().dimension());
        }
    }
    for (const auto& f : H) {
        for (const auto& it : f) {
            maxdim = val::Max(maxdim,it.actualterm().dimension());
        }
    }
    //
    int n = maxdim +1;
    s_expo::setdim(n);
    s_expo::setordtype(-1000);
    val::vector<int> selection(0,n);

    for (int i=0;i<n-1;++i) selection(i)=1; // selection = (1...10)
    s_expo::setordmatrix(intersec::eliminationmatrix(selection));


    // construct Iex = (Y*G,(Y-1)*H)  Y last variable
    s_polynom<T> f;
    s_expo X;
    int i;

    for (const auto & g : G) {
        for (const auto & it : g) {
            for (i=0;i<n-1;++i) X[i] = (it.actualterm())[i];
            X[n-1] = 1;
            f.insert(it.actualcoef(),X);
        }
        if (!f.iszero()) Iex.sinsert(std::move(f));
    }

    //val::saveGBasis(std::string("log1.txt"),Iex);

    for (const auto & g : H) {
        for (const auto & it : g) {
            for (i=0;i<n-1;++i) X[i] = (it.actualterm())[i];
            X[n-1] = 1;
            f.insert(it.actualcoef(),X);
            X[n-1] = 0;
            f.insert(-it.actualcoef(),X);
        }
        if (!f.iszero()) Iex.sinsert(std::move(f));
    }

    val::saveGBasis(std::string("log1.txt"),Iex);
    // compute groebner basis
    val::SetNumberofThreads(nthreads);
    if (comment) WriteText("\nComputing groebner basis...");
    val::groebner(Iex);
    if (comment) WriteText("\nGroebner basis computed!");
    if (comment) WriteText("\nBasis has " + val::ToString(Iex.length()) + " elements.");
    //val::saveGBasis(std::string("log1.txt"),Iex);
    //
    // Eliminate last variable:
    s_expo::setdim(n-1);
    s_expo::setordtype(-2);
    s_expo Y;
    int Xn;

    f.del();

    for (const auto& g : Iex) {

        //s_expo::setdim(n);
        //std::cout<<"\n g = \n"<<g;
        //s_expo::setdim(n-1);

        for (auto& it : g) {
            s_expo::setdim(n);
            Xn=(it.actualterm())[n-1];
            s_expo::setdim(n-1);
            //std::cout<<"\n Xn = "<<Xn;
            if (Xn) {
                f.del();
                break;
            }
            else {
                for (i=0;i<n-1;++i) Y[i] = (it.actualterm())[i];
                f.insert(std::move(it.actualcoef()),Y);
            }
        }
        //std::cout<<"\n f= \n"<<f;
        if (!f.iszero()) I.sinsert(std::move(f));
    }

    return I;
}


int main(int argnr,char* argv[])
{
    using namespace val;
 	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr < 3) {
		program_info();
		return 0;
	}
	
    int type1=val::getFileType(argv[1]), type2=val::getFileType(argv[2]);

    std::cout<<"\n type1 = "<<type1;
    std::cout<<"\n type2 = "<<type2;

    if (type1!=type2 || (type1!=2 && type1!=3)) {
        std::cout<<"\nWrong type of file!\n\n";
        return 0;
    }

    int nthreads,maxthreads = std::thread::hardware_concurrency();

    if (maxthreads<3) nthreads=maxthreads;
    else nthreads = maxthreads/2;

    if (argnr==4) {
        nthreads = FromString<int>(std::string(argv[3]));
        if (nthreads<1) nthreads=1;
        if (nthreads>maxthreads) nthreads = maxthreads;
    }

    if (type1==2) { //integer
        Glist<n_polynom<integer>> G,H;
        Glist<s_polynom<integer>> I;
        std::string s;

        G = intersec::readfromfileint(std::string(argv[1]));
        H = intersec::readfromfileint(std::string(argv[2]));

        //intersec::saveBasis(intersec::logfile,G);

        val::ChronoClass Chrono;
        I = idealintersection(G,H,nthreads,1);

        std::cout<<"\nGroebner basis of intersection ideal computed!";
        std::cout<<"\nNumber of elements in basis: "<<I.length();
        std::cout<<"\nNumber of monomials: "<<val::s_polynom<val::integer>::getmnumber();
        std::cout<<"\nMaximum length of integer during computation: "<<val::integer::GetMaxlength();
        std::cout<<"\nMaximum length of integer in basis: "<<intersec::Maxintegerlength(I);
        std::cout<<"\nTime: "<<Chrono();
        std::cout<<"\n\nStore basis in gnpoly.txt? (y/N): ";
        std::getline(std::cin,s);
        if (s=="j" || s=="J" || s=="y" || s=="Y") {
            val::saveGBasis(std::string("gnpoly.txt"),I);
        }
    }
    else {  //modq
        Glist<n_polynom<modq>> G,H;
        Glist<s_polynom<modq>> I;
        std::string s;
        int p;

        G = intersec::readfromfilemodq(std::string(argv[1]));
        p = modq::q;
        H = intersec::readfromfilemodq(std::string(argv[2]));

        if (p!=modq::q) {
            std::cout<<"\nBases have different base fields!\n\n";
            return 0;
        }

        val::ChronoClass Chrono;
        I = idealintersection(G,H,nthreads,1);

        std::cout<<"\nGroebner basis of intersection ideal computed!";
        std::cout<<"\nNumber of elements in basis: "<<I.length();
        std::cout<<"\nNumber of monomials: "<<val::s_polynom<val::integer>::getmnumber();
        std::cout<<"\nTime: "<<Chrono();
        std::cout<<"\n\nStore basis in gnpoly.txt? (y/N): ";
        std::getline(std::cin,s);
        if (s=="j" || s=="J" || s=="y" || s=="Y") {
            val::saveGBasis(std::string("gnpoly.txt"),I);
        }
    }



    std::cout<<std::endl;
    return 0;
}
