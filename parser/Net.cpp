#include <string>

#include <iostream>

#include <iomanip>

#include <vector>

#include <map>

#include <ext/hash_map>

#include <algorithm>

#include "Net.h"

#define TAILLEBUFF 500
/***********************************************************/
/*                      class Node                         */
/***********************************************************/

void Node::addPre(int node, int valuation) {
  pair < int, int > x(node, valuation);
  pre.push_back(x);
}

void Node::addPost(int node, int valuation) {
  pair < int, int > x(node, valuation);
  post.push_back(x);
}


bool Node::operator == (const Node & n) const {
  return n.name == name;
}
/*void Node::addReset(int node) {
  reset.push_back(node);
}*/
///////////////////////////////////////////////////////operator<//////////

bool Node::operator < (const Node& t) const {
  return t.name < name;
}


bool RelaCausal::operator < (RelaCausal
  const & rhs) const {
  bool res = ( * (rhs.first) == * first) && ( * (rhs.second) == * second);
  cout << "ici < de RelaCausal donne " << res << endl;
  return ( * (rhs.first) == * first && * (rhs.second) == * second && rhs.relation == relation);
}

bool ObsNCau::operator < (ObsNCau
  const & rhs) const {
  return  rhs.couvrant < couvrant || (rhs.couvrant == couvrant && rhs.couverts < couverts);
  //cout << "ici < de ObNCau donne " << res << endl;
  //return (rhs.couvrant == couvrant && rhs.couverts == couverts);
}


/***********************************************************/
/*                      class RelaCausal                   */
/***********************************************************/

RelaCausal::RelaCausal(set < Transition > * f, set < Transition > * s, Relation r, bool b) {
  first = f;
  second = s;
  relation = r;
  visited = b;
}

/***********************************************************/
/*                      class ObsNCau                   */
/***********************************************************/
ObsNCau::ObsNCau(Transition t, set < Transition > c, bool cy) {
  couvrant = t;
  couverts = c;
  cycle = cy;
}
/***********************************************************/
/*                      class RdPE                         */
/***********************************************************/
net::net(const char * f,
  const char * Formula_trans,
    const char * Int_trans) {
  //cout << "CREATION D'UN NOUVEAU SOUS-RESEAU \n";
  if (create(f)) {
    for (vector < class Place > ::iterator p = places.begin(); p != places.end(); p++) {
      sort(p -> pre.begin(), p -> pre.end());
      sort(p -> post.begin(), p -> post.end());
    }
    for (vector < class Transition > ::iterator p = transitions.begin(); p != transitions.end(); p++) {
      sort(p -> pre.begin(), p -> pre.end());
      sort(p -> post.begin(), p -> post.end());
    }
  } else {
    places.clear();
    transitions.clear();
    placeName.clear();
    transitionName.clear();
  }

  //cout << "FIN CREATION \n";
}


/*---------------------------------------------rech_couple_cause---------------------------------------*/
RelaCausal * net::rech_couple_cause(Transition t) {
  for (auto  c: causality) {
    if ((c -> visited) == false && c -> relation == Atom && (c -> first) -> size() == 1 && (*(c -> first)).find(t)!=(*(c -> first)).end())

    {
     cout << "la relation de caus trouvé " << endl;

      return c;

    }
  }
  set < Transition > * f;
  set < Transition > * s;
  f = new set < Transition > ;
  s = new set < Transition > ;
  RelaCausal * r = new RelaCausal(f, s, Atom);
  return r;

}

/*------------------------------calcul l’ensemble d’observation-----------------------------------*/
set<int> net::calcul1() {
  set<int> unobs;
  int l=transitions.size(); 
  for(int i=0;i<l;i++)
    {
    Transition t =transitions[i];
         t.visited = true;
         if (t.post.size() >= 1) 
            {
                for (auto p : t.post)
                {
                    if ((places[p.first]).pre.size()==1 && (places[p.first]).marking==0) 
                    {
                         unobs.insert(i);
                    }
                }
            }  
    }
    

  return unobs ;
}
    

 

/*-----------------------pos_trans()--------------------*/
int pos_trans(TRANSITIONS T, string trans) {
  int pos = 0;
  //	cout<<"on cherche "<<trans<<" dans :\n";
  for (TRANSITIONS::const_iterator i = T.begin(); !(i == T.end()); i++, pos++) {
    //  cout<<i->name<<"   ";
    if (i -> name == trans)
      return pos;
  }
  //cout<<"Non trouve :\n";
  return -1;
}
    
    ostream& operator<<(ostream &os,const ObsNCau & onc)
    {
    os<<onc.couvrant.name<<"--->{";
    for(auto t:onc.couverts)
        os<<t.name<<", ";
    os<<"}"<<endl;
    return os;
    }
    
     ostream& operator<<(ostream &os,const RelaCausal & onc)
    {
    os<<"Les first :{";
        for(auto t:*onc.first)
        os<<t.name<<", ";
    os<<"}"<<endl;
    os<<"Les second :{";
    for(auto t:*onc.second)
        os<<t.name<<", ";
    os<<"}"<<endl;
    os<<" la relation: "<<onc.relation<<endl;
    return os;
    }
    
    
    
    
    
    
