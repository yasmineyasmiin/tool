#include <time.h>
#include <iostream>
#include <string>


#include <iomanip>

#include <map>

#include <algorithm>

using namespace std;
#include "Net.h"
#include "Net.cpp"
#include "bdd.h"
#include "fdd.h"

#include "RdPBDD.h"

double getTime(){
	return (double)clock() / (double)CLOCKS_PER_SEC;
}
  
int main(int argc, char** argv) {
    cout << "nom fichier source " << endl;
	cout<<"Fichier net : "<<argv[1]<<endl;
	net R(argv[1]);	
	cout << "parser...done" << endl;
	set <vector<int>> abstrait;
    
    double d,tps;  
    int b = 32;
    map<int,int> obs;
    set<int> unobs;
    set<vector<int>> chemins;
   
    d = getTime();




    unobs=R.calcul1();
    
    for (long unsigned int i = 0; i < R.transitions.size(); i++ )
    {
         if ((unobs.find(i))== unobs.end())
         {
             obs.insert({i,1}); 
         }  
    }	
	RdPBDD DR(R,obs,unobs, b);
   	MDGraph g;

    chemins= DR.chem_obs(g,obs);


 for (auto i: chemins)
 {

  vector<int> chem_abs;
  chem_abs= DR.chem_abs(i,g);
  
  abstrait.insert(chem_abs); }

 
 tps = getTime() - d;

   cout << " Temps de construction du graphe d'observation " << tps << endl;
   g.printCompleteInformation();
  cout<< "transi "<<R.transitions.size()<<endl;
  cout<< "etat "<<R.places.size()<<endl;
    cout<< "trans obs "<<obs.size()<<endl;
  for (auto i: abstrait)
 {

  for (auto tr:i){  cout<<"t"<<tr+1;}
    cout<< endl;
  }
	return 0;
 }
