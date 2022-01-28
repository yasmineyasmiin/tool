/* -*- C++ -*- */
#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include "Net.h"
#include <stack>
#include <ext/hash_map>
using namespace std;
#include <math.h>
#include "bvec.h"
#include "RdPBDD.h"
int NbIt;
int itext,itint;
int MaxIntBdd;
bdd *TabMeta;
int nbmetastate;
double old_size;
const vector<class Place> *vplaces = NULL;
void my_error_handler(int errcode) {
    cout<<"errcode = "<<errcode<<endl;
	if (errcode == BDD_RANGE) {
		// Value out of range : increase the size of the variables...
		// but which one???
		bdd_default_errhandler(errcode);
	}
	else {
		bdd_default_errhandler(errcode);
	}
}

/*****************************************************************/
/*                         printhandler                          */
/*****************************************************************/
void printhandler(ostream &o, int var)
{
	o << (*vplaces)[var/2].name;
	if (var%2)
		o << "_p";
}
 
/*****************************************************************/
/*                         class Trans                           */
/*****************************************************************/
Trans::Trans(const bdd& v, bddPair* p, const bdd& r,const bdd& pr, const bdd& pre, const bdd& post): var(v),pair(p),rel(r),prerel(pr),Precond(pre),Postcond(post) {
}
//Franchissement avant
bdd Trans::operator()(const bdd& n) const {
	bdd res = bdd_relprod(n,rel,var);
	res = bdd_replace(res, pair);
	return res;
}
//Franchissement arrière
bdd Trans::operator[](const bdd& n) const {
	bdd res = bdd_relprod(n,prerel,var);
	res = bdd_replace(res, pair);
	return res;
}

/*****************************************************************/
/*                         class RdPBDD                          */
/*****************************************************************/

RdPBDD::RdPBDD(const net &R, map <int,int> observables,Set NonObservables, int BOUND,bool init){
    int nbPlaces=R.places.size(), i, domain;
	vector<Place>::const_iterator p;
	
	bvec *v = new bvec[nbPlaces];
	bvec *vp = new bvec[nbPlaces];
	bvec *prec = new bvec[nbPlaces];
	bvec *postc = new bvec[nbPlaces];
	int *idv = new int[nbPlaces];
	int *idvp = new int[nbPlaces];
	int *nbbit = new int[nbPlaces];
	if(!init)
	  bdd_init(1000000,1000000);
	// the error handler
	bdd_error_hook((bddinthandler)my_error_handler);
	//_______________
	transitions=R.transitions;
	for ( auto i:observables){Observable.insert(i.first);};
	//Observable=Observables;
	NonObservable=NonObservables;
	transitionName=R.transitionName;
	Nb_places=R.places.size();
	cout<<"Nombre de places : "<<Nb_places<<endl; 
	cout<<"Derniere place : "<<R.places[Nb_places-1].name<<endl; 
  /* place domain, place bvect, place initial marking and place name */
	// domains	
	i=0;
	for(i=0,p=R.places.begin();p!=R.places.end();i++,p++) {
		if (p->hasCapacity()) {
			domain = p->capacity+1;
		}
		else {
			domain = BOUND+1; // the default domain
		}
		// variables are created one by one (implying contigue binary variables)
		fdd_extdomain(&domain, 1);
		//cout<<"nb created var : "<<bdd_varnum()<<endl;
		fdd_extdomain(&domain, 1);
		//cout<<"nb created var : "<<bdd_varnum()<<endl;
    }

	// bvec
	currentvar = bdd_true();
	for(i=0;i<nbPlaces;i++) {
		nbbit[i] = fdd_varnum(2*i);
		//cout<<"nb var pour "<<2*i<<" = "<<fdd_domainsize(2*i)<<endl;
		v[i]=bvec_varfdd(2*i);
		vp[i]=bvec_varfdd(2*i+1);
		//cout<<"nb var pour "<<2*i+1<<" = "<<fdd_domainsize(2*i+1)<<endl;
		currentvar = currentvar & fdd_ithset(2*i);
	}

	// initial marking 
    M0=bdd_true();
	for(i=0,p=R.places.begin();p!=R.places.end();i++,p++)
		M0 = M0 & fdd_ithvar(2*i,p->marking);

	// place names
	vplaces = &R.places;
	fdd_strm_hook(printhandler);


  /* Transition relation */
    for(vector<Transition>::const_iterator t=R.transitions.begin();
	t!=R.transitions.end();t++){
		int np=0;
		bdd rel = bdd_true(), var = bdd_true(),prerel=bdd_true();
		bdd Precond=bdd_true(),Postcond=bdd_true();
		bddPair *p=bdd_newpair();
		
		for(i=0;i<nbPlaces;i++) {
			prec[i]=bvec_con(nbbit[i],0);
			postc[i]=bvec_con(nbbit[i],0);
		}
		set<int> adjacentPlace;
		
		// arcs pre
		for(vector< pair<int,int> >::const_iterator it=t->pre.begin();it!=t->pre.end(); it++) {
			adjacentPlace.insert(it->first);
			prec[it->first] = prec[it->first] + bvec_con(nbbit[it->first], it->second);
		}
		// arcs post
		for(vector< pair<int,int> >::const_iterator it=t->post.begin();it!=t->post.end(); it++) {
			adjacentPlace.insert(it->first);
			postc[it->first] = postc[it->first] + bvec_con(nbbit[it->first], it->second);
		}
		
		np=0;
		for(set<int>::const_iterator it=adjacentPlace.begin();it!=adjacentPlace.end(); it++) {
			idv[np]=2*(*it);
			idvp[np]=2*(*it)+1;
			var = var & fdd_ithset(2*(*it));
			//Image
			// precondition
			rel = rel & (v[*it] >= prec[*it]);
			Precond = Precond & (v[*it] >= prec[*it]);
			// postcondition
			rel = rel & (vp[*it] == (v[*it] - prec[*it] + postc[*it]));
			// Pre image __________
			// precondition
			prerel = prerel & (v[*it] >= postc[*it]);
			// postcondition
			Postcond=Postcond & (v[*it]>=postc[*it]);
			prerel = prerel & (vp[*it] == (v[*it] - postc[*it] + prec[*it]));
			//___________________
			// capacité
			if (R.places[*it].hasCapacity())
				rel = rel & (vp[*it] <= bvec_con(nbbit[*it], R.places[*it].capacity));
			np++;
		}
		fdd_setpairs(p, idvp, idv, np);
		relation.push_back(Trans(var, p, rel,prerel,Precond,Postcond));
    }
	delete [] v;
	delete [] vp;
	delete [] prec;
	delete [] postc;
	delete [] idv;
	delete [] idvp;
	delete [] nbbit;
}




