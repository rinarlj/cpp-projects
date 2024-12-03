#include <iostream>
#include <complex> 
#include <cmath>
#include "cuthill.hpp"

using namespace std;



int main() {
    int x, start;
    Cuthill cuthill("data3.txt");
    cuthill.displayMat("matrice");
    cuthill.neighbour();
    cuthill.displayMat("voisin");
    cout<<endl<<"Entrer un sommet : ";
    cin >> x;
    start = cuthill.firstNode(x);
    cout<<"Premier noeud: "<<start<<endl;
    cuthill.cmki();
    cuthill.displayCMKI();
    cuthill.profile();
    cuthill.factorisation();
    cuthill.resolution();
    return 0;
}
