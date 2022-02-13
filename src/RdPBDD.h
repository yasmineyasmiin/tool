/* -*- C++ -*- */
#ifndef RdPBDD_H
#define RdPBDD_H
#include <vector>
#include <stack>
#include "Net.h"
#include "bdd.h"
#include"MDGraph.h"
#include"Modular_Obs_Graph.h"
#include "Modular_Class_of_state.h"
//#include"Modular_Obs_Graph.hpp"
/***********************************************/

class Trans {
public:
	Trans(const bdd& var, bddPair* pair, const bdd& rel,const bdd& prerel,const bdd& Precond, const bdd& Postcond);
	bdd operator()(const bdd& op) const;
	bdd operator[](const bdd& op) const;
  friend class RdPBDD;
private:
	bdd var;
	bddPair* pair;
        bdd Precond;
        bdd Postcond;
	bdd rel;
        bdd prerel;
};
typedef set<int> Set;
typedef vector<int> chem;

class RdPBDD {
private :  
  vector<class Transition> transitions;
  Set Observable;
  Set NonObservable;
  map<string,int> transitionName;
   unsigned int Nb_places;
public:
  bdd M0;
  bdd currentvar;
  vector<Trans> relation;
  bdd ReachableBDD1();
  bdd ReachableBDD2();
  bdd ReachableBDD3();
/* Génération de graphes d'observation */
  stack <pair<Class_Of_State*, bdd >>  recherche_points_entree(chem ch,MDGraph& g);
  set<chem> chem_obs(MDGraph& g,map <int,int> tranobs);
  chem chem_abs(chem ch,MDGraph& g );
  vector<int> Sub_path_agregate(bdd *source,bdd cible,Class_Of_State * agr);
 void bdd_firable_obs(Class_Of_State* agr, int t);

  void compute_canonical_deterministic_graph_Opt(MDGraph& g) ;
  bdd Accessible_epsilon(bdd From);
  bdd Accessible_epsilon2(bdd From);
  bdd StepForward(bdd From);
  bdd StepForward2(bdd From);
  bdd StepBackward(bdd From);
  pair<int,bdd>  StepBackward1(bdd From,Class_Of_State * agr ); //ajouté
  bdd StepBackward2(bdd From);
  bdd EmersonLey(bdd S, bool trace);
  Set firable_obs(bdd State);
  bdd get_successor(bdd From,int t);
  void GeneralizedSynchProduct1(Modular_Obs_Graph& Gv, int NbSubnets,RdPBDD* Subnets[] ,int nbbddvar) ;
  bool Set_Div(bdd &S) const;
  bool Set_Bloc(bdd &S) const;
  bdd FrontiereNodes(bdd From) const ;
  bdd FrontiereNodes1(bdd From, int t)  ;//ajouté
  bdd CanonizeR(bdd s, unsigned int i) ;
  RdPBDD(const net&,map <int,int> observables,Set NonObservables,int BOUND=32,bool init=false);

  ~RdPBDD(){};
};
/*____________Structure utiles aux produit synchronisé généralisé de n graphes d'observations ________*/


typedef pair<Modular_Class_Of_State*,bdd*> Couple;
typedef pair<Couple,Set*> StackElt; 
typedef stack<StackElt> Stack;
typedef vector<StackElt> PileInt;
typedef pair<StackElt,PileInt> CanonicRep;
//typedef stack<CanonicRep> STACK;
typedef stack<PileInt> STACK;
int InStack(PileInt,Modular_Class_Of_State*);
bool MemeVecteur(vector<bdd>,vector<bdd>);
bool Inferieur(vector<bdd>,vector<bdd>);

#endif
  
  
