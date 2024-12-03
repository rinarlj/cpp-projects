class Student{
    public:
		Student();
		void 	 setStud(string nom, float note);
		void 	 display();
		Student* getNext();								//pour chainer
		void 	 setNext(Student* pst);
		string   getName();
		float 	 getGrade();
    
    private:
		string name;
		float grade;
		Student* pnext;
};
class Lstudents{
	public:
		Lstudents();										//constructeur
		Student* newStud(string nom, float note);
		void 	 setNext(Student* prev, Student* next);
		void 	 setHead(Student* pst);
		Student* getHead();		
		void 	 display();									//afficher une liste
		void 	 addStud();									//ajouter un etudiant
		void 	 delStud();									//supprimer un etudiant
		void 	 saveData(Lstudents& liste);				//sauvegarder sur disque
		void 	 loadData();								//recuperer une liste stocquee sur disque
		~Lstudents();										//destructeur
		
	private:
		Student* phead;
};
class Menu{
	public:
		Menu();					
		int  getChoice();
		void manage();
		void manage(Lstudents& liste);
		~Menu();				
	private:
		int dim;
		string* items;
		int choice;
};