/*----------------------------------- Reachability space using bdd ----------*/
bdd RdPBDD:: ReachableBDD1()
{
  bdd M1;
  bdd M2=M0;
  double d,tps;
  d=(double)clock() / (double)CLOCKS_PER_SEC;
  NbIt=0;
  MaxIntBdd=bdd_nodecount(M0);
  while(M1!=M2){
		M1=M2; 	
		for(vector<Trans>::const_iterator i = relation.begin();
				i!=relation.end();i++) {
			bdd succ = (*i)(M2);
			M2=succ|M2;
			//cout << bdd_nodecount(succ) << endl;
			//if(succ!=bddfalse)
		}
		NbIt++;
		int Current_size=bdd_nodecount(M2);
		if(MaxIntBdd<Current_size)
		  MaxIntBdd=Current_size;
		//cout<<"Iteration numero "<<NbIt<<" nb node de reached :"<<bdd_nodecount(M2)<<endl;		
//		cout << bdd_nodecount(M2) << endl;
	}
	cout << endl;
	tps = ((double)(clock()) / CLOCKS_PER_SEC) - d;
	cout<<"-----------------------------------------------------\n";
	cout << "CONSTRUCTION TIME  " << tps << endl;
	cout<<"Max Intermediary BDD "<<MaxIntBdd<<endl;
	cout<<"Nombre d'iteration : "<<NbIt<<endl;
	//bdd Cycle=EmersonLey(M2,0);
	//cout<<Cycle.id()<<endl;
	return M2;
}
bdd RdPBDD:: ReachableBDD2()
{
    bdd M1;
  bdd M2=M0;
  double d,tps;
  d=(double)clock() / (double)CLOCKS_PER_SEC;
  NbIt=0;
  MaxIntBdd=bdd_nodecount(M0);
  while(M1!=M2){
		M1=M2; 	
		bdd Reached;
		for(vector<Trans>::const_iterator i = relation.begin();
				i!=relation.end();i++) {
			bdd succ = (*i)(M2);
			Reached=succ|Reached;
			//cout << bdd_nodecount(succ) << endl;
			//if(succ!=bddfalse)
		}
		NbIt++;
		M2=M2|Reached;
		int Current_size=bdd_nodecount(M2);
		if(MaxIntBdd<Current_size)
		  MaxIntBdd=Current_size;
		//cout<<"Iteration numero "<<NbIt<<" nb node de reached :"<<bdd_nodecount(M2)<<endl;		
//		cout << bdd_nodecount(M2) << endl;
	}
	cout << endl;
	tps = ((double)(clock()) / CLOCKS_PER_SEC) - d;
	cout<<"-----------------------------------------------------\n";
	cout << "CONSTRUCTION TIME  " << tps << endl;
	cout<<"Max Intermediary BDD "<<MaxIntBdd<<endl;
	cout<<"Nombre d'iteration : "<<NbIt<<endl;
	return M2;
}
bdd RdPBDD:: ReachableBDD3()
{
  double d,tps;
  d=(double)clock() / (double)CLOCKS_PER_SEC;
	bdd New,Reached,From;
	Reached=From=M0;
	NbIt=0;
	do{    
	  NbIt++;
	     bdd succ;		
	     for(vector<Trans>::const_iterator i = relation.begin();	   	i!=relation.end();i++) 
		 succ=(*i)(From)|succ;
  	    New=succ-Reached;
	    From=New;
	    Reached=Reached | New;
	    cout<<"Iteration numero "<<NbIt<<" nb node de reached :"<<bdd_nodecount(Reached)<<endl;		
	}while(New!=bddfalse);
	tps=(double)clock() / (double)CLOCKS_PER_SEC-d;
	cout << "TPS CONSTRUCTION : "<<tps<<endl;
	return Reached;
}
/*----------------Fermeture transitive sur les transitions non observées ---*/
bdd RdPBDD::Accessible_epsilon2(bdd Init)
{
 
  bdd Reached,New,From;
  Reached=From=Init;
  do{    
       bdd succ;		
	for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
       
		 succ= relation[(*i)](From)|succ;
  	    New=succ-Reached;    
	    From=New;
	    Reached=Reached | New;		     
	}while(New!=bddfalse);
	cout << endl;
	return Reached;
}
bdd RdPBDD::Accessible_epsilon(bdd From)
{
  bdd M1;
  bdd M2=From;
  int it=0;
  do{  
        M1=M2;
	for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
	  {
       
	    bdd succ= relation[(*i)](M2);	 
	    M2=succ|M2;
	  }	  
	TabMeta[nbmetastate]=M2;
	int intsize=bdd_anodecount(TabMeta,nbmetastate+1);
	if(MaxIntBdd<intsize)
	    MaxIntBdd=intsize;	
	it++;
	//	cout << bdd_nodecount(M2) << endl;
  }while(M1!=M2);
  //cout << endl;
	return M2;
}/*------------------------  StepForward()  --------------*/
bdd RdPBDD::StepForward2(bdd From)
{
  // cout<<"Debut Step Forward \n";
  bdd Res;
  for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
  {
       bdd succ = relation[(*i)](From);
       Res=Res|succ;
  }
  //cout<<"Fin Step Forward \n";
  return Res;
}
bdd RdPBDD::StepForward(bdd From)
{
  //cout<<"Debut Step Forward \n";
  bdd Res=From;
  for(Set::const_iterator i=NonObservable.begin();!(i==NonObservable.end());i++)
  {
       bdd succ = relation[(*i)](Res);
       Res=Res|succ;
  }
  //cout<<"Fin Step Forward \n";
  return Res;
}
/*------------------------  StepBackward2()  --------------*/
bdd RdPBDD::StepBackward2(bdd From)
{
  bdd Res;
  cout<<"Ici Step Back : From.id() = "<<From.id()<<endl;
   for(vector<Trans>::const_iterator t=relation.begin();t!=relation.end();t++)
   {
       bdd succ = (*t)[From];
       Res=Res|succ;
  }
  cout<<"Res.id() = "<<Res.id()<<endl;
  return Res;
}
/*------------------------  StepBackward()  --------------*/
bdd RdPBDD::StepBackward(bdd From)
{
  bdd Res=From;
   for(vector<Trans>::const_iterator t=relation.begin();
	t!=relation.end();t++){
       bdd succ = (*t)[Res];
       Res=Res|succ;
  }
  return Res;
}

