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
    
    cout<<"transitions unobservées trouvées"<<endl;
    for (long unsigned int i = 0; i < R.transitions.size(); i++ )
    {
         if ((unobs.find(i))== unobs.end())
         {
             obs.insert({i,1}); 
         }  
    }	
	RdPBDD DR(R,obs,unobs, b);
	    cout<<"RdpBDD construit"<<endl;
    MDGraph g;

    chemins= DR.chem_obs(g,obs);
 /*  cout << " n de chemins " << chemins.size() << endl;
    for (auto i: chemins)
    {   cout << " le chemin : "  << endl;
        for (auto k:i){
            cout <<"t"<< k+1  << endl;
            }
    }*/


 

/*for(auto i:g.GONodes)
{
cout<<"le noeud num "<<i->class_state.id()<<" a comme succ: "<<endl;
    for(auto k: i->Successors){
        cout<<"**t"<<k.second+1<<" jusqu'a "<<k.first->class_state.id()<<endl;
        }
                cout<<"**"<<endl;
}
  
 
 
*/

 for (auto i: chemins)
 {
 // cout<< "---un nouv chem----"<<endl;
  vector<int> chem_abs;
  chem_abs= DR.chem_abs(i,g);
  
  abstrait.insert(chem_abs); }
/*  
  cout<< "************************ ********************** ************************"<<endl;
  cout<< "************************ ********************** ************************"<<endl; 
   cout<< "************************    FINAL RESULT      ************************"<<endl;
cout<<"                                                               "<<endl;
cout<<"                                                               "<<endl;
  
cout<< "************************ observable transitions   ************************"<<endl;
  for(auto i:obs)
  {
  cout<<"t"<<i.first+1<<endl;
  }
  
  cout<< "************************ observable paths ************************"<<endl;
 }
 for (auto i: chemins)
 {
  cout<< "--- observable path ----"<<endl;
  for (auto tr:i)
  cout<<"t"<<tr+1<<endl;
  }
  
    cout<< "************************  abstract paths ************************"<<endl;
 for (auto i: abstrait)
 {
  cout<< "--- abstract path ----"<<endl;
  for (auto tr:i)
  cout<<"t"<<tr+1<<endl;
  
  }*/
 
 tps = getTime() - d;

   cout << " Temps de construction du graphe d'observation " << tps << endl;
   g.printCompleteInformation();
  cout<< "transi "<<R.transitions.size()<<endl;
  cout<< "etat "<<R.places.size()<<endl;
    cout<< "trans obs "<<obs.size()<<endl;
  for (auto i: abstrait)
 {
  cout<< "--- abstract path ----"<<endl;
  for (auto tr:i)
  cout<<"t"<<tr+1;
  
  }
	return 0;
 }
