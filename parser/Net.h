/* -*- C++ -*- */
#ifndef NET_H
#define NET_H
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <cstddef>
#include <map>
#include <ext/hash_map>
#include "RdPMonteur.h"
#include <cstring>
typedef set<int> Set;
class Node{

public:
  Node(){};
   bool visited=false;
    string name;
  ~Node(){};
  vector< pair<int,int> > pre, post /*,inhibitor, preAuto, postAuto*/;
  //vector<int> reset;
  void addPre(int,int);
  void addPost(int,int);
 // void addInhibitor(int,int);
  //void addPreAuto(int,int);
 // void addPostAuto(int,int);
  //void addReset(int);
    bool operator==(const Node &) const;
    bool operator<(const Node & t)const;
};

class Place:public Node{
public:
  int marking, capacity;
  Place();
    Place(const string &p,int m=0, int c=0):marking(m),capacity(c){name=p;};
  ~Place(){};
  bool isLossQueue() const {return marking==-2;}
  bool isQueue() const {return marking<=-1;}
  bool hasCapacity() const {return capacity!=0;}
};

class Transition:public Node{
public:
  Transition(){};
  Transition(const string &t){name=t;};
   ~Transition(){};


};
/*-----------------------------------------------------------------*/
/*struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1,s2)<0;
	}
};
typedef set<const char*,ltstr> Set_mot;*/
//typedef vector<Place> PLACES;
typedef vector<Transition> TRANSITIONS;
enum Relation  { Atom, Disj, Conj};

/////////////////////////RelaCausal/////////////
 class RelaCausal
{
  public:
    set<Transition> *first ;           
    set<Transition> *second ;          
    Relation relation ; 
    bool visited ;
    RelaCausal();
    bool operator<(RelaCausal const& rhs)const;
    RelaCausal(set<Transition> *f, set<Transition> *s ,Relation r , bool b= false);

  };

//////////////////ObsNCau/////////////////
    
class ObsNCau 
{
public:
    Transition couvrant ;
    set<Transition> couverts ;
    bool cycle;
    ObsNCau(Transition t,set<Transition>c,bool cy=0);
    ObsNCau(){};
    bool operator<(ObsNCau const& rhs) const;
    friend ostream& operator<<(ostream &,const ObsNCau &);
};

 /////////////////////Net////////
class net: public RdPMonteur {
private:
 /*Initialisation des attributs*/
  //bool Set_Observables(const char* file) ;
 // bool Set_Interface_Trans(const char* file) ;
  //bool Set_Formula_Trans(const char* file) ;
 // bool Set_ObsNonObservables(Set_mot obs) ;
  //void Set_Non_Observables();
public:
  /* Attributs */ 
  vector<class Place> places;
  vector<class Transition> transitions;
  map<string,int> placeName;
  map<string,int> transitionName;
 //Set Observable;
 //Set NonObservable;
//  Set InterfaceTrans;
  //Set Formula_Trans;
  set < RelaCausal*> causality;
 

  /* Constructors */  
  net(){};
  ~net(){};
  net(const char *file,const char*Obs="", const char* Int="");
  /* Monteur */

  void caus(bool type=false,int nbn=0);
  set<ObsNCau*> observation(RelaCausal &r);
  void Obs_causee(ObsNCau & s);
  map<int,int>calcul();
  RelaCausal* rech_couple_cause(Transition t);
  bool addPlace(const string &place,int marking=0, int capacity=0);
  bool addQueue(const string &place,int capacity=0);
  bool addLossQueue(const string &place,int capacity=0);
  bool addTrans(const string &transition);
  bool addPre(const string &place,const string &transition,int valuation=1);
  bool addPost(const string &place,const string &transition,int valuation=1);  
  bool addPreQueue(const string &place,const string &transition,int valuation=1);
  bool addPostQueue(const string &place,const string &transition,int valuation=1);  
 // bool addInhibitor(const string &place,const string &transition,int valuation=1);
  //bool addPreAuto(const string &place,const string &transition,const string &valuation);
 // bool addPostAuto(const string &place,const string &transition,const string &valuation);  
  //bool addReset(const string &place,const string &transition);
  /* Visualisation */
  int nbPlace() const {return places.size();};
  int nbTransition() const {return transitions.size();};
};

ostream& operator<<(ostream &,const net &);

#endif
