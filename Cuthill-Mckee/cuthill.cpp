#include<iostream>
#include <fstream>
#include <vector>
#include"cuthill.hpp"
#include<math.h>

using namespace std;

Cuthill::Cuthill(string filename){
    ifstream file(filename);

    if (file){
        file >> dim;     
        for(int i=0; i<dim; i++){
            Mat.push_back(vector<float>(dim));
        }
        file.ignore();
        char caractere=0;
        for(int i=0; i<dim; i++){
            for(int j=0; j<dim; j++){
                if (caractere != ' ' && caractere != '\n') file >> Mat[i][j];     
            }
        }
        file.ignore();
        b.resize(dim);
        for(int i=0; i<dim; i++)
            file >> b[i];
    }
}
void Cuthill::displayMat(string name){
    if (name == "matrice"){
        cout << endl << "Affichage de la matrice:"<<endl;
        for(int i=0; i<dim; i++){
            for(int j=0; j<dim; j++) cout << Mat[i][j] <<"       ";
            cout << endl;
        }
    }
    if (name == "voisin"){
        cout << endl << "Affichage des voisins:"<<endl;
        for(int i=0; i<dim; i++){
            cout <<endl<< i+1 <<" -> ";
            for(float v: V[i]) cout << v << ", ";
            cout << endl;
        }
    }
}
void Cuthill::searchVois(vector<int>& Mat1, vector<int>Mat2, vector<int>& N){
    for (int m: Mat2){
        for (int v : V[m-1]){
            if (Mat1.size()==0){
                for(int n=0; n<(int)(N.size()); n++){
                    if(v != N[n]){
                        if(n == (int)(N.size())-1) Mat1.push_back(v);
                        else continue;
                    }
                    else break;
                }
                continue;
            }
            int i=0, j=0;
            while (i != (int)(Mat1.size()) && j != (int)(N.size())){
                if(Mat1.size() < N.size()){
                    if (v != Mat1[i] && v != N[j]){
                        if(i == ((int)(Mat1.size())-1)){
                            if(j==((int)(N.size()-1))){                    
                                Mat1.push_back(v);
                                break;
                            }
                            else j+=1;
                        }
                        else{
                            i+=1;
                            j+=1;
                        }
                    }
                    else break; 
                }
                else{
                    if (v != Mat1[i] && v != N[j]){                 
                        if(j == ((int)(N.size())-1)){
                            if(i==((int)(Mat1.size()-1))){                           
                                Mat1.push_back(v);
                                break;
                            }
                            else i+=1;
                        }
                        else{
                            i+=1;
                            j+=1;
                        }
                    }
                    else break;
                }
            }   
        }
    }
}
void Cuthill::neighbour(){
    for(int i=0; i<dim; i++){
        vector<int> temp;
        for(int j=0; j<dim; j++){
            if((Mat[i][j] != 0 || Mat[j][i]!=0) && (i!=j)) temp.push_back(j+1);
        }
        V.push_back(temp);
    }
}
int Cuthill::exentricite(int x){
    vector<int> A;
    vector<int> B;
    vector<int> C;
    vector<int> done;
    int compteur=0;

    done.push_back(x);
    for(int i=0; i<3; i++){
        if (i==0){
            A = V[x-1];
            compteur+=1;    
        }
        if (i == 1){
            if (A.empty()){
                N_es.push_back(x);
                break;
            }
            else{
                for(int s:A) done.push_back(s);
                searchVois(B, A, done);
                compteur+=1;
            }
        }
        if (i == 2){
            if (B.empty()){
                N_es = A;
                break;
            }
            else{
                for(int s:B) done.push_back(s);
                searchVois(C, B, done);
                compteur+=1;
            }
        }
    }
    while(true){
        if(compteur%3 == 0){
            if (!C.empty()){
                A.clear();
                for(int s:C) done.push_back(s);
                searchVois(A, C, done);
                compteur+=1;
            }
            else{
                N_es = B;
                break;
            }
        }
        else if(compteur%3 == 1){
            if(!A.empty()){
            B.clear();
            for(int s:A) done.push_back(s);
            searchVois(B, A, done);
            compteur+=1;
            }
            else{
                N_es = C;
                break;
            }
        }
        else if(compteur%3 == 2){
            if(!B.empty()){
            C.clear();
             for(int s:B) done.push_back(s);
            searchVois(C, B, done);
            compteur+=1;
            
            }
            else{
                N_es = A;
                break;
            }
        }
    }
    return compteur-1;
}
int Cuthill::firstNode(int x){
    n = x;
    int en=0;
    vector<int> Nn;
    done.push_back(n);
    en = exentricite(n);
    Nn = N_es;
    for(int s=0; s<(int)(Nn.size()); s++){
        int pass=0;
        for(int d=0; d<(int)(done.size()); d++){
            if(d==(int)(done.size())-1){
                if(Nn[s]!=done[d]){        
                    if(exentricite(Nn[s])>en){
                        n = Nn[s];
                        done.push_back(n);
                        firstNode(n);
                        pass=1;
                        break;
                    }           
                }        
            }
            if(Nn[s]!=done[d]) continue;
            else break;
        }
        if(pass==1) break;
    } 
    return n;   
}
void Cuthill::ComputeMatPrim(){
    vector<vector<int>> inv;
    vector<int> cmki;
    for(int i=0; i<(int)(sommets.size()); i++){
        cmki.push_back(sommets[dim-1-i]);
    }
    Mat_prim.resize(dim, vector<float> (dim));
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            Mat_prim[i][j] = Mat[cmki[i]-1][cmki[j]-1];      
        }
    }
    vector<int> temp;
    temp.resize(dim);
    for(int i=0; i<dim;i++){
        temp[i] = (b[cmki[i]-1]);
    }
    b=temp; 
}
void Cuthill::cmk(){
    int nbNeighbour = 0;
    int next_n = n; 
    int compteur=0;
    sommets.push_back(n);
    int compteursommet=1;
    
    while((int)(sommets.size()) != dim){
        
        vector<int> to_visit;
        //triage
        to_visit = V[next_n-1];
        for(int tv=0; tv<(int)(to_visit.size()); tv++){
            for(int s:sommets){
                if(to_visit[tv]==s){
                    to_visit.erase(to_visit.begin()+tv);
                    tv-=1;
                    break;
                }
            }
        }
        while(!(to_visit.empty())){         
            nbNeighbour = (int)(V[to_visit[0]-1].size());
            int pos =0;
            for(int idx=0; idx<(int)(to_visit.size()); idx++){
                if(nbNeighbour>=(int)(V[to_visit[idx]-1].size())){
                    nbNeighbour=(int)(V[to_visit[idx]-1].size());
                    pos = idx;
                }
            }
            sommets.push_back(to_visit[pos]);
            compteursommet+=1;
            to_visit.erase(to_visit.begin() + pos);
        }
        compteur+=1;
        next_n = sommets[compteur];
    }
    cout<<endl<<endl;
    int sElem;
    vector<int> neigh_from_s;
    //prends les voisins selon les nouvelles numerotations 
    for(int i=0; i<dim; i++){
        sElem = sommets[i];
        neigh_from_s = V[sElem-1];
        CMK.push_back(neigh_from_s);
    }
    ComputeMatPrim();
}
void Cuthill::cmki(){
    vector<int> inverse;
    cmk();
    for(int i=0; i<dim; i++){
        inverse = CMK[dim-1-i];
        CMKI.push_back(inverse);
    }
}

