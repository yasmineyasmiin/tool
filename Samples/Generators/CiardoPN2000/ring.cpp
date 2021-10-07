#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <string>

int main(int argc,char **argv){	
  int N;

  if (argc!=2) return(-1);
  if ((N=atoi(argv[1]))<2) return(-1);
  char nom[20];
  sprintf(nom,"ring%d.net",N);
  ofstream f(nom);

  for(int i=0;i<N;i++){
    f<<"#place C"<<i<<" mk(<..>)"<<endl;
    f<<"#place E"<<i<<endl;
    f<<"#place A"<<i<<" mk(<..>)"<<endl;
    f<<"#place B"<<i<<endl;
    f<<"#place D"<<i<<endl;
    f<<"#place F"<<i<<endl;
    f<<"#place G"<<i<<endl;
    f<<"#place H"<<i<<endl;
  }

  for(int i=0;i<N;i++){
    
    f<<"#trans Free"<<i<<endl;
    f<<"in {A"<<i<<":<..>;C"<<((i+N-1)%N)<<":<..>;}"<<endl;
    f<<"out {B"<<i<<":<..>;E"<<((i+N-1)%N)<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans Used"<<i<<endl;
    f<<"in {H"<<i<<":<..>;C"<<((i+N-1)%N)<<":<..>;}"<<endl;
    f<<"out {B"<<i<<":<..>;D"<<((i+N-1)%N)<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans Get"<<i<<endl;
    f<<"in {F"<<i<<":<..>;B"<<i<<":<..>;}"<<endl;
    f<<"out {C"<<i<<":<..>;A"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans Put"<<i<<endl;
    f<<"in {B"<<i<<":<..>;G"<<i<<":<..>;}"<<endl;
    f<<"out {C"<<i<<":<..>;H"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans Other"<<i<<endl;
    f<<"in {D"<<i<<":<..>;}"<<endl;
    f<<"out {G"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans Owner"<<i<<endl;
    f<<"in {D"<<i<<":<..>;}"<<endl;
    f<<"out {E"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans GoOn"<<i<<endl;
    f<<"in {E"<<i<<":<..>;}"<<endl;
    f<<"out {F"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans Write"<<i<<endl;
    f<<"in {E"<<i<<":<..>;}"<<endl;
    f<<"out {G"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<flush;
  }
}
