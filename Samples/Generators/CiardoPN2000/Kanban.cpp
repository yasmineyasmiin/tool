#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <string>

int main(int argc,char **argv){	
  int N;

  if (argc!=2) return(-1);
  if ((N=atoi(argv[1]))<1) return(-1);
  char nom[20];
  sprintf(nom,"kanban%d.net",N);
  ofstream f(nom);

  for(int i=4;i>0;i--){
    f<<"#place P"<<i<<" mk("<<N<<"<..>)"<<endl;
    f<<"#place Pm"<<i<<endl;
    f<<"#place Pback"<<i<<endl;
    f<<"#place Pout"<<i<<endl;
  }

  f<<"#trans tin4"<<endl;
  f<<"in {P4:<..>;}"<<endl;
  f<<"out {Pm4:<..>;}"<<endl;
  f<<"#endtr"<<endl;

  for(int i=4;i>0;i--){    
    f<<"#trans tredo"<<i<<endl;
    f<<"in {Pm"<<i<<":<..>;}"<<endl;
    f<<"out {Pback"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tback"<<i<<endl;
    f<<"in {Pback"<<i<<":<..>;}"<<endl;
    f<<"out {Pm"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tok"<<i<<endl;
    f<<"in {Pm"<<i<<":<..>;}"<<endl;
    f<<"out {Pout"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;
  }

  f<<"#trans tsynch4_23"<<endl;
  f<<"in {Pout4:<..>;P2:<..>;P3:<..>;}"<<endl;
  f<<"out {P4:<..>;Pm2:<..>;Pm3:<..>;}"<<endl;
  f<<"#endtr"<<endl;

  f<<"#trans tsynch1_23"<<endl;
  f<<"in {Pout3:<..>;Pout2:<..>;P1:<..>;}"<<endl;
  f<<"out {P3:<..>;P2:<..>;Pm1:<..>;}"<<endl;
  f<<"#endtr"<<endl;

  f<<"#trans tout1"<<endl;
  f<<"in {Pout1:<..>;}"<<endl;
  f<<"out {P1:<..>;}"<<endl;
  f<<"#endtr"<<endl;

  cout<<flush;
  
}