pair <int,bdd> RdPBDD::StepBackward1(bdd From,Class_Of_State * agr )
{
pair<int,bdd> p;
 for(auto t : NonObservable )
	{
	    cout<<"on traite transition "<<t+1<<endl; 
        bdd succ = (relation[t])[From];
        cout<<"on aura le bdd  "<<succ<<endl;  


        //la fonction qui retourne le bdd precedent avec la transition t 
       
        if ( (succ!=bdd_false() )&((succ &= agr->class_state)!=bdd_false() )){
        cout<<"non null et appartient a agr !  "<<endl;  
         p.first=t;
        p.second=succ;
        break;
        }
  }
  
    return p;
}



bool operator<(const pair<bdd,bdd> r, const pair<bdd,bdd> s ) 
{ return r.first.id()== s.first.id(); }

/*---------------------------GetSuccessor()-----------*/
bdd RdPBDD::get_successor(bdd From,int t)
{
  return relation[t](From);
}
/*-------------------------Firable Obs()--------------*/
Set RdPBDD::firable_obs(bdd State)
{
  Set res;
  for(Set::const_iterator i=Observable.begin();!(i==Observable.end());i++)
  {
	{bdd succ =  relation[(*i)](State);
	if(succ!=bddfalse)
	  res.insert(*i);}
   }
  return res;
}