/* ------------------------------ operator<< -------------------------------- */
ostream & operator << (ostream & os,  const Set & s) {
  bool b = false;

  if (s.size()) {
    for (Set::const_iterator i = s.begin(); !(i == s.end()); i++) {
      if (b)
        os << ", ";
      else
        os << "{";
      os << * i << " ";
      b = true;
    }
    os << "}";
  } else
    os << "empty set";
  return os;

}
/*----------------------------------------------------------------------*/
bool net::addPlace(const string & place, int marking, int capacity) {
  map < string, int > ::const_iterator pi = placeName.find(place);
  if (pi == placeName.end()) {
    placeName[place] = places.size();
    Place p(place, marking, capacity);
    places.push_back(p);
    return true;
  } else
    return false;
}

bool net::addQueue(const string & place, int capacity) {
  map < string, int > ::const_iterator pi = placeName.find(place);
  if (pi == placeName.end()) {
    placeName[place] = places.size();
    Place p(place, -1, capacity);
    places.push_back(p);
    return true;
  } else
    return false;
}

bool net::addLossQueue(const string & place, int capacity) {
  map < string, int > ::const_iterator pi = placeName.find(place);
  if (pi == placeName.end()) {
    placeName[place] = places.size();
    Place p(place, -2, capacity);
    places.push_back(p);
    return true;
  } else
    return false;
}

bool net::addTrans(const string & trans) {
  map < string, int > ::const_iterator ti = transitionName.find(trans);
  if (ti == transitionName.end()) {
    transitionName[trans] = transitions.size();
    Transition t(trans);
    transitions.push_back(t);
    return true;
  } else
    return false;
}

bool net::addPre(const string & place,
  const string & trans, int valuation) {
  int p, t;
  map < string, int > ::const_iterator pi = placeName.find(place);
  if (pi == placeName.end() || places[pi -> second].isQueue())
    return false;
  else
    p = pi -> second;
  map < string, int > ::const_iterator ti = transitionName.find(trans);
  if (ti == transitionName.end())
    return false;
  else
    t = ti -> second;
  transitions[t].addPre(p, valuation);
  places[p].addPost(t, valuation);
  return true;
}

bool net::addPost(const string & place,
  const string & trans, int valuation) {
  int p, t;
  map < string, int > ::const_iterator pi = placeName.find(place);
  if (pi == placeName.end() || places[pi -> second].isQueue())
    return false;
  else
    p = pi -> second;
  map < string, int > ::const_iterator ti = transitionName.find(trans);
  if (ti == transitionName.end())
    return false;
  else
    t = ti -> second;
  transitions[t].addPost(p, valuation);
  places[p].addPre(t, valuation);
  return true;
}

bool net::addPreQueue(const string & place,
  const string & trans, int valuation) {
  int p, t;
  map < string, int > ::const_iterator pi = placeName.find(place);
  if (pi == placeName.end() || !places[pi -> second].isQueue())
    return false;
  else
    p = pi -> second;
  map < string, int > ::const_iterator ti = transitionName.find(trans);
  if (ti == transitionName.end())
    return false;
  else
    t = ti -> second;
  transitions[t].addPre(p, valuation);
  places[p].addPost(t, valuation);
  return true;
}

bool net::addPostQueue(const string & place,
  const string & trans, int valuation) {
  int p, t;
  map < string, int > ::const_iterator pi = placeName.find(place);
  if (pi == placeName.end() || !places[pi -> second].isQueue())
    return false;
  else
    p = pi -> second;
  map < string, int > ::const_iterator ti = transitionName.find(trans);
  if (ti == transitionName.end())
    return false;
  else
    t = ti -> second;
  transitions[t].addPost(p, valuation);
  places[p].addPre(t, valuation);
  return true;
}



/* Visualisation */
ostream & operator << (ostream & os,
  const net & R) {

  /* affichage nombre de places et de transitions */
  os << "***************************" << endl;
  os << "Nombre de places     :" << R.nbPlace() << endl;
  os << "Nombre de transitions:" << R.nbTransition() << endl;

  /* affichage de la liste des places */
  os << "********** places **********" << endl;
  int i = 0;
  for (vector < class Place > ::const_iterator p = R.places.begin(); p != R.places.end(); p++, i++) {
    if (p -> isQueue()) {
      os << "queue " << setw(4) << i << ":" << p -> name << ", cp(" << p -> capacity << ")";
      if (p -> isLossQueue())
        cout << " loss";
      cout << endl;
    } else
      os << "place " << setw(4) << i << ":" << p -> name << ":" << p -> marking << " <..>, cp(" << p -> capacity << ")" << endl;
  }

  os << "Nombre global de transitions :" << R.nbTransition() << endl;
  os << "********** transitions  **********" << endl;
  for (TRANSITIONS::const_iterator t = R.transitions.begin(); t != R.transitions.end(); t++, i++) {
    os << setw(4) << i << ":" << t -> name << endl;

  }
  return (os);
}


