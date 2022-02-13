   // version 0.1
#include <time.h>
#include <iostream>
#include <string>
using namespace std;
#include "bdd.h"
#include "fdd.h"
#include "Net.h"
#include "RdPBDD.h"
int Formula_transitions(const char * f, Set_mot& formula_trans, net Rv) ;
bdd EmersonLey(bdd,net);
double getTime(){
	return (double)clock() / (double)CLOCKS_PER_SEC;
}
int Menu()
{
	int choix;
	cout<<"\t\t______________________________________________"<<endl;
	cout<<"\t\t|                                            |"<<endl;
	cout<<"\t\t|        OBSERVATION GRAPHE TOOL             |"<<endl;
	cout<<"\t\t______________________________________________\n\n"<<endl;
	cout<<"\tConstruction de l'espace d'etats accessible avec OBDDs            : 1\n"<<endl;
	cout<<"\tConstruction du graphe d'observation avec Canonize Dichotomique   : 2\n"<<endl;
	cout<<"\tProduit synchronise a la volee de n graphes d'observations        : 3\n"<<endl;
	cout<<"\tProduit synchronise canonise de n graphes d'observations          : 4\n"<<endl;
	cout<<"\tQuitter l'outil : 0"<<endl;
	cin>>choix;
	return choix;
}


void reordering(int nbvar) {

	// all the variables belong to a main block
	bdd_varblockall();
	// to each fdd variable corresponds a block
	for (int i=0; i<nbvar; i++)
		fdd_intaddvarblock(i, i, BDD_REORDER_FIXED);
			
	bdd_autoreorder(BDD_REORDER_SIFT);

	bdd_reorder_verbose(2);
}


/***********************************************/
int main(int argc, char** argv) {
     int choix;
     do
     {
	choix=Menu();
	switch(choix)
	{
	  case 1 :{
	  if (argc<2)
	     return 0;
	  int b = 32;
	  if (argc>2) b = atoi(argv[argc-1]);
	      net R(argv[1]);
	  RdPBDD DR(R, b);
	  cout << "parser...done" << endl;
	  // no print when garbage collection operates
	  bdd_gbc_hook(NULL);
	  //	reordering(R.places.size()*2);
	  //	bdd_reorder(BDD_REORDER_RANDOM);
	  double d, tps;
	  bdd M2;
	  d = getTime();
	  M2 = DR.ReachableBDD1();
	  tps = getTime() - d;
	   cout << argv[1] << " \n Nb states " << bdd_satcountset(M2,DR.currentvar);
	  cout << " \nNb bdd node :" << bdd_nodecount(M2)<<endl; // nombre de noeuds
	  cout << " Ex Time : " << tps << endl;

	  // if (argc>3)
	  // cout << fddset << M2 << endl;
	  break;
	    }
	case 2:{
	  int b = 32;
	       
	  if(argc<3)
	    return 0;
	  char Obs[100]="";
	  char Int[100]="";
	  if(argc>3)
	    strcpy(Obs,argv[2]);
	  if(argc>4)
	    strcpy(Int,argv[3]);
	  b = atoi(argv[argc-1]);
	  cout<<"Fichier net : "<<argv[1]<<endl;
	  cout<<"Fichier formule : "<<Obs<<endl;
	  cout<<"Fichier Interface : "<<Int<<endl;
	  cout<<"______________________________________\n";
	   net R(argv[1],Obs,Int);
	   //cout<<"______________Apres construction de net________________________\n";
	   //cout<<R<<endl;	  
	   double d,tps;
	  RdPBDD DR(R, b);
	  MDGraph g;
	   // no print when garbage collection operates
	  bdd_gbc_hook(NULL);
	  //	reordering(R.places.size()*2);
	  //	bdd_reorder(BDD_REORDER_RANDOM);
	   d = getTime();
	  DR.compute_canonical_deterministic_graph_Opt(g);
	  tps = getTime() - d;
	  cout << " Temps de construction du graphe d'observation " << tps << endl;
	  g. printCompleteInformation();
	 
	   break;
	}
	case 3  :{       
	  int NbSubnets=(argc-2)/2;
	  cout<<"nombre de sous-réseaux : "<<NbSubnets<<endl;
	  //int toto;
	  //cin>>toto;
	  net **Subnets=new net*[NbSubnets];
	  RdPBDD **BddSubnets=new RdPBDD*[NbSubnets];
	  bdd_init(1000000,100000);
	  for(int i=1,j=0;i<argc-2;i=i+2,j++)
	    {		
	      //cout<<"________ sous-reseau _______________"<<j+1<<endl;
	      Subnets[j]=new net(argv[i],argv[argc-1],argv[i+1]);
	      BddSubnets[j]=new RdPBDD(*Subnets[j],1,true);
	    }
	  //for(int k=0;k<NbSubnets;k++)
	  //{
	  //cout<<*Subnets[k]<<endl;
	  //cin>>toto;
	  //}
	  int NbBddVar=Subnets[0]->nbPlace();
	  //cout<<"NB BDD VAR : "<<NbBddVar<<endl;
	  for(int i=1;i<NbSubnets;i++)
	  {
	    //cout<<"Sous-reseau "<<i<<endl;
	    //cout<<"son nb places : "<<Subnets[i]->nbPlace()<<endl;
	    if(Subnets[i]->nbPlace()>NbBddVar)
	      NbBddVar=Subnets[i]->nbPlace();	
	  }
	  //
	  //cin>>toto;
	  // bdd_setvarnum(NbBddVar);					
	  Modular_Obs_Graph Gv;
	   // no print when garbage collection operates
	  bdd_gbc_hook(NULL);
	  //	reordering(R.places.size()*2);
	  //	bdd_reorder(BDD_REORDER_RANDOM);
	  double TpsInit = (double)(clock()) / CLOCKS_PER_SEC;
	  BddSubnets[0]->GeneralizedSynchProduct1(Gv,NbSubnets,BddSubnets,NbBddVar);
	  double TpsConstruct = ((double)(clock()) / CLOCKS_PER_SEC) - TpsInit;
	  cout << "SYNCHRONIZATION TIME:    " << TpsConstruct<< endl;
	  cout<<"GRAPHE DE LA FORMULE \n";
	  Gv.printCompleteInformation(NbSubnets);
	  bdd_done();
	  break;
	}
      }
    }while(choix);
  return 1;
}
