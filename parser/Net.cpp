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

/*void Node::addInhibitor(int node, int valuation) {
  pair < int, int > x(node, valuation);
  inhibitor.push_back(x);
}

void Node::addPreAuto(int node, int valuation) {
  pair < int, int > x(node, valuation);
  preAuto.push_back(x);
}

void Node::addPostAuto(int node, int valuation) {
  pair < int, int > x(node, valuation);
  postAuto.push_back(x);
}*/
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
  cout << "CREATION D'UN NOUVEAU SOUS-RESEAU \n";
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
  /*if (strlen(Formula_trans) > 0) {
    //    cout<<"transitions de la formule non vide \n";
    Set_Formula_Trans(Formula_trans);
    if (strlen(Int_trans) > 0) {
      Set_Interface_Trans(Int_trans);
      //cout<<"transitions de l'interface non vide \n";
    }
    cout << "______________66666666666666666666666______________________\n";
    set_union(InterfaceTrans.begin(), InterfaceTrans.end(), Formula_Trans.begin(), Formula_Trans.end(), inserter(Observable, Observable.begin()));
    Set_Non_Observables();
  } else
    for (unsigned int i = 0; i < transitions.size(); i++)
      Observable.insert(i);
*/
  cout << "FIN CREATION \n";
}

//////////////////////////////////////////////////////////////////////////////
/*-----------------------------------Caus----------------------------------*/
void net::caus(bool type, int nbn) {
  set < Transition > * f;
  set < Transition > * s;
  f = new set < Transition > ;
  s = new set < Transition > ;
  if (type && !places[nbn].visited) {
    Place p = places[nbn];
    places[nbn].visited = true;

    cout << "ici il traite la place  " << places[nbn].name << endl;

    if (places[nbn].pre.size() == 1 && places[nbn].post.size() == 1) {

      f -> insert(transitions[places[nbn].pre.front().first]);
      cout << "le prem ele" << (transitions[places[nbn].pre.front().first]).name << endl;
      s -> insert(transitions[places[nbn].post.front().first]);
      cout << "le second ele" << (transitions[places[nbn].post.front().first]).name << endl;
      RelaCausal * r = new RelaCausal(f, s, Atom);
      causality.insert(r);
      cout << "on a ajouté une relation atomique atomique  " << endl;

    } else {

      for (unsigned int i = 0; i < places[nbn].pre.size(); i++) {
        f -> insert(transitions[places[nbn].pre.at(i).first]);
      }
      for (unsigned int i = 0; i < places[nbn].post.size(); i++) {
        s -> insert(transitions[places[nbn].post.at(i).first]);
      }
      RelaCausal * r = new RelaCausal(f, s, Disj);
      causality.insert(r);
      cout << "on a ajouté une relation de Disjonction  " << endl;
    }

    for (auto u: places[nbn].post) {
      cout << "le nb de relations cosalite" << causality.size() << endl;
      cout << "/***********************************************" << endl;
      caus(false, u.first);

    }
  }

  if (!type && !transitions[nbn].visited) {
    transitions[nbn].visited = true;
    Transition t = transitions[nbn];

    cout << "ici il traite la transition  " << transitions[nbn].name << endl;
    if (transitions[nbn].post.size() > 1) {
         s->clear() ;
         f->clear() ;
      cout << "les succ. et sont eg " << transitions[nbn].post.size() << endl;
      for (auto u: transitions[nbn].post) {
        cout << "le post est  " << places[u.first].name << endl;
        if (places[u.first].pre.size() == 1 && places[u.first].post.size() == 1) //.u=1 et u.=1
        {
          s -> insert(transitions[places[u.first].post.front().first]);
          cout << "un elmt inseré a s" << endl;
        }

      }
      if (s -> size() > 1) {
        cout << "le size de s " << s -> size() << endl;
        f -> insert(t);
        RelaCausal * r = new RelaCausal(f, s, Conj);
        causality.insert(r);
        cout << "on a ajouté une relation de Conjonction  " << endl;
      }
    }

    if (transitions[nbn].pre.size() > 1) {
        s->clear() ;
         f->clear() ;
      for (auto u: transitions[nbn].pre)

      {
        cout << "pour la place " << places[u.first].name << " le nb de  succ est " << places[u.first].post.size() << " et le nb de pred est " << places[u.first].pre.size() << endl;
        if (places[u.first].pre.size() == 1 && places[u.first].post.size() == 1) //.u=1 et u.=1
        {
          cout << "on va  ajouter son pred a f" << transitions[places[u.first].pre.front().first].name << endl;
          f -> insert(transitions[places[u.first].pre.front().first]);
          cout << "un elmt inseré a f" << endl;
        }

      }
      if (f -> size() > 1) {
        cout << "le nb d elmt de f " << f -> size() << endl;
        s -> insert(t);
        RelaCausal * r = new RelaCausal(f, s, Conj);
        causality.insert(r);
        cout << "on a ajouté une relation de Conjonction  " << endl;
      }

    }

    for (auto u: transitions[nbn].post) {
      if (places[u.first].visited == 0) //.u=1 et u.=1
      {
        cout << "le nb de relations cos" << causality.size() << endl;
        cout << "/***********************************************" << endl;
        caus(true, u.first);

      }
    }
  }
 
}

