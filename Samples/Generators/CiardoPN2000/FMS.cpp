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
  sprintf(nom,"fms%d.net",N);
  ofstream f(nom);

  {
    f<<"#place P1 mk("<<N<<"<..>)"<<endl;
    f<<"#place P1wM1"<<endl;
    f<<"#place P1M1"<<endl;
    f<<"#place M1 mk(3<..>)"<<endl;
    f<<"#place P1d"<<endl;
    f<<"#place P1s"<<endl;
    f<<"#place P1wP2"<<endl;
    f<<"#place P12"<<endl;
    f<<"#place P12wM3"<<endl;
    f<<"#place P12M3"<<endl;
    f<<"#place M3 mk(2<..>)"<<endl;
    f<<"#place P12s"<<endl;
    f<<"#place P2 mk("<<N<<"<..>)"<<endl;
    f<<"#place P2wM2"<<endl;
    f<<"#place P2M2"<<endl;
    f<<"#place M2 mk(<..>)"<<endl;
    f<<"#place P2d"<<endl;
    f<<"#place P2wP1"<<endl;
    f<<"#place P2s"<<endl;
    f<<"#place P3 mk("<<N<<"<..>)"<<endl;
    f<<"#place P3M2"<<endl;
    f<<"#place P3s"<<endl;
  }

  {    
    f<<"#trans tP1"<<endl;
    f<<"in {P1:<..>;}"<<endl;
    f<<"out {P1wM1:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tM1"<<endl;
    f<<"in {P1wM1:<..>;M1:<..>;}"<<endl;
    f<<"out {P1M1:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP1M1"<<endl;
    f<<"in {P1M1:<..>;}"<<endl;
    f<<"out {M1:<..>;P1d:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP1e"<<endl;
    f<<"in {P1d:<..>;}"<<endl;
    f<<"out {P1s:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP1s"<<endl;
    f<<"in {P1s:<..>;}"<<endl;
    f<<"out {P1:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP1j"<<endl;
    f<<"in {P1d:<..>;}"<<endl;
    f<<"out {P1wP2:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tx"<<endl;
    f<<"in {P1wP2:<..>;P2wP1:<..>;}"<<endl;
    f<<"out {P12:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP12"<<endl;
    f<<"in {P12:<..>;}"<<endl;
    f<<"out {P12wM3:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tM3"<<endl;
    f<<"in {P12wM3:<..>;M3:<..>;}"<<endl;
    f<<"out {P12M3:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP12M3"<<endl;
    f<<"in {P12M3:<..>;}"<<endl;
    f<<"out {P12s:<..>;M3:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP12s"<<endl;
    f<<"in {P12s:<..>;}"<<endl;
    f<<"out {P1:<..>;P2:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP2"<<endl;
    f<<"in {P2:<..>;}"<<endl;
    f<<"out {P2wM2:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tM2"<<endl;
    f<<"in {P2wM2:<..>;M2:<..>;}"<<endl;
    f<<"out {P2M2:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP2M2"<<endl;
    f<<"in {P2M2:<..>;}"<<endl;
    f<<"out {P2d:<..>;M2:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP2j"<<endl;
    f<<"in {P2d:<..>;}"<<endl;
    f<<"out {P2wP1:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP2e"<<endl;
    f<<"in {P2d:<..>;}"<<endl;
    f<<"out {P2s:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP2s"<<endl;
    f<<"in {P2s:<..>;}"<<endl;
    f<<"out {P2:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP3"<<endl;
    f<<"in {P3:<..>;}"<<endl;
    f<<"out {P3M2:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP3M2"<<endl;
    f<<"in {P3M2:<..>;M2:<..>;}"<<endl;
    f<<"out {P3s:<..>;M2:<..>;}"<<endl;
    f<<"#endtr"<<endl;

    f<<"#trans tP3s"<<endl;
    f<<"in {P3s:<..>;}"<<endl;
    f<<"out {P3:<..>;}"<<endl;
    f<<"#endtr"<<endl;
    f<<flush;
  }
}
