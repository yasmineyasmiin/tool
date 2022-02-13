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
  void addPre(int,int);
  void addPost(int,int);
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
 
public:
  /* Attributs */ 
  vector<class Place> places;
  vector<class Transition> transitions;
  map<string,int> placeName;
  map<string,int> transitionName;
  set < RelaCausal*> causality;
 

  /* Constructors */  
  net(){};
  ~net(){};
  net(const char *file,const char*Obs="", const char* Int="");
  /* Monteur */

  //void caus(bool type=false,int nbn=0);
  //set<ObsNCau*> observation(RelaCausal &r);
  //void Obs_causee(ObsNCau & s);
  //map<int,int>calcul();
  set<int> calcul1();//mathode jorg
  RelaCausal* rech_couple_cause(Transition t);
  bool addPlace(const string &place,int marking=0, int capacity=0);
  bool addQueue(const string &place,int capacity=0);
  bool addLossQueue(const string &place,int capacity=0);
  bool addTrans(const string &transition);
  bool addPre(const string &place,const string &transition,int valuation=1);
  bool addPost(const string &place,const string &transition,int valuation=1);  
  bool addPreQueue(const string &place,const string &transition,int valuation=1);
  bool addPostQueue(const string &place,const string &transition,int valuation=1);  
 
  /* Visualisation */
  int nbPlace() const {return places.size();};
  int nbTransition() const {return transitions.size();};
};

ostream& operator<<(ostream &,const net &);

#endif