/**-----------------------les points de sortie d'un agr avec une transition  donnée t ---------*/

void RdPBDD::bdd_firable_obs(Class_Of_State * agr, int t)
{
  /*Set res;
  for(Set::const_iterator i=Observable.begin();!(i==Observable.end());i++)
  {
	{bdd succ =  relation[(*i)](State);
	if(succ!=bddfalse)
	  res.insert(*i);}
   }
  return res;*/
  cout<<"les differents bdd de l agregat "<<agr->class_state<<endl;

}

/*---------------------------------------------------------*/

int findMax(set<int> my_set)
{
  
    // Get the maximum element
    int max_element;
    if (!my_set.empty())
        max_element = *(my_set.rbegin());
  
    // return the maximum element
    return max_element;
}

int choisir(set<int>cov ,set<int>fire)
{

std::set<int>::iterator it= fire.begin();
    
while( it != fire.end())
    { if (const bool is_in = cov.find(*it) == cov.end())
        {
        return *it;
        }
        it++;
    }


return -1;
}



void reinit_cycle(vector < int > sw,map <int,int> & tranobs){

for (auto i :sw){
    if(tranobs[i]>0)
    {
    tranobs[i]=2;
    } 
}
}




/*---------------------------------extraction des chemins ---------------------------------*/
typedef vector < int > chem;

set < chem > RdPBDD::chem_obs(MDGraph & g,map <int,int> tranobs) {
   set < chem > cto;
   vector < int > sw;
   bool ancien= true ;
   set < int > cov;
   typedef pair < Class_Of_State * , bdd > couple;
   typedef pair < couple, Set > Pair;
   typedef stack < Pair > pile;
   pile st;
   int t;

   double d, tps;
   d = (double) clock() / (double) CLOCKS_PER_SEC;
   TabMeta = new bdd[100000];
   nbmetastate = 0;
   MaxIntBdd = 0;
   NbIt = 0;
   itext = itint = 0;
   Class_Of_State * reached_class;
   Set fire;

   Class_Of_State * c = new Class_Of_State; // construction duu premier agregat
   {
      bdd Complete_meta_state = Accessible_epsilon(M0);
      fire = firable_obs(Complete_meta_state);
      c -> class_state = Complete_meta_state;
      TabMeta[nbmetastate] = c -> class_state;
      nbmetastate++;
      old_size = bdd_nodecount(c -> class_state);
      st.push(Pair(couple(c, Complete_meta_state), fire));
   }
   g.setInitialState(c);               // init
   g.insert(c);
   cout << "premier agregat construit " << endl;
   g.nbMarking += bdd_pathcount(c -> class_state);
   for (auto f: fire) {
      cout << "le nb de fires de"<<c->class_state.id()<<" est " << fire.size() << " : t" << f+1 << endl;
   }
   while (!st.empty()) {
     cout<<"il reste des elmts dans st " <<endl; 
      Pair e = st.top();
      st.pop();
      nbmetastate--;
     
      if (!e.second.empty()) //firable non vide 
      {
         cout<<"il reste des fire" <<endl;   
        //choisir transition des transitions firables  
         int t = choisir(cov,e.second);
         if (t!=-1){ancien=false;
         tranobs[t]--;
         sw.push_back(t);
         if (tranobs[t]==0){cov.insert(t);
          if (cov.size() == Observable.size()) {
            
            if(!ancien){cto.insert(sw); }            
            return cto;
         }
         }
         }
         
         else{t=*e.second.begin();
                  sw.push_back(t);
}
        cout<<"+++++++++++++++on a trouvé la transition t"<<t+1<<endl;
         e.second.erase(t);

       
         cout << "on ajoute t"<<t+1 <<" au chemin"<< endl;

         cout << "ici on couvre  la transition t" << t+1 <<"qui a un second de "<<tranobs[t]<< " et le nb de trans couv = " << cov.size() <<"/"<<Observable.size()<< endl;
         st.push(e);

    

         double nbnode;
         reached_class = new Class_Of_State; {
            cout << "on traite un bdd avac la trans t" <<t+1<< endl;
            bdd Complete_meta_state=Accessible_epsilon(get_successor(e.first.second,t));
            cout << "on crre un bdd" << endl;
            reached_class -> class_state = Complete_meta_state;
            Class_Of_State * pos = g.find(reached_class);
            nbnode = bdd_pathcount(reached_class -> class_state);
            
            if (!pos) // nouvel agregat
            {
               cout << "nouvel agregat" << endl;
               fire = firable_obs(Complete_meta_state);
               for (auto f: fire) {
                  cout << "le nb de fires de  "<<reached_class->class_state.id()<<" est " << fire.size() << " : t" << f+1 << endl;
               }
               //st.push(Pair(couple(reached_class, Complete_meta_state), fire));
               nbmetastate++;
               e.first.first -> Successors.insert(e.first.first -> Successors.begin(), Edge(reached_class, t));
               reached_class -> Predecessors.insert(reached_class -> Predecessors.begin(), Edge(e.first.first, t));
               cout<<"les pred sont avec t"<<Edge(e.first.first, t).second+1<<endl;
               g.addArc();
               g.insert(reached_class); //ajouter l'agregat 
               if (fire.empty()) {
                cout << "pas de successeurs " << endl;

                if(!ancien){cto.insert(sw);}
                reinit_cycle(sw,tranobs);
                   
                  
                  
                  sw.pop_back();
                  cout << "on enleve du chemin"<< endl;

               } else {
                 cout << "il y a  de successeurs " << endl;
                  st.push(Pair(couple(reached_class, Complete_meta_state), fire));
               }

            } else {  //agregat existant
               cout << "agregat deja existant" << endl;
               if(!ancien){cto.insert(sw);}
               reinit_cycle(sw,tranobs);
                
               sw.pop_back();
               ancien=true;
               cout << "on enleve du chemin"<< endl;
               delete reached_class;
               e.first.first -> Successors.insert(e.first.first -> Successors.begin(), Edge(pos, t));
               pos -> Predecessors.insert(pos -> Predecessors.begin(), Edge(e.first.first, t));
                cout<<"les pred existe sont avec "<<Edge(e.first.first, t).second+1<<endl;
               g.addArc();
            }
         }

      } else {

         sw.pop_back();
         ancien=true;
      }
   }
   
   return cto;
}

