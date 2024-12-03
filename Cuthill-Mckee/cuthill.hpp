#include<iostream>
#include <fstream>
#include <vector>

using namespace std;

class Cuthill
{
    private:
        vector<vector<float>> Mat, Mat_prim;
        vector<vector<int>> V;
        vector<int> N_es, done, sommets;
        vector<vector<int>> CMKI;
        vector<vector<int>> CMK;
        vector<int> b, x, li, pi, ki;
        int n;
        int dim, npf;
        vector<int>AP, LP;
        vector<int>nDiag;
        vector<float>Diag;
   
    public:
        Cuthill(string filename);
        void displayMat(string name);
        void  searchVois(vector<int>& Mat1, vector<int>Mat2, vector<int>& N);
        void neighbour();
        int exentricite(int x);
        int firstNode(int x);
        void cmk();
        void cmki();
        void displayCMKI();
        void profile();
        void ComputeMatPrim();
        void factorisation();
        void resolution();
        float A(int i, int j);
        float lp(int i, int j);
        void set_lp(int i, int j, float val);

};