/*------------------------------------observation------------------------------------------*/
set < ObsNCau *> net::observation(RelaCausal & r) {
  set < ObsNCau * > E;
  if (!r.visited) {
    r.visited = true;
    if (r.relation == Atom || r.relation == Conj) {
      if (r.relation == Atom) {
        cout << "relation Atom" << endl;
      }
      if (r.relation == Conj) {
        cout << "relation Conj" << endl;
        for (auto h:*r.first){
       cout << "les first" <<h.name<< endl;}
        for (auto h:*r.second){
       cout << "les second" <<h.name<< endl;}

        
      }
      for (auto i: * r.second) {
        ObsNCau *o =new ObsNCau();
        o->couvrant = i;
        o->couverts.insert(i);
        for (auto j: * r.first) {
          o->couverts.insert(j);
        }
        E.insert(o);
       cout << "elt insere1 " <<*o<< endl;
        

      }
    }
    if (r.relation == Disj) {
      //cout << "relation de disj" << endl;
      if (r.first -> size() > 1) {
        for (auto i: * (r.first)) {
          ObsNCau *o=new ObsNCau();
          o->couvrant = i;
          o->couverts.insert(i);
          E.insert(o);
          cout << "elt insere2 " <<*o<< endl;
        }
        for (auto i: * (r.second)) {
          ObsNCau *o=new ObsNCau();
          o->couvrant = i;
          o->couverts.insert(i);
          E.insert(o);
          cout << "elt insere3 " <<*o<< endl;
        }
      } else {
        for (auto i: * (r.second)) {
          ObsNCau *o=new ObsNCau();
          o->couvrant = i;
          
          o->couverts.insert(i);
          E.insert(o);
          cout << "elt insere4 " <<*o<< endl;
        }

      }
    }
  }
  return E;

}