/////////////////////////////////////////////

stack <pair<Class_Of_State *, bdd >>  RdPBDD::recherche_points_entree(chem ch,MDGraph& g)
{

stack <pair<Class_Of_State *,bdd >> pt_entr;
pair<Class_Of_State *,bdd> p;
Class_Of_State * agr = new Class_Of_State;


bdd entree= M0;
agr=g.initialstate;

p.first= agr;
p.second=entree;
pt_entr.push(p);
//cout<<"l' 1 agr "<<agr->class_state.id()<<endl; 
//cout<<"le 1 bdd "<<entree<<endl; 

// cout<<"les points de sortie avec  le t"<< ch[0]<<" sont : "<<FrontiereNodes1(agr->class_state,ch[0])<<endl; 


for (std::vector<int>::iterator k = ch.begin() ; k != ch.end()-1; k++ )
{
int t=*k;
entree=relation[t](p.first-> class_state);
p.second=entree;

    for(auto succ: agr->Successors){
        if(succ.second==t)
        {
       agr=succ.first;
       //cout<<"avec trans "<<t+1<<endl;
        //cout<<"l'id de agr "<<agr->class_state.id()<<endl;
        break;
        } 
    }
    
 p.first=agr;   
 pt_entr.push(p);

}
cout<<"entrey points found"<<endl;
return  pt_entr;
}

////////////////////////////////////////////

chem RdPBDD::chem_abs(chem ch,MDGraph& g )
{
   vector < int > ch_abstrait , ch_agregat;
   int trans;
   pair<Class_Of_State*, bdd > agr_entree;
   stack <pair<Class_Of_State*, bdd > > point_entree;
   bdd cible,source;
   Class_Of_State * agr = new Class_Of_State;
   //agr=dernier agr
   point_entree=recherche_points_entree(ch,g);
   int i=0;
   while(!point_entree.empty())
   {
    trans=*(ch.end()-1) ;
    cout<<"on traite "<< trans+1<<endl;
    agr_entree=point_entree.top();
    cout<<" l'agr traité "<<agr_entree.first->class_state.id()<<endl;
    point_entree.pop();//effacer l'element le dernier element 
    cible=agr_entree.second;
    agr=agr_entree.first;
    if(i==0){
    source =FrontiereNodes1(agr->class_state,trans);}
    if(i==1){ source= (relation[trans])[source];}
    ch_agregat=Sub_path_agregate(&source,cible,agr);
     cout<<"sub_path found "<<endl;
     ch_abstrait.insert( ch_abstrait.begin(), trans);
     ch_abstrait.insert( ch_abstrait.begin(), ch_agregat.begin(), ch_agregat.end());
   ch.pop_back();
   i=1;
   }
   
   
   
   return ch_abstrait;
}

