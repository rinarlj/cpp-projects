#include<iostream>
#include<cstdlib>
#include<fstream>

using namespace std;
#include "gestion_eleves.hpp"		


int main(){
	cout<<"Gestion d'élèves"<<endl;
	Menu menu;						//declaration d'un objet de type Menu
	Lstudents liste;				//declaration d'un objet de type Lstudents
	int choix(1);
	
	while(choix){
		choix = menu.getChoice();	//recuperation du choix de l'utilisateur
		menu.manage(liste);
		if (choix==0) break;
	}

	return 0;
}

Student::Student(){
	name = "Toto";
	grade = 10;
}
///Affichage d'un etudiant
void Student::display(){
	cout <<"Nom : "<<name<<"\t Note: "<< grade <<endl;
}

///recuperation du pnext d'un etudiant
Student* Student::getNext(){
	return pnext;
}

///modification du name et grade d'un etudiant
void Student::setStud(string nom, float note){
	name = nom;
	grade = note;
}

///modification du pnext d'un etudiant
void Student::setNext(Student* pst){
	pnext = pst;
}

///recuperation du name d'un etudiant
string Student::getName(){
	return name;
}

///recuperation du grade d'un etudiant
float Student::getGrade(){
	return grade;
}

///constructeur de liste
Lstudents::Lstudents(){
    setHead(nullptr);		//liste vide au depart
}

///creation d'un nouvel etudiant
Student* Lstudents::newStud(string nom, float note){
	Student* pst(nullptr);
    pst = new (nothrow) Student;			//allocation pour la creation d'un eleve
    if(pst!=nullptr){
        pst->setStud(nom, note);			//affectation du name et grade de l'etudiant
    }
    else{
        cout << "Problème d'allocation"<<endl;
        exit(1);
    }
    return pst;
}

///affichage de la liste des etudiants
void Lstudents::display(){
	 Student* pcur(phead);
	 if (getHead() == nullptr){
		cout<<"Liste d'élèves vide"<<endl;
	 }
     while(pcur!=nullptr){
        pcur->display();
        pcur=pcur->getNext();
     }
}

///suppression d'un etudiant de la liste
void Lstudents::delStud(){
	Student* pprev(phead), *pcur(phead), *pst(nullptr);
	string nom;
	cout<<"Nom de l'élève à supprimer: "; cin>> nom;
	if (nom == phead -> getName()){
		pst = phead;
		phead = phead -> getNext();
		cout << "\n" << pst -> getName() << " a été supprimé(e)\n" << endl;
		delete pst;
		return;
	}
	while(pcur != nullptr){
		pprev = pcur;
		pcur = pcur -> getNext();
		if (pcur -> getName()==nom){
			pst = pcur -> getNext();
			pprev -> setNext(pst);
			cout << "\n" << pcur -> getName() << " a été supprimé(e)\n" << endl;
			delete pcur;
			break;
		}
	}
}

///ajout d'un etudiant dans la liste
void Lstudents::addStud(){
	Student* pprev(phead), *pcur(phead);
	string nom("");
	string Note("");
	float note;
	if (getHead() == nullptr){
		cout << "Nom du nouvel élève: "; cin >> nom;
		ici:
		cout << "Note du nouvel élève: "; cin >> Note;
		for(unsigned int i = 0; i < Note.length(); i++){
			if(!isdigit(Note[i])){									//verifie si Note contient un caractere autre qu'un chiffre
				cout << "Entrer un nombre pour la note" << endl;		
				goto ici;
			}
		}
		note = stof(Note);
		cout << endl;
		pcur = newStud(nom, note);
		setHead(pcur); 
		pcur -> display();
		cout << "\nElève ajouté(e)\n"<< endl;	
	}
	else{
		while(pcur != nullptr){
			pprev = pcur;
			pcur = pcur -> getNext();
		}
		cout << "Nom du nouvel élève: "; cin >> nom;
		here:
		cout << "Note du nouvel élève: "; cin >> Note;
		for(unsigned int i = 0; i < Note.length(); i++){
			if(!isdigit(Note[i])){
				cout << "Entrer un nombre pour la note" << endl;
				goto here;
			}
		}
		note = stof(Note);
		cout << endl;
		pcur = newStud(nom, note);
		pprev -> setNext(pcur);
		pcur -> display();
		cout << "\nElève ajouté(e)\n"<< endl;
	}
}