/*----------------------------------------Observation causée--------------------------------------------*/
//ObsNCau net::Obs_causee(ObsNCau & S)
void net::Obs_causee(ObsNCau & S) {
  ObsNCau f;
  //cout << "on va faire obs-causee" << endl;
  set < ObsNCau *> obs;
  RelaCausal * c = rech_couple_cause(S.couvrant);
  

  if ((c -> first) -> size() != 0 && !(c -> visited)) {

    
    obs = observation( * c);
    //cout << "le set ObsNCaucalculé de c" << endl;
    for (auto o: obs) {
    //cout << "l'encient couvrant " << S.couvrant.name << endl;
      //cout << "le nouveau couvrant " << o->couvrant.name << endl;
        
       S.couvrant = o->couvrant;

      if (S.couverts.find(S.couvrant)!=S.couverts.end()) {
        S.cycle = true;
        //cout << "il y a un cycle " << endl;

      } else {
       //cout << "pas de cycle" << endl;
        S.couverts.insert(S.couvrant);
        //cout << "on ajoute un couvert " << S.couvrant.name << endl;
      }

    }
   Obs_causee(S);

  }

  // return S;
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
 
 map< int,int > net::calcul() {
 
  int num = 0;
  set < ObsNCau > S, K;
  set < ObsNCau *>H, E;
  map < int,int > obs;
  for (auto i: causality) {
    num++;
    //cout << "****** la relation de cos num " << num << "***************" << endl;
    if (!i -> visited) {
      E = observation( * i);
          //cout << "---cette relation a un nb de ObsNcau egale a " << E.size() << endl;
      for (auto e: E) {
        cout << "---ancien elt de E : " << *e<<endl;
        Obs_causee(*e);
        K.insert(*e);
        cout << "---element ajouté a K : " << *e<<endl;

       
      }
 
    }  
  }
  
  
    cout << "*****************generation de S ************ " << endl;
    cout <<"on a un nombre dans k = "<<K.size()<<endl;
        
    for (auto k:K) {
    bool ex = false;
        cout<<"             "<<endl;
    cout<<" **** ici on traite "<<k<<endl;

    
    
    for (auto  h: H)
    {
        cout<<"on parcourt la liste S  s:  "<<"s="<<*h<<endl;
        if (k.couvrant == h->couvrant)
        {
        if(!h->cycle){h->cycle=k.cycle;}
          cout << "element " << k.couvrant.name << " deja dans S " << endl;
          for (auto m: k.couverts)
          {
            h->couverts.insert(m);
          }
          cout<<" le nouv S ---"<<*h<<endl;
          ex = true;
        }
        else
        {
            if ((h->couverts).find(k.couvrant)!=h->couverts.end())
            {
                cout << "element " << k.couvrant.name << " existe dans S sous l'elmt " <<h->couvrant.name << endl;
                for (auto m: k.couverts) {
                    h->couverts.insert(m);
                }
                ex = true;
                cout<<" le nouv S ---"<<*h<<endl;
            }
            for (auto i: k.couverts)
                {
                    if (i == h->couvrant )
                    {
                        cout << "l'element " << k.couvrant.name << " couvre " <<  i.name << " deja dans S " << endl;
                        ex = true;
                        h->couvrant = k.couvrant;
                        for (auto m: k.couverts) {
                            h->couverts.insert(m);
                        }
                    cout<<" le nouv S ---"<<*h<<endl;
                        break;
                    }
                }
            
          
        }
    }
    if (!ex) {
            cout<<"otherwise"<<endl;
            ObsNCau *t= new ObsNCau(k.couvrant,k.couverts,k.cycle);
            H.insert(t);
            cout<<"---"<<*t<<endl;
          }
    }
   
  
  for (auto s: H) {
    if (s->cycle )     
        {
        cout<<"il y a cycle"<<endl;
        obs [transitionName.find(s->couvrant.name)->second]=2;
        } 
        else{obs[transitionName.find(s->couvrant.name)->second]=1;} 
      }
  return obs;
}
/*---------------------------------Init Set of  transitions ------------------------------*/
/*---------------------------------Set_formula_trans()------------------*/
/*bool net::Set_Formula_Trans(const char * f) {
  FILE * in ;
  int i, nb;
  //	cout<<"ici set formula transitions \n";
  int pos_trans(TRANSITIONS, string);
  char Buff[TAILLEBUFF], * z; in = fopen(f, "r");
  if ( in == NULL) {
    cout << "file " << f << " doesn't exist" << endl;
    exit(1);
  }
  int nb_formula_trans;
  fscanf( in , "%d\n", & nb_formula_trans);
  nb = fread(Buff, 1, TAILLEBUFF - 1, in );
  Buff[nb] = '\0';
  z = strtok(Buff, " \t\n");
  cout << "taille " << TAILLEBUFF << " buff " << Buff << " z: " << z << endl;
  for (i = 0; i < nb_formula_trans; i++) {
    cout << " z: " << z << endl;
    if (z == NULL) {
      cout << "error in formula trans format " << endl;
      return false;
    }
    string tmp(z);
    int pos = pos_trans(transitions, tmp);
    if (pos == -1) {
      cout << z << "    Error??? : observale transition " << tmp << " doesn't exist \n";
      //return false;		  
    } else
      Formula_Trans.insert(pos);
    /*cout<<"insertion de :"<<transitions[pos].name<<endl;*/
    /*z = strtok(NULL, " \t\n");
    if (z == NULL) {
      nb = fread(Buff, 1, TAILLEBUFF - 1, in );
      Buff[nb] = '\0';
      z = strtok(Buff, " \t\n");
    }
  }
  fclose( in );
  return true;
}*/
/*---------------------------------Set_Interface_trans()------------------*/
/*bool net::Set_Interface_Trans(const char * f) {
  FILE * in ;
  int i, nb;
  int pos_trans(TRANSITIONS, string);
  char Buff[TAILLEBUFF], * z; in = fopen(f, "r");
  if ( in == NULL) {
    cout << "file " << f << " doesn't exist" << endl;
    exit(1);
  }
  int int_trans;
  fscanf( in , "%d\n", & int_trans);
  nb = fread(Buff, 1, TAILLEBUFF - 1, in );
  Buff[nb] = '\0';
  z = strtok(Buff, " \t\n");
  cout << "taille " << TAILLEBUFF << " buff " << Buff << " z: " << z << endl;
  for (i = 0; i < int_trans; i++) {
    cout << " z: " << z << endl;
    if (z == NULL) {
      cout << "error in interface format " << endl;
      return false;
    }
    string tmp(z);
    int pos = pos_trans(transitions, tmp);
    //if(Formula_Trans.find(pos)==Formula_Trans.end())
    if (pos == -1) {
      cout << z << "         Error??? : interface transition doesn't exist \n";
      //	return false;
    } else
      InterfaceTrans.insert(pos);
    z = strtok(NULL, " \t\n");
    if (z == NULL) {
      nb = fread(Buff, 1, TAILLEBUFF - 1, in );
      Buff[nb] = '\0';
      z = strtok(Buff, " \t\n");
    }
  }
  fclose( in );
  return true;
}*/
/*---------------------------------Set_Non_Observables()------------------*/
/*void net::Set_Non_Observables() {
  NonObservable.clear();
  for (unsigned int i = 0; i < transitions.size(); i++)
    if (Observable.find(i) == Observable.end()) {

      NonObservable.insert(i);
    }
}*/
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

/*bool net::addInhibitor(const string & place,
  const string & trans, int valuation) {
  int p, t;
  map < string, int > ::const_iterator pi = placeName.find(place);
  if (pi == placeName.end())
    return false;
  else
    p = pi -> second;
  map < string, int > ::const_iterator ti = transitionName.find(trans);
  if (ti == transitionName.end())
    return false;
  else
    t = ti -> second;
  transitions[t].addInhibitor(p, valuation);
  places[p].addInhibitor(t, valuation);
  return true;
}

bool net::addPreAuto(const string & place,
  const string & trans,
    const string & valuation) {
  int p, t, v;
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
  map < string, int > ::const_iterator pv = placeName.find(valuation);
  if (pv == placeName.end() || places[pv -> second].isQueue())
    return false;
  else
    v = pv -> second;
  transitions[t].addPreAuto(p, v);
  places[p].addPostAuto(t, v);
  return true;
}

bool net::addPostAuto(const string & place,
  const string & trans,
    const string & valuation) {
  int p, t, v;
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
  map < string, int > ::const_iterator pv = placeName.find(valuation);
  if (pv == placeName.end() || places[pi -> second].isQueue())
    return false;
  else
    v = pv -> second;
  transitions[t].addPostAuto(p, v);
  places[p].addPreAuto(t, v);
  return true;
}

bool net::addReset(const string & place,
  const string & trans) {
  int p, t;
  map < string, int > ::const_iterator pi = placeName.find(place);
  if (pi == placeName.end())
    return false;
  else
    p = pi -> second;
  map < string, int > ::const_iterator ti = transitionName.find(trans);
  if (ti == transitionName.end())
    return false;
  else
    t = ti -> second;
  transitions[t].addReset(p);
  places[p].addReset(t);
  return true;
}*/

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
 /* os << "********** transitions  de la formule  **********" << endl;
  for (Set::const_iterator h = R.Formula_Trans.begin(); !(h == R.Formula_Trans.end()); h++)
    cout << R.transitions[ * h].name << endl;
  os << "********** transitions  de l'interface  **********" << endl;
  for (Set::const_iterator h = R.InterfaceTrans.begin(); !(h == R.InterfaceTrans.end()); h++)
    cout << R.transitions[ * h].name << endl;
  os << "Nombre de transitions observable:" << R.Observable.size() << endl;
  os << "********** transitions observables **********" << endl;
  for (Set::const_iterator h = R.Observable.begin(); !(h == R.Observable.end()); h++)
    cout << R.transitions[ * h].name << endl;
  os << "Nombre de transitions non observees:" << R.NonObservable.size() << endl;
  os << "********** transitions  non observees **********" << endl;
  for (Set::const_iterator h = R.NonObservable.begin(); !(h == R.NonObservable.end()); h++)
    cout << R.transitions[ * h].name << endl;
  i = 0;*/
  os << "Nombre global de transitions :" << R.nbTransition() << endl;
  os << "********** transitions  **********" << endl;
  for (TRANSITIONS::const_iterator t = R.transitions.begin(); t != R.transitions.end(); t++, i++) {
    os << setw(4) << i << ":" << t -> name << endl;

  }
  return (os);
}

