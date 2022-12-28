
#include <string>
#include <vector.h>
#include <matrix.h>
#include <val_utils.h>
#include <fstream>



void program_info()
{
 std::cout<<"\nCommand: eliminationmatrix nvariables [elimanation-flag] [file]";
 std::cout<<"\n=======";
 std::cout<<"\nCreates an Eliminations-Termorder-Matrix with nvarables - variables";
 std::cout<<"\nwrt. the elimintations-flag [default: 0...01] and stores it eventually in <file>\n";
}


void setselection(char* sel,val::vector<int> &selected)
{
    int n=selected.dimension(),i;
    for (i=0;i<n && sel[i]!= '\0';i++)
        if (sel[i]!='0') selected[i]=1;

    // pruefe ob selected==0
    if (selected.iszero()) selected[n-1] = 1;
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


void savematrix(char *name,const val::matrix<int> &M)
{

 int i,j,n=M.numberofcolumns();

 std::ofstream file(name,std::ios::out | std::ios::trunc);
 if (!file) {
	 std::cout<<"\nCannot write in file!!!\n";
	 return;
 }
 file<<n<<std::endl;
 for (i=0;i<n;i++) {
    for (j=0;j<n;j++) file<<M(i,j)<<" ";
    file<<std::endl;
 }
 std::cout<<"\nMatrix stored in "<<std::string(name)<<" !\n";

}



int main(int argnr,char* argv[])
{
    int n=1,i,j;
    val::vector<int> selected;
    val::matrix<int> M;

	std::string firstarg;
	if (argnr == 2) firstarg = std::string(argv[1]);
	
	if (argnr == 1 || (argnr == 2 && (firstarg == "-h" || firstarg == "--help") )) {
        program_info();
	    return 0;
    }

    if (argnr>=2) {
        n=val::FromString<int>(std::string(argv[1]));
        if (n<=0) return 0;
        else selected = val::vector<int>(0,n);
    }
    if (argnr>=3) {
        setselection(argv[2],selected);
    }
    else {
        selected[n-1]=1;
    }

    std::cout<<n<<std::endl;
    std::cout<<selected<<std::endl<<std::endl;

    M=eliminationmatrix(selected);

    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) std::cout<<M(i,j)<<" ";
        std::cout<<std::endl;
    }

    if (argnr>=4) savematrix(argv[3],M);

    std::cout<<std::endl;

    return 0;
}
