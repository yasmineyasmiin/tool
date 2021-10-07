#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc,char **argv)
{	
  int i,n;
  char Philo[20]="Philo";
  FILE *f;

  if (argc!=2) return(-1);
  if ((n=atoi(argv[1]))<2) return(-1);

  strcat(Philo,argv[1]);
  strcat(Philo,".net");

  if ((f=fopen(Philo,"w"))==NULL)
    return (-1);

  for(i=0;i<n;i++)
    {
      fprintf(f,"#place Think%d mk(<..>)\n",i);
      fprintf(f,"#place Eat%d\n",i);
      fprintf(f,"#place Fork%d mk(<..>)\n",i);
    }

  for(i=0;i<n;i++)
    {
      fprintf(f,"#trans TakeFork%d\n",i);
      fprintf(f,"in {Fork%d:<..>;Think%d:<..>;Fork%d:<..>;}\n",(i+n-1)%n,i,i);
      fprintf(f,"out {Eat%d:<..>;}\n",i);
      fprintf(f,"#endtr\n");

      fprintf(f,"#trans LeaveFork%d\n",i);
      fprintf(f,"in {Eat%d:<..>;}\n",i);
      fprintf(f,"out {Fork%d:<..>;Think%d:<..>;Fork%d:<..>;}\n",(i+n-1)%n,i,i);
      fprintf(f,"#endtr\n");
    }
  fclose(f);
}
