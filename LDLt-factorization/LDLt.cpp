#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <vector>
using namespace std;

class LDLt
{
private:
    float **Mat;
    float *b, *x;
    int dim;
    float **D;
    float **L;

public:
    LDLt(string filename);
    void displayMat();
    void displayL();
    void displayD();
    void displayVect(/*float *v*/);
    void displayResult();
    void calcul();
    void solve();
    ~LDLt();
};

LDLt::LDLt(string filename)
{
    ifstream file(filename);
    if (file)
    {
        file >> dim;
        // cout << "Dimension: "<<dim << endl;
        Mat = new float *[dim];
        b = new float[dim];
        for (int i = 0; i < dim; i++)
        {
            Mat[i] = new float[dim];
        }
        file.ignore();
        char caractere = 0;
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                if (caractere != ' ' && caractere != '\n')
                    file >> Mat[i][j];
            }
        }
        file.ignore();
        for (int i = 0; i < dim; i++)
            file >> b[i];
    }
    file.close();
    L = new float *[dim];
    D = new float *[dim];
    for (int i = 0; i < dim; i++)
    {
        D[i] = new float[dim];
        L[i] = new float[dim];
    }
    x = new float[dim];
}
void LDLt::displayMat()
{
    cout << endl;
    cout << "Dimension: " << dim << endl;
    cout << endl
         << "Affichage de la matrice:" << endl;
    ;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            cout << Mat[i][j] << "    ";
        }
        cout << endl;
    }
}
void LDLt::displayL()
{
    cout << endl;
    cout << endl
         << "Affichage de L:" << endl;
    ;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            cout << L[i][j] << "    ";
        }
        cout << endl;
    }
}
void LDLt::displayD()
{
    cout << endl;
    cout << "Affichage de D:" << endl;
    
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            cout << D[i][j] << "    ";
        }
        cout << endl;
    }
}
void LDLt::displayVect(/*float *v*/)
{
    cout << endl;
    cout << "Affichage du vecteur:" << endl;
    for (int i = 0; i < dim; i++)
        cout << b[i] << endl;
}


void LDLt::calcul()
{
    D[0][0] = Mat[0][0];
    L[0][0] = Mat[0][0] / D[0][0];
    for (int i = 0; i < dim; i++)
    {

        for (int j = 0; j < dim; j++)
        {
            if (j < i)
            {
                float s(0);
                for (int k = 0; k < j - 1; k++)
                {
                    s += L[i][k] * D[k][k] * L[j][k];
                }
                L[i][j] = 1 / D[j][j] * (Mat[i][j] - s);
            }
            else
            {
                float d(0);
                for (int k = 0; k < i - 1; k++)
                {
                    d += D[k][k] * L[i][k] * L[i][k];
                }
                D[i][i] = Mat[i][i] - d;
                L[i][j] = 0;
            }
            if (i != j)
            {
                D[i][j] = 0;
            }
            if (i == j)
            {
                L[i][j] = 1;
            }
        }
    }
}
void LDLt::solve(){
    // Lx=b
    for (int i = 0; i < dim; i++){
        float s(0);
        for (int k = 0; k<i; k++){
            s+=L[i][k]*x[k];
        }
        x[i] = b[i] - s;
    }

    // Dx=x
    for (int i = 0; i < dim; i++)
    {
        x[i] = x[i] / D[i][i];
    }
    // Ltx
    for (int i = dim-1; i>=0; i--){
        float s(0);
        for (int k = i+1; k<dim; k++){
            s+=L[k][i]*x[k];
        }
        x[i] = x[i] - s;
        x[i] = round(x[i]);
    }
}

void LDLt::displayResult()
{
    cout << "\nSolution " << endl;
    for (int i = 0; i < dim; i++)
    {
        cout << x[i] << "  " << endl;
    }
}
LDLt::~LDLt()
{
    for (int i = 0; i < dim; i++)
        delete[] Mat[i];
    delete[] Mat;
    for (int i = 0; i < dim; i++)
    {
        delete[] D[i];
        delete[] L[i];
    }
    delete[] D;
    delete[] L;
}

int main()
{

    LDLt fact("data.txt");
    fact.displayMat();
    fact.displayVect();
    fact.calcul();
    fact.displayL();
    fact.displayD();
    fact.solve();
    fact.displayResult();
    return 0;
}