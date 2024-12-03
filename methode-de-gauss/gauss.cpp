#include<iostream>
#include<fstream>
#include<algorithm>
#include<math.h>

using namespace std;

class Gauss
{
private:
    float **Mat;
    float *b, *x;
    int dim;
public:
    Gauss(string filename);
    void displayMat();
    void displayVect(/*float *v*/);
    void displayResult();
    void gaussElim();
    void solveTriangSup();
    void factor();
    void solveTriangInf();
    ~Gauss();
};

Gauss::Gauss(string filename){
    ifstream file(filename);
    //lecture des données dans le fichier
    if (file){
        file >> dim;        //lecture de la dimension
        //allocation des tableaux
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
                        file >> Mat[i][j];      //stockage des valeurs dans la matrice
                }
            }
        file.ignore();
        for(int i=0; i<dim; i++)
            file >> b[i];                       //stockage des valeurs dans le vecteur
    }
    file.close();
}
void Gauss::displayMat(){
    cout << endl << "Affichage de la matrice:"<<endl;;
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            cout << Mat[i][j] <<"             ";
        }
        cout << endl;
    }    
}
void Gauss::displayVect(/*float *v*/){
    cout << "\nAffichage du vecteur:"<< endl;
    for(int i=0; i<dim; i++)
        cout << b[i] << endl;
}
void Gauss::displayResult(){
    cout << "\nLa solution est: " << endl;
    for(int i=0; i<dim; i++){
        cout << "x[" << i+1 << "] =  "<< x[i] << endl;
    }
}
void Gauss::gaussElim(){
    cout << "\nElimination de Gauss"<< endl;
    for(int k=0; k<dim; k++){
        for(int l=k+1; l<dim; l++){
            //interversion des lignes selon sa valeur absolue
            if(fabs(Mat[k][k]) < Mat[l][k]){
                float *temp;
                float tp;
                temp = new float[dim];
                temp = Mat[k];
                Mat[k] = Mat[l];
                Mat[l] = temp;
                tp = b[k];
                b[k] = b[l];
                b[l] = tp;
                cout << "Max: "<< l <<"   "<< Mat[k][k] << endl;
                displayMat();
        
                //delete[] temp;
            }
        }
        for(int i=k+1; i<dim; i++){
            for(int j=k+1; j<dim; j++){
                Mat[i][j] = Mat[i][j] - (Mat[i][k]/Mat[k][k])*Mat[k][j];
                cout << Mat[i][j] << "  ";
            }

            b[i] = b[i] - (b[k]*Mat[i][k])/Mat[k][k];
            displayVect();
            Mat[i][k] = 0;
        }
        displayMat();
    }
}
void Gauss::solveTriangSup(){
    cout <<"\nResolution de la matrice triangulaire: "<< endl;
    x[dim-1] = b[dim-1]/Mat[dim-1][dim-1];      
     for(int i=dim-2; i>=0; i--){
        float s=0;
        for(int j=i+1; j<dim; j++){
            s += Mat[i][j]*x[j];
        }
        x[i] = (b[i] - s)/Mat[i][i];
    }
}

void Gauss::factor(){
    Mat[0][0] = sqrt(Mat[0][0]);
    Mat[1][0] = 1/Mat[0][0]*(Mat[1][0]);
        for(int i=0; i<dim; i++){
            for(int j=0; j<i+1; j++){
                float sum1=0, sum2=0;
                for(int k=0; k<j; k++){
                    sum1 += Mat[i][k]*Mat[j][k];
                    sum2 += pow(Mat[i][k], 2);
                     
                }
                if(i==j){
                    Mat[i][i] = sqrt((Mat[i][i] - sum2));
                }else{
                    Mat[i][j] = (1/Mat[j][j])*(Mat[i][j] - sum1);
                }
            }
            for(int j=i+1; j<dim; j++){
                Mat[i][j] = 0;
            }
        }
        Mat[3][3] = 10;
        displayMat();
    
}
void Gauss::solveTriangInf(){
    float *y;
    y = new float [dim];
    y[0] = b[0]/Mat[0][0];      
    for(int i=1; i<dim; i++){
        float s=0;
        for(int j=i-1; j>=0; j--){
            s += Mat[i][j]*y[j];
            cout << "s="<<s<<endl;
        }
        cout<<b[i] << "   "<<Mat[i][i]<<endl;
        y[i] = (b[i] - s)/Mat[i][i];
        cout << y[i] << endl;
    }
    for(int i=0; i<dim; i++)
        cout << "y[" << i << "]"<<y[i] << endl;
    x[dim-1] = y[dim-1]/Mat[dim-1][dim-1];
    cout << x[dim-1]<<endl;      
     for(int i=dim-2; i>=0; i--){
        float s=0;
        for(int j=i+1; j<dim; j++){
            s += Mat[j][i]*x[j];
        }
        x[i] = (y[i] - s)/Mat[i][i];
    }
}

Gauss::~Gauss(){
    for(int i=0; i<dim; i++)
        delete[] Mat[i];
    delete[] Mat; cout << "Mat deleted" << endl;
    delete[] b; cout << "b deleted" << endl;
    //delete[] x; cout << "x deleted" << endl;
}


int main(){
    cout << "Resolution de systeme d'equation" << endl;

    Gauss gauss("data1.txt");
    gauss.displayMat();
    gauss.displayVect();
    // gauss.gaussElim();
    // cout << "\nVoici la matrice triangulaire: "<<endl;
    // gauss.displayMat();
    // gauss.displayVect();
    // gauss.solveTriangSup();
    // gauss.displayResult();
    gauss.factor();
    gauss.solveTriangInf();
    gauss.displayResult();
    return 0;
}