void Cuthill::displayCMKI(){
    cout << endl << "Apres Cuthill-McKee Inverse:"<<endl;
    cout <<endl<<"A': "<< endl;
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++)
            cout<<Mat_prim[i][j]<<"    ";    
        cout<<endl;
    }
    cout<<endl;
 }

void Cuthill::profile(){
    npf=0;
    nDiag.resize(dim, 0);
    for(int i=0; i<dim; i++){
        for(int j=0; j<=i; j++){
            if(Mat_prim[i][j] != 0){
                for(int k=j; k<=i; k++){
                    npf++;
                    if (i == k) nDiag[i] = npf;
                }
                break;
            }
        }
    }
    cout<<"nDiag: "<<endl;
    for(int d:nDiag) cout<<d<<"   ";
    cout<<endl;
    for(int i=0; i<dim; i++){
        for(int j=0; j<=i; j++){
            if(Mat_prim[i][j] != 0){
                for(int k=j; k<=i; k++)
                    AP.push_back(Mat_prim[i][k]);
                break; 
            }    
        }
    }
    cout<<endl<<"A'P: "<<endl;
    for(int s:AP) cout<<s<<"   ";
    cout<<endl;
    li.resize(dim);
    pi.resize(dim);
    ki.resize(dim);
    for(int i=0; i<dim; i++){
        li[i] = nDiag[i] -nDiag[i-1] -1;
        pi[i] =  i + 1 - li[i];
        ki[i] = nDiag[i-1]+1;
    }
    cout<<"\nl: "<<endl;
    for(int s:li) cout<<s<<"   ";
    cout<<endl<<endl;
    cout<<"\np: "<<endl;
    for(int s:pi) cout<<s<<"   ";
}

float Cuthill::A(int i, int j){
    if(i<j) return A(j,i);
    if(j<ki[i]-1) return 0;
    if(i==j) return AP[nDiag[i]-1];
    else return AP[ki[i]-1 + j -pi[i]-1];

}
float Cuthill::lp(int i, int j){
    if(i<j) return lp(j,i);
    if(j<ki[i]-1) return 0;
    if(i==j) return LP[nDiag[i]-1];
    else return LP[ki[i]-1 + j -pi[i]-1];

}
void Cuthill::set_lp(int i, int j, float val){
    if(i<j)  set_lp(j,i,val);
    if(j<ki[i]-1)  return;
    if(i==j)  LP[nDiag[i]-1]=val;
    else LP[ki[i]-1 + j -pi[i]-1]=val;

}
void Cuthill::factorisation(){
    for(int i=0; i<npf; i++)
        LP.push_back(0);
    Diag.push_back(AP[0]);
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
        Diag.push_back(AP[nDiag[i]-1] - r);
    }
    cout<<"\nDiag: " <<endl;
    for(int s:Diag) cout<<s<<"    ";
    cout<<endl<<endl;
    cout<<"L'P: " <<endl;
    for(int s:LP) cout<<s<<"    ";
    cout<<endl<<endl;
}
void Cuthill::resolution(){
    x.resize(dim);
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
    vector<int> temp;
    vector<int> cmki;
    for(int i=0; i<(int)(sommets.size()); i++)
        cmki.push_back(sommets[dim-1-i]);
    temp.resize(dim);
    for(int i=0; i<dim;i++)
        temp[i] = x[cmki[i]-1];
    x=temp;
    cout << "Solution x: " <<endl;
    for(int i=0; i<dim; i++)
        cout << x[i] << "  ";
    cout << endl;
}


