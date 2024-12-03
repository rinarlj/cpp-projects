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
    int *nDiag, *li, *pi;
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
    for(int i=0; i<dim; i++){
        li[i] = nDiag[i] -nDiag[i-1] -1;
        pi[i] =  i + 1 - li[i]; 
    }
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
// void Profile::solveSup(){
//     int ki(0), pi(0);
//     for(int i=dim-1; i>=0; i--){
//         if(i) ki= nDiag[i-1]+1;
//         else ki = 0;
//         pi = ki - nDiag[i] + i;
//         for(int j=pi; j<i; j++){
//             b[j] -= (AP[ki+j-pi]*b[i]);
//         x[i] = b[i];
//         }
//     }
// }
// void Profile::solveInf(){
//     float s=0;
//     int ki(0), pi(0);
//     for(int i=0; i<dim; i++){
//         if(i) ki= nDiag[i-1]+1;
//         else ki = 0;
//         pi = ki - nDiag[i] + i;
//         for(int j=pi, s=0.; j<i; j++){
//             s += (AP[ki+j-pi]*x[j]);
//         x[i] = (b[i] - s);
//         }
//     }
// }
void Profile::solveInf() {
    float s = 0;
    int ki = 0, pi = 0;
    for (int i = 0; i < dim; i++) {
        if (i) ki = nDiag[i - 1] + 1;
        else ki = 0;
        pi = ki - nDiag[i] + i;
        s = 0;  
        for (int j = pi; j < i; j++) {  
            s += (AP[ki + j - pi] * x[j]);
        }
        x[i] = (b[i] - s);  
    }
}

void Profile::solveSup() {
    int ki = 0, pi = 0;
    for (int i = dim - 1; i >= 0; i--) {
        if (i) ki = nDiag[i - 1] + 1;
        else ki = 0;
        pi = ki - nDiag[i] + i;
        for (int j = pi; j < i; j++) {
            b[j] -= (AP[ki + j - pi] * b[i]);
        }
        x[i] = b[i];  
    }
}

void Profile::solveProfile(){
    solveInf();
    
    for(int i=0; i<dim; i++){
        b[i] = x[i]/AP[nDiag[i]];
    }
    
    solveSup();
    displayResult();

}
void Profile::factor(){
    Diag = new float [dim];
    LP = new float [npf];
    Diag[0] = AP[nDiag[0]];
    LP[0] = AP[nDiag[0]] / Diag[0];
    // for (int i = 0; i < dim; i++)
    // {

    //     for (int j = 0; j < dim; j++)
    //     {
    //         if (j < i)
    //         {
    //             float s(0);
    //             for (int k = 0; k < j - 1; k++)
    //             {
    //                 s += LP[nDiag[i] - i + k] * Diag[k] * LP[nDiag[j]-j+k];
    //             }
    //             LP[nDiag[i] - i + j] = 1 / Diag[j] * (AP[nDiag[i] - i + j]- s);
    //         }
    //         else
    //         {
    //             float d(0);
    //             for (int k = 0; k < i - 1; k++)
    //             {
    //                 d += Diag[k] * LP[nDiag[i] - i + k] * LP[nDiag[i] - i + k];
    //             }
    //             Diag[i] = AP[nDiag[i]] - d;
    //             LP[nDiag[i] - i + j] = 0;
    //         }
            
    //     }
    // }
    // cout << "LP: "<<endl;
    // for (int i = 0; i < npf; i++){
    //     cout << LP[i] << "  ";
    // }
    // cout << endl;

    // cout << "Diag "<<endl;
    // for (int i = 0; i < dim; i++){
    //     cout << Diag[i] << "  ";
    // }
    // cout << endl;
    for (int i = 0; i < dim; i++){
        LP[nDiag[i]] = 1;
    }
    for (int i = 1; i < dim; i++){
        for(int j=pi[i]; j<i; j++){
            float r=0;
            for(int k=pi[i]; k<j; k++){
                if(k>=pi[j]){
                    r+=LP[nDiag[i]-i+k]*Diag[k]*LP[nDiag[j]-j+k];
                }
            }
            LP[nDiag[i] - i + j] = 1 / Diag[j] * (AP[nDiag[i] - i + j]- r);
        }
        float d(0);
        for (int k = pi[i]; k < i; k++)
        {
            d += Diag[k] * pow(LP[nDiag[i] - i + k], 2);
        }
        Diag[i] = AP[nDiag[i]] - d;
    }
    cout << "LP: "<<endl;
    for (int i = 0; i < npf; i++){
        cout << LP[i] << "  ";
    }
    cout << endl;

    cout << "Diag "<<endl;
    for (int i = 0; i < dim; i++){
        cout << Diag[i] << "  ";
    }
    cout << endl;      

}
void Profile::factorLDLt(){
    float s(0);
    int pi(0), ki(0), pt(0), kt(0), pj(0), kj(0);
    for (int i = 0; i < dim; i++){
        if(i) ki = nDiag[i-1] +1;
        else ki=0;
        pi = ki - nDiag[i] + i;
        for(int j=pi; j<i; j++){
            if(j) kj = nDiag[j-1] + 1;
            else kj=0;
            pj = kj - nDiag[j] + j;
            s=0;
            for(int t=max(pi, pj); t<j; t++){
                if(t) kt=nDiag[t-1] + 1;
                else kt=0;
                pt = kt - nDiag[t] + t;
                s+=(AP[ki+t-pi]*AP[kt+t-pt]*AP[kj+t-pj]);

            }
            AP[ki+j-pi] = (AP[ki+j-pi]-s)/AP[kj+j-pj];
        }
        s=0;
        for(int t=pi; t<i; t++){
            if(t) kt = nDiag[t-1] +1;
            else kt=0;
            pt = kt - nDiag[t] + t;
            s+=(AP[ki+t-pi]*AP[ki+t-pi]*AP[kt+t-pt]);
        }
        AP[ki+i-pi] -= s;
    }
}
void Profile::solve(){
    // Lx=b
    for (int i = 0; i < dim; i++){
        float s(0);
        for (int k = 0; k<i; k++){
            s+=LP[nDiag[i]-i+k];
        }
        x[i] = b[i] - s;
    }

    // Dx=x
    for (int i = 0; i < dim; i++)
    {
        x[i] = x[i] / Diag[i];
    }
    // Ltx
    for (int i = dim-1; i>=0; i--){
        float s(0);
        for (int k = i+1; k<dim; k++){
            s+=LP[nDiag[k]-k+i]*x[k];
        }
        x[i] = x[i] - s;
        x[i] = round(x[i]);
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
}


int main(){
    Profile profile("data.txt");
    profile.displayMat();
    profile.displayVect();
    profile.display();
    // profile.solveProfile();
    // profile.displayResult();
    profile.factor();
    profile.solve();
    profile.displayResult();
    
    return 0;
}