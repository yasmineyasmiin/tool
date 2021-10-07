#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <string>

int
main(int argc,char **argv)
{	
  int N;

  if (argc!=2) return(-1);
  if ((N=atoi(argv[1]))<2) return(-1);
  char nom[20];
  sprintf(nom,"philo%d.net",N);
  ofstream f(nom);

  for(int i=0;i<N;i++){
    f<<"#place Idle"<<i<<" mk(<..>)"<<endl;
    f<<"#place WaitLeft"<<i<<endl;
    f<<"#place HasLeft"<<i<<endl;
    f<<"#place WaitRight"<<i<<endl;
    f<<"#place HasRight"<<i<<endl;
    f<<"#place Fork"<<i<<" mk(<..>)"<<endl;
  }

  for(int i=0;i<N;i++){
    f<<"#trans GoEat"<<i<<endl;
    f<<"in {Idle"<<i<<":<..>;}"<<endl;
    f<<"out {WaitLeft"<<i<<":<..>;WaitRight"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans GoLeft"<<i<<endl;
    f<<"in {Fork"<<((i+N-1)%N)<<":<..>;WaitLeft"<<i<<":<..>;}"<<endl;
    f<<"out {HasLeft"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans GoRight"<<i<<endl;
    f<<"in {Fork"<<i<<":<..>;WaitRight"<<i<<":<..>;}"<<endl;
    f<<"out {HasRight"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans Release"<<i<<endl;
    f<<"in {HasLeft"<<i<<":<..>;HasRight"<<i<<":<..>;}"<<endl;
    f<<"out {Idle"<<i<<":<..>;Fork"<<((i+N-1)%N)<<":<..>;Fork"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;
  }
  cout<<flush;
}