///modification de la tete de liste
void Lstudents::setHead(Student* pst){
	phead = pst;
}

///recuperation du phead
Student* Lstudents::getHead(){
	return phead;
}	
///modification du pnext de prev par next 
void Lstudents::setNext(Student* prev, Student* next){
	prev-> setNext(next);
}

///sauvegarde la liste dans un fichier
void Lstudents::saveData(Lstudents& liste){
	fstream myFile;
	Student* pcur(phead);
	myFile.open("myFile.txt", ios::app);
	if (myFile){
		while(pcur != nullptr){
			myFile << pcur->getName() << "\t";
		    myFile << pcur-> getGrade() << endl;
		    pcur = pcur -> getNext();
		}
	}
	else{
		cout<<"Impossible d'ouvrir le fichier"<<endl;
	}
	myFile.close();
	
}

///recuperation du contenu du fichier
void Lstudents::loadData(){
	ifstream myFile("myFile.txt");
	if(myFile){
		string line("");
		while (getline(myFile, line)){
			cout<<line<<endl;
		}
	}
	myFile.close();
}
///destructeur
Lstudents::~Lstudents(){
	Student* pst(nullptr);
		while(phead!=nullptr){
	  		pst = phead->getNext();
	  		cout<<"deleting"<<endl; phead->display();
	  		delete phead;
	  		setHead(pst);
	  	}	
}

///constructeur du menu
Menu::Menu(){
	items = nullptr;
	dim = 6;
	choice = 0;
	items = new (nothrow) string[dim];
	if (items != nullptr){
		items[0] = "\t0 pour arreter ce programme";
		items[1] = "\t1 pour afficher la liste des élèves";
		items[2] = "\t2 pour ajouter un nouvel élève";
		items[3] = "\t3 pour supprimer un élève";
		items[4] = "\t4 pour sauvegarder la liste";
		items[5] = "\t5 pour récuperer la liste";
	}
	else{
		cout<<"Impssible de créer le menu"<<endl;
		exit(1); 
	} 
}

/// destructeur du menbu
Menu::~Menu(){
	delete [] items;
	cout << "Liste de menu détruite"<< endl;
}

///affichage de la liste et recuperation du choix de l'etudiant
int Menu::getChoice(){
	cout<<"\nVous pouvez taper: \n"<<endl;
	for(int i=1; i < dim; i++)		//affichage des choix
		cout << items[i] <<endl;
		
	cout<<items[0]<<endl;
	
	cout<<"\n\tVotre choix SVP: "; cin>>choice;
	return choice;
}

void Menu::manage()	{
	switch(choice){
		case 1:
			cout<<"\nLa liste des élèves\n"<<endl;
			break;
		case 2:
			cout<<"\nUn nouvel élève inseré\n"<<endl;
			break;
		case 3:
			cout<<"\nUn élève supprimé\n"<<endl;
			break;
		case 4:
			cout<<"\nSauvegarde\n"<<endl;
			break;
		case 5:
			cout<<"\nRecuperation\n"<<endl;
			break;
		case 0:
			cout<<"\nMerci d'avoir utilisé notre produit\n"<<endl;
			break;
		default:
			break;
	}
}

///traitement de la liste selon le choix de l'utilisateur
void Menu::manage(Lstudents& liste){
	switch(choice){
		case 1:
			cout<<"\nLa liste des élèves: \n"<<endl;
			liste.display();
			break;
		case 2:
			cout<<"\nAjout d'un nouvel élève: \n"<<endl;
			liste.addStud();
			break;
		case 3:
			cout<<"\nSuppression d'un élève: \n"<<endl;
			liste.delStud();
			break;
		case 4:
			cout<<"\nSauvegarde de la liste...\n"<<endl;
			liste.saveData(liste);
			cout<<"\nListe sauvegardée\n"<<endl;
			break;
		case 5:
			cout<<"\nRécuperation de la liste\n"<<endl;
			liste.loadData();
			break;
		case 0:
			cout<<"\nMerci d'avoir utilisé notre produit\n"<<endl;
			break;
		default:
			break;
	}
}
