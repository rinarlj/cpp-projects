#include<iostream>
#include<fstream>
#include<algorithm>
#include<math.h>
#include<vector>
using namespace std;

class Profile
{
private:
    float **Mat;
    float *b, *x;
    int dim, npf;
    float *AP, *LP;
    int *nDiag, *li, *pi, *ki;
    float  *Diag;
    
public:
    Profile(string filename);
    void displayMat();
    void display();
    void displayVect(/*float *v*/);
    void displayResult();
    void solve();
    void solveSup();
    void solveInf();
    void factorLDLt();
    void solveProfile();
    void factor();
    void factorisation();
    void resolution();
    float A(int i, int j);
    float lp(int i, int j);
    void set_lp(int i, int j, float val);
    ~Profile();
};

Profile::Profile(string filename){
    ifstream file(filename);
    if (file){
        file >> dim;
        //cout << "Dimension: "<<dim << endl;
        Mat = new float *[dim];
        b = new float[dim];
        for(int i=0; i<dim; i++){
            Mat[i] = new float [dim];
        }
        file.ignore();
        char caractere=0;
        for(int i=0; i<dim; i++){
            for(int j=0; j<dim; j++){
                if (caractere != ' ' && caractere != '\n')
                        file >> Mat[i][j];
                }
            }
        file.ignore();
        for(int i=0; i<dim; i++)
            file >> b[i];
    }
    file.close();
    x = new float[dim];
    // Search i
    npf=0;
    nDiag = new int [dim];
    for(int i=0; i<dim; i++){
        for(int j=0; j<=i; j++){
            if(Mat[i][j] != 0){
                for(int k=j; k<=i; k++){
                    npf++;
                    if (i == k) nDiag[i] = npf;
                }
                break;
            }
        }
    }
    int np=0;
    AP = new float [npf];
    for(int i=0; i<dim; i++){
        for(int j=0; j<=i; j++){
            if(Mat[i][j] != 0){
                for(int k=j; k<=i; k++){
                    AP[np] = Mat[i][k];
                    np++;
                }
                break; 
            }    
        }
    }
    li= new int[dim];
    pi= new int[dim];
    ki= new int[dim];
    for(int i=0; i<dim; i++){
        li[i] = nDiag[i] -nDiag[i-1] -1;
        pi[i] =  i + 1 - li[i];
        ki[i] = nDiag[i-1]+1; 
    }
    cout<<"NPF= "<<npf<<endl;
}
void Profile::displayMat(){
    cout << endl;
    cout << endl << "Matrice:"<<endl;;
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            cout <<Mat[i][j] <<"    ";
        }
        cout << endl;
    }    
}
void Profile::display(){
    cout << endl;
    cout << endl << "nDiag:"<<endl;
    for(int i=0; i<dim; i++) cout<< nDiag[i] << "  ";
    cout<<endl; 

    cout << endl << "AP: "<<endl;
    for(int i=0; i<npf; i++) cout<< AP[i] << "  ";
    cout<<endl; 


    cout << "\nli: \n";
    for(int i=0; i<dim; i++){
        cout << li[i] << "  ";
        
    }
    cout << "\n\npi: \n";
    for(int i=0; i<dim; i++){
        cout << pi[i] << "  ";
        
    }
    cout<<endl;
   
}

void Profile::displayVect(/*float *v*/){
    cout << endl;
    cout << "Affichage du vecteur:"<< endl;
    for(int i=0; i<dim; i++)
        cout << b[i] << endl;
}
void Profile::displayResult(){
    cout << "x: " <<endl;
    for(int i=0; i<dim; i++){
        cout << x[i] << "  ";
    }
    cout << endl;
}
float Profile::A(int i, int j){
    if(i<j) return A(j,i);
    if(j<ki[i]-1) return 0;
    if(i==j) return AP[nDiag[i]-1];
    else return AP[ki[i]-1 + j -pi[i]-1];

}
float Profile::lp(int i, int j){
    if(i<j) return lp(j,i);
    if(j<ki[i]-1) return 0;
    if(i==j) return LP[nDiag[i]-1];
    else return LP[ki[i]-1 + j -pi[i]-1];

}
void Profile::set_lp(int i, int j, float val){
    if(i<j)  set_lp(j,i,val);
    if(j<ki[i]-1)  return;
    if(i==j)  LP[nDiag[i]-1]=val;
    else LP[ki[i]-1 + j -pi[i]-1]=val;

}
void Profile::factorisation(){
    Diag = new float[dim];
    LP = new float[npf];
    for(int i=0; i<npf; i++)
        LP[i]= 0;
    Diag[0]=AP[0];
    LP[0] = 1;
    float r;
    int max;
    for (int i = 1; i < dim; i++){
        LP[nDiag[i]-1]=1;
        for (int j = pi[i]-1; j < i; j++){
            r=0.0;
            if(pi[i]-1<=pi[j]-1) max=pi[j]-1;
            else max = pi[i]-1;
            for (int k = max; k < j; k++)
                r+=LP[nDiag[i]-1 - i + k] * Diag[k] * LP[nDiag[j]-1 - j + k];
            LP[nDiag[i]-1 - i + j] = 1 / Diag[j] * (AP[nDiag[i]-1 - i + j] - r);
        }
        r=0.0;
        for (int k = pi[i]-1; k < i; k++)
            r += Diag[k] * LP[nDiag[i]-1 - i + k] * LP[nDiag[i]-1 - i + k];
        Diag[i] = AP[nDiag[i]-1] - r;
    }
    
}
void Profile::resolution(){
    for (int i = 0; i < dim; i++){
        x[i] = b[i];
        for (int j = 0; j < i; j++)
            x[i] -= lp(i,j)*x[j];
    }
    for (int i = 0; i < dim; i++)
        x[i] /= Diag[i];
    for (int i = dim-1; i >=0; i--){
        for (int j = i+1; j < dim; j++)
            x[i] -= lp(j,i)*x[j];
    }
    
}
Profile::~Profile(){
    for(int i=0; i<dim; i++)
        delete[] Mat[i];
    delete[] Mat;
    delete[] AP;
    delete[] nDiag;
    delete[] Diag;
    delete[] LP;
    delete[] li;
    delete[] pi;
    delete[] x;
    delete[] ki;
}


int main(){
    Profile profile("data.txt");
    profile.displayMat();
    profile.displayVect();
    profile.display();
    profile.factorisation();
    profile.resolution();
    profile.displayResult();
    
    return 0;
}