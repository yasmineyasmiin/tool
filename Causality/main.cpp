#include <time.h>
#include <iostream>
#include <string>


#include <iomanip>

#include <map>
#include <ext/hash_map>
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
    




R.caus(false,0);// remplir causality

 for (auto i :R.causality){
      cout<<*i<<endl;
}



map<int,int> obs;
set<int> unobs;
unobs=R.calcul1();

for (int i = 0; i < R.transitions.size(); i++ )
    {
         if ((unobs.find(i))== unobs.end())
         {
         obs.insert({i,1}); 
         }  
    }	
	

 

  double d,tps;  
  int b = 32;
  RdPBDD DR(R,obs,unobs, b);
  MDGraph g;
 d = getTime();
set<vector<int>> chemins= DR.chem_obs(g,obs);
   cout << " n de chemins " << chemins.size() << endl;
    for (auto i: chemins)
    {   cout << " le chemin : "  << endl;
        for (auto k:i){
            cout <<"t"<< k+1  << endl;
            }
    }

  g.printCompleteInformation();
  tps = getTime() - d;
   cout << " Temps de construction du graphe d'observation " << tps << endl;

for(auto i:g.GONodes)
{
cout<<"le noeud num "<<i->class_state.id()<<" a comme succ: "<<endl;
    for(auto k: i->Successors){
        cout<<"**t"<<k.second+1<<" jusqu'a "<<k.first->class_state.id()<<endl;
        }
                cout<<"**"<<endl;
}
  
 
 

  set <vector<int>> abstrait;
 for (auto i: chemins)
 {
  cout<< "---un nouv chem----"<<endl;
  vector<int> chem_abs;
  chem_abs= DR.chem_abs(i,g);
  
  abstrait.insert(chem_abs); 
  
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
  
  }
 
 
 
	return 0;
 }
