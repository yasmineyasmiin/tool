#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <string>

int
main(int argc,char **argv)
{	
  int N, P;

  if (argc!=3) return(-1);
  if ((P=atoi(argv[1]))<2) return(-1);
  if ((N=atoi(argv[2]))<1) return(-1);
  char nom[200];
  sprintf(nom,"EcrivainsPrehemptifs2.%d_%d.net",P,N);
  ofstream f(nom);
  
  f<<"#place Idle mk("<<P<<"*<..>)"<<endl;
  
  for (int i = 0; i < N; i++)
  {
    f<<"#place Read"<<i<<endl;
    f<<"#place Write"<<i<<endl;
    f<<"#place Test"<<i<<" mk(<..>)"<<endl;
  }
  
  for (int i = 0; i < N; i++)
  {
    f<<endl;
    f<<"#trans t1_"<<i<<endl;
    f<<"in {Idle:<..>;Test"<<i<<":<..>;}"<<endl;
    f<<"out {Read"<<i<<":<..>;Test"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;
    f<<endl;
    f<<"#trans t2_"<<i<<endl;
    f<<"in {Read"<<i<<":<..>;}"<<endl;
    f<<"out {Idle:<..>;}"<<endl;
    f<<"#endtr"<<endl;
/*    f<<endl;
    f<<"#trans t3_"<<i<<endl;
    f<<"in {Idle:<..>;Test"<<i<<":<..>;Read"<<i<<":Read"<<i<<";}"<<endl;
    f<<"out {Write"<<i<<":<..>;Idle:Read"<<i<<";}"<<endl;
    f<<"#endtr"<<endl;
    f<<endl; */
    
    for (int j = 0; j < P; j++)
    {
      f<<endl;
      f<<"#trans t3_"<<i<<"_"<<j<<endl;
      f<<"in {Idle:<..>;Test"<<i<<":<..>;";
      f<<"Read"<<i<<":"<<j<<"*<..>;Read"<<i<<"<"<<j+1<<";}"<<endl;
      f<<"out {Write"<<i<<":<..>;Idle:"<<j<<"*<..>;}"<<endl;
      f<<"#endtr"<<endl;
      f<<endl; 
    }
    
    f<<"#trans t4_"<<i<<endl;
    f<<"in {Write"<<i<<":<..>;}"<<endl;
    f<<"out {Idle:<..>;Test"<<i<<":<..>;}"<<endl;
    f<<"#endtr"<<endl;
  }
  cout<<flush;
}
