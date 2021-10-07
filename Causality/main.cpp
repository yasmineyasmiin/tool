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

//map<int,int> transobs=R.calcul();/*----------------pour tester avec les transitions observées 1,7,8,10-----------------*/

set<int> trans_nonobs;
set<int> cycle;
/*----------------------------------pour tester avec les transitions observées 7,8,10,11-----------------*/
map <int,int> transobs;
transobs[6]=1;
transobs[7]=1;
transobs[9]=1;
transobs[2]=2;

 for (int i = 0; i < R.transitions.size(); i++ ){
 if ((transobs.find(i))== transobs.end()){
 trans_nonobs.insert(i);
  
 }
 }  
 

 



for (auto i :transobs ){
  cout<<"la trans t"<<i.first +1<<" nb occurence "<<i.second<<endl; 
}
  
  for (auto i :trans_nonobs ){
  cout<<"le num de trans non obs est t"<<i+1<<endl; 
} 
 
  double d,tps;  
  int b = 32;
  RdPBDD DR(R,transobs,trans_nonobs, b);
  MDGraph g;
 d = getTime();
set<vector<int>> chemins= DR.chem_obs(g,transobs);
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

/*for(auto i:g.GONodes)
{cout<<"le noeud num "<<i->class_state.id()<<" a comme succ: "<<endl;
for(auto k: i->Successors){
        cout<<"**t"<<k.second+1<<endl;
        }
                cout<<"**"<<endl;
}*/
   cout << " **********************on construit les chemins abstraits ************************* " << endl;
/* stack <pair<Class_Of_State*, bdd >> k;
 for (auto i: chemins)
 {
 cout<< "---un nouv chem----"<<endl;
 k=DR.recherche_points_entree(i, g);
 }
 set <vector<int>> abstrait;
 for (auto i: chemins)
 {
  cout<< "---un nouv chem----"<<endl;
    vector<int> chem_abs;
    chem_abs= DR.chem_abs(i,g);
    abstrait.insert(chem_abs);
    
 }*/
	return 0;
 }