///////----------------- trouver un chemain de la source à cible dans un agregat agr---
vector<int> RdPBDD::Sub_path_agregate(bdd *source,bdd cible,Class_Of_State * agr )
{           
    vector<int> ch_agregat;

    pair <int,bdd> couple;
    int transition;
    int i=0;
    bdd courant=*source;
    bdd egalite= cible & courant;


   cout<<"a difference between source and target "<< (egalite == bdd_false())<<endl;
    while(egalite == bdd_false())
        { 
          couple=StepBackward1(courant,agr);
          ch_agregat.insert(ch_agregat.begin(),couple.first);
           courant=couple.second;
           egalite= (cible & courant);

            cout<<"a difference between source and target  "<< (egalite == bdd_false())<<endl;
        }

*source= courant;   
return ch_agregat ;
}  




/*-----------------------CanonizeR()----------------*/
bdd RdPBDD::CanonizeR(bdd s, unsigned int i)
 {
   bdd s1,s2;
   do
   {
	itext++;
   	s1 = s - bdd_nithvar(2*i);
   	s2 = s - s1;
   	if((!(s1==bddfalse))&&(!(s2==bddfalse)))
   	{
   		bdd front = s1;
   		bdd reached = s1;
   		do
   		{
		  //cout<<"premiere boucle interne \n";
   		    itint++;
   		    front=StepForward(front) - reached;
   		    reached = reached | front;
   		    s2 = s2 - front;
   		}while((!(front==bddfalse))&&(!(s2==bddfalse)));
   	}
   	if((!(s1==bddfalse))&&(!(s2==bddfalse)))
   	{
   		bdd front=s2;
   		bdd reached = s2;
   		do
   		{
		  // cout<<"deuxieme boucle interne \n";
		   itint++;
   		   front=StepForward(front) - reached;
   		   reached = reached | front;
   		   s1 = s1 - front;
   		}while((!(front==bddfalse))&&(!(s1==bddfalse)));
   	}
   	s=s1 | s2;
  	i++;
  }while((i<Nb_places) &&((s1==bddfalse)||(s2==bddfalse)));
  if(i>=Nb_places)
   {
     //cout<<"____________oooooooppppppppsssssssss________\n";
       return s;
   }
  else
    {
      //cout<<"________________p a s o o o p p p s s s ______\n";
      return(CanonizeR(s1,i) | CanonizeR(s2,i));
    }
}
/*---------------------------  Set_Bloc()  -------*/
bool RdPBDD::Set_Bloc(bdd &S) const
{	
  //cout<<"Ici detect blocage \n";
	int k=0;
	bdd Mt=bddtrue;		            
	  for(vector<Trans>::const_iterator i = relation.begin();	   	i!=relation.end();i++,k++) 		 
	  {
	    Mt=Mt & !((*i).Precond);	   
	  }
	return ((S&Mt)!=bddfalse);
		//BLOCAGE
}
/*-------------------------Set_Div() à revoir -----------------------------*/
bool RdPBDD::Set_Div(bdd &S) const
{
	Set::const_iterator i;
	bdd To,Reached;
	//cout<<"Ici detect divergence \n";
	Reached=S;
	do
	{
	        bdd From=Reached;
		for(i=NonObservable.begin();!(i==NonObservable.end());i++)
		{

				To=relation[*i](Reached);
				Reached=Reached|To; //Reached=To ???
				//Reached=To;
		}
		if(Reached==From)
			//cout<<"SEQUENCE DIVERGENTE \n";
			return true;
		//From=Reached;
	}while(Reached!=bddfalse);
	 return false;
	//cout<<"PAS DE SEQUENCE DIVERGENTE \n";
}
/*-----------FrontiereNodes() pour bdd ---------*/
bdd RdPBDD::FrontiereNodes(bdd From) const 
{	
	bdd res=bddfalse;
	for(Set::const_iterator i=Observable.begin();!(i==Observable.end());i++)     
			res=res | (From & relation[*i].Precond);
	return res;
}
/*-----------FrontiereNodes1() pour bdd ---------*/
bdd RdPBDD::FrontiereNodes1(bdd From, int t)  
{	
	bdd res=bddfalse;
	res=res | (From & relation[t].Precond);
    return res;
}
/*-------- Produit synchronisé à la volée de n graphes d'observation : Adaptation à la capture des séquences bloquantes et les séquences divergentes----------------------*/
void RdPBDD::GeneralizedSynchProduct1(Modular_Obs_Graph& Gv, int NbSubnets,RdPBDD* Subnets[] ,int nbbddvar) 
{
    cout<<"_____________  GeneralizedSynchProduct1  _________________________"<<NbSubnets<<"sous-reseaux "<<endl;
    //for(int k=0;k<NbSubnets;k++)
    //	cout<<*Subnets[k]<<endl;
   int pos_trans(TRANSITIONS,string);
   TabMeta=new bdd[1000000];
   nbmetastate=0;
   MaxIntBdd=0;
   nbmetastate=0;
   Stack st;
   //int cpt=0;
   int k;
   bdd *Complete_meta_state=new bdd[NbSubnets];
   Set *fire=new Set[NbSubnets];
   Modular_Class_Of_State *Meta_State=new Modular_Class_Of_State;
   for(k=0;k<NbSubnets;k++)
     {
	Complete_meta_state[k]=Subnets[k]->Accessible_epsilon(Subnets[k]->M0);
	fire[k]=Subnets[k]->firable_obs(Complete_meta_state[k]);
	//Meta_State->State.push_back(Subnets[k]->FrontiereNodes(Complete_meta_state[k]));
	//Meta_State->State.push_back(Subnets[k]->CanonizeR(Subnets[k]->FrontiereNodes(Complete_meta_state[k]),0));
	Meta_State->State.push_back(Subnets[k]->CanonizeR(Complete_meta_state[k],0));
	/*-------------------- STAT ----------------------*/
	TabMeta[nbmetastate]=Meta_State->State[k];
	nbmetastate++;
     }	
   old_size=bdd_anodecount(TabMeta,nbmetastate);
   Meta_State->blocage=true;
   for( k=0;((k<NbSubnets)&&(Meta_State->blocage));k++)
   Meta_State->blocage=Meta_State->blocage&&Subnets[k]->Set_Bloc(Complete_meta_state[k]);
   Meta_State->boucle=false;
   for(k=0;((k<NbSubnets)&&(!Meta_State->boucle));k++)
     Meta_State->boucle=Meta_State->boucle||Subnets[k]->Set_Div(Complete_meta_state[k]);
   Gv.setInitialState(Meta_State);		   
   Gv.insert(Meta_State);
   nbmetastate++;
   st.push(StackElt(Couple(Meta_State,Complete_meta_state),fire));  
   do
     {
       NbIt++;
	StackElt e=st.top();
	st.pop();	
	for(k=0;k<NbSubnets;k++)
	{
	  while(!e.second[k].empty())
	  {
		int t=*e.second[k].begin();	
		e.second[k].erase(t);
		bool ok=true;
		Set ConcernedSubnets;
		ConcernedSubnets.insert(k);
		string tmp=Subnets[k]->transitions[t].name;
		for(int j=0;j<NbSubnets;j++)
		{
		  if(j!=k)
		  {
		    int num=Subnets[j]->transitionName[tmp];		
		    int pos=pos_trans(Subnets[j]->transitions,tmp);
		    if((pos!=-1)&&!(Subnets[j]->Observable.find(num)==Subnets[j]->Observable.end()))
		    {							
			ConcernedSubnets.insert(j);
			if(e.second[j].find(num)==e.second[j].end())
			{
			  ok=false;
			}
			else
			  e.second[j].erase(num);
		    }				
		  }
		}
		if(ok)
		{
		  Complete_meta_state=new bdd[NbSubnets];
		  fire=new Set[NbSubnets];
		  Meta_State=new Modular_Class_Of_State;
		  for(int j=0;j<NbSubnets;j++)
		  {
		    if(ConcernedSubnets.find(j)==ConcernedSubnets.end())
		    {
		      Complete_meta_state[j]=e.first.second[j];		
		      Meta_State->State.push_back(e.first.first->State[j]);
		    }
		    else
		    {											
		      Complete_meta_state[j]=Subnets[j]->Accessible_epsilon(Subnets[j]->get_successor(e.first.second[j],Subnets[j]->transitionName[tmp])); 
		      //Point de sortie
		      //Meta_State->State.push_back(Subnets[j]->FrontiereNodes(Complete_meta_state[j]));
		      //Meta_State->State.push_back(Subnets[j]->CanonizeR(Subnets[j]->FrontiereNodes(Complete_meta_state[j]),0));
		      Meta_State->State.push_back(Subnets[j]->CanonizeR(Complete_meta_state[j],0));
			/*-------------------- STAT ----------------------*/
		      TabMeta[nbmetastate]=Meta_State->State[k];
		      nbmetastate++;
		    }
		    fire[j]=Subnets[j]->firable_obs(Complete_meta_state[j]);
		  }
		  Modular_Class_Of_State * pos=Gv.find(Meta_State);
		  if(!pos)
		  {		
		    old_size=bdd_anodecount(TabMeta,nbmetastate);
		    //Calcul de deadlock et loop attributes
		    Meta_State->blocage=true;
		    for(int j=0;((j<NbSubnets)&&(Meta_State->blocage));j++)
		    Meta_State->blocage&=Subnets[j]->Set_Bloc(Complete_meta_state[j]);
		    Meta_State->boucle=false;
		    for(int j=0;((j<NbSubnets)&&(!Meta_State->boucle));j++)
		      Meta_State->boucle|=Subnets[j]->Set_Div(Complete_meta_state[j]);
		    st.push(StackElt(Couple(Meta_State,Complete_meta_state),fire));
		    e.first.first->Successors.insert(e.first.first->Successors.begin(),Modular_Edge(Meta_State,tmp));
		    Meta_State->Predecessors.insert(Meta_State->Predecessors.begin(),Modular_Edge(e.first.first,tmp));
		    Gv.addArc();
		    Gv.insert(Meta_State);						
		  }
		  else
		  {
		    e.first.first->Successors.insert(e.first.first->Successors.begin(),Modular_Edge(pos,tmp));
		    pos->Predecessors.insert(pos->Predecessors.begin(),Modular_Edge(e.first.first,tmp));
		    Gv.addArc();
		    delete Meta_State;
		    //Neoud déja rencontré ;
		  }
		}
	  }
	}
     }while(!st.empty());
   cout<<" MAXIMAL INTERMEDIARY BDD SIZE \n"<<MaxIntBdd<<endl;
   cout<<"OLD SIZE : "<<bdd_anodecount(TabMeta,nbmetastate)<<endl;
   cout<<"NB SHARED NODES : "<<bdd_anodecount(TabMeta,nbmetastate)<<endl;
   cout<<"NB META STATE DANS CONSTRUCTION : "<<nbmetastate<<endl;
   cout<<"NB ITERATIONS CONSTRUCTION : "<<NbIt<<endl;
   cout<<"NB ITERATIONS EXTERNES : "<<itext<<endl;
   cout<<"NB ITERATIONS INTERNES : "<<itint<<endl;
}
/*------------------------EmersonLey ----------------------------*/
bdd RdPBDD::EmersonLey(bdd S,bool trace) 
{
	cout<<"ICI EMERSON LEY \n";
	double		TpsInit, TpsDetect;
	double debitext,finitext;
	TpsInit = (double)(clock()) / CLOCKS_PER_SEC;
	bdd b=S;
	bdd Fair=bdd_ithvar(2*Nb_places-1);
	cout<<"PLACE TEMOIN \n";
	//cout<<places[places.size()-1].name<<endl;
	bdd oldb;
	bdd oldd,d;
	int extit=0;
	int init=0;
	do
	{
		
		extit++;
		if(trace)
		{
			
			cout<<"ITERATION EXTERNES NUMERO :"<<extit<<endl;
			debitext=(double)(clock()) / CLOCKS_PER_SEC;
			cout<<"TAILLE DE B AVANT IT INTERNE : "<<bdd_nodecount(b)<<endl;
			cout<<endl<<endl;
		}
		oldb=b;
		//cout<<"Fair : "<<Fair.id()<<endl;
		d=b & Fair;
		//cout<<"d : "<<d.id()<<endl;
		//init=0;
		do
		{
			init++;
			if(trace)
			{
				
				cout<<"ITERATION INTERNES NUMERO :"<<init<<endl;
				cout<<"HEURE : "<<(double)(clock()) / CLOCKS_PER_SEC<<endl;
				cout<<"TAILLE DE D : "<<bdd_nodecount(d)<<endl;
			}
			oldd=d;
			bdd inter=b & StepForward2(d);
			//cout<<"Tille de inter :"<<bdd_nodecount(inter)<<endl;
			d=d | inter;
		}while(!(oldd==d));
		if(trace)
			cout<<"\nTAILLE DE D APRES ITs INTERNES : "<<bdd_nodecount(d)<<endl;
		b=b & StepBackward2(d);
		init++;
		if(trace)
		{
			cout<<"\n\nTAILLE DE B APRES ELEMINER LES PRED DE D : "<<bdd_nodecount(b)<<endl;
			finitext=(double)(clock()) / CLOCKS_PER_SEC;
			cout<<"DUREE DE L'ITERATION EXTERNE NUMERO "<<extit<<"  :  "<<finitext-debitext<<endl;
			cout<<endl<<"_________________________________________________\n\n";
		}
	}while(!(b==oldb));
	cout<<"NOMBRE D'ITERATIONS EXTERNES -----:"<<extit<<endl;
	cout<<"NOMBRE D'ITERATIONS INTERNES -----:"<<init<<endl;
	TpsDetect = ((double)(clock()) / CLOCKS_PER_SEC) - TpsInit;
	cout << "DETECTION DE CYCLE TIME  " << TpsDetect << endl;
	return b;
}

