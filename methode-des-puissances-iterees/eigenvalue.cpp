#include <iostream>
#include <cmath>
#include<fstream>
#include<algorithm>
#include<math.h>

using namespace std;
class PowerMethod
{
    private:
        float **A;
        float *eigenvector, *x;
        int dim;
    public:
        PowerMethod(string filename);
        void displayMat();
        void computeEigenvalue();
        void displayEigenvalue(double evalue);
        void displayEigenvector();
        ~PowerMethod();
};
PowerMethod::PowerMethod(string filename){
    ifstream file(filename);
    //lecture des données dans le fichier
    if (file){
        file >> dim;        //lecture de la dimension
        //allocation des tableaux
        A = new float *[dim]; 
        x = new float[dim];
        eigenvector = new float[dim];
        for(int i=0; i<dim; i++){
            A[i] = new float [dim];
        }
        file.ignore();
        char caractere=0;
        for(int i=0; i<dim; i++){
            for(int j=0; j<dim; j++){
                if (caractere != ' ' && caractere != '\n')
                        file >> A[i][j];      //stockage des valeurs dans la matrice
                }
            }
        file.ignore();
        for(int i=0; i<dim; i++)
            file >> x[i];                       //stockage des valeurs dans le vecteur x
    }
    file.close();
}
void PowerMethod::displayMat(){
    cout << endl << "Affichage de la matrice:"<<endl;
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            cout << A[i][j] <<"             ";
        }
        cout << endl;
    }    
}
void PowerMethod::displayEigenvector(){
    cout << "Eigenvector: ";
    for (int i = 0; i < dim; i++)
    {
        cout << eigenvector[i]<<" ";
    }
}
void PowerMethod::displayEigenvalue(double evalue){
    cout << "The largest eigenvalue is " << evalue << endl;
}
void PowerMethod::computeEigenvalue(){
    double lambda_old = 0;
    double lambda_new = 1;
    double eps = 1e-6;
    int compt=0;

    while (abs(lambda_new - lambda_old) > eps)
    {
        lambda_old = lambda_new;
        double y[dim];
        double norm = 0;
        for (int i = 0; i < dim; i++)
        {
            norm += x[i] * x[i];
        }
        norm = sqrt(norm);
        for (int i = 0; i < dim; i++)
        {
            y[i] = 0;
            y[i] = x[i] / norm;                        //normalise
        }
        for (int i = 0; i < dim; i++)
        {
            x[i] = 0;
            for (int j = 0; j < dim; j++)
            {
                
                x[i] += A[i][j] * y[j];               //itère
            }
        
        }
        
        lambda_new = 0;
        for (int i = 0; i < dim; i++)
        {
            lambda_new += y[i]* x[i];
            cout<<lambda_new<<endl;
        }

        cout << "Eigenvector: ";
        for (int i = 0; i < dim; i++)
        {
            eigenvector[i] = 0;
            eigenvector[i] = y[i];
        }
        cout << endl;
        compt += 1;
    }

    displayEigenvector();
    displayEigenvalue(lambda_new);
    cout << "Nombre d'itérations: "<< compt <<endl;
}
//Liberation de la memoire
PowerMethod::~PowerMethod(){
    for(int i=0; i<dim; i++)
        delete[] A[i];
    delete[] A; cout << "Mat deleted" << endl;
    delete[] x; cout <<"x deleted" << endl;

}
int main()
{
    cout << "Methode des puissances itérées" << endl;

    PowerMethod power("data1.txt");
    power.displayMat();
    power.computeEigenvalue();
    // power.displayEigenvalue();
    // power.displayEigenvector();

    return 0;
}

