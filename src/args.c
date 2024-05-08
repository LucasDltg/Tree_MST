#include "args.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXWEIGHT 10000000000

void ask()  //function who ask if the user wants to continue
{
  char choice;
  as : 
      printf("Do you want to continue ? [y/n]\n");
      scanf("%c",&choice);
      getchar();
      if(choice == 'n'){exit(1);}
      else if(choice == 'y'){return;}
      else{goto as;}
}

int parse_args(int argc, char **argv, Options_t *opt){

    opt->savebinfile = NULL;        // -o
    opt->pathdatabase = NULL;       // -i
    opt->savedelaunay = NULL;       // -d
    opt->savefinaloutput = NULL;    // -f
    opt->loadfile = NULL;           // -l
    opt->header = 0;                //store the header of binary file if the user use -l
    opt->limitweight = MAXWEIGHT;  //store the limit of weight for an edge
  int index;
  int c;
  char aide[] = "Options available are :\n\t-i : Path to .CSV\n\t-o : Path where to save the binary file\n\t-d : Path where to save the Delaunay's triangulation file, if null file won't be saved\n\t-f : Path where to save the Minimum Spanning Tree, if null saved in /data/output\n\t-l : Path of the file to be loaded (if the file is a MST, it will be displayed)\n\t -w : limit of the weight for an edge"; 

  opterr = 0;

  while ((c = getopt (argc, argv, "l:o:i:d:f:w:h")) != -1)
    switch (c)
      {
      case 'd':
        opt->savedelaunay = optarg;
        break;
      case 'f':
        opt->savefinaloutput = optarg;
        break;
      case 'o':
        opt->savebinfile = optarg;
        break;
      case 'i':
        opt->pathdatabase = optarg;
        break;
      case 'l':
        opt->loadfile = optarg;
        break;
      case 'w':
        opt->limitweight = atof(optarg);
        break;
      case 'h':
        printf("%s\n",aide);
        exit(0);
      case '?':
        if (optopt == 'd' || optopt == 'f' || optopt == 'o'|| optopt == 'i' || optopt == 'l' || optopt == 'w')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
        exit(1);
      default:
        exit(1);
      }


  for (index = optind; index < argc; index++)
  {
    printf ("Non-option argument %s\n", argv[index]);
    exit(1);
  }
  
  if(opt->loadfile !=NULL)
  {
      if(opt->limitweight != MAXWEIGHT)
      {
        printf("Warning -w is useless\n");
        ask(); 
      }
      FILE *load = fopen(opt->loadfile,"r");
      if(load == NULL)
      {
          printf("Unable to load %s\nAre you sure the file is existing ?\n",opt->loadfile);
          exit(1);
      }
      fread(&(opt->header),sizeof(char),1,load);
      fclose(load);

      if(opt->header == 'M'){return 0;}

      if(opt->header != 'D' && opt->header != 'B')  //verify if the file is compatible
      {
          printf("Unknown type of file\n");
          exit(1);
      }
  }



  if(opt->savefinaloutput == NULL)
  {
    printf("Warning no path for output, output will be save at data/output, could erase other datas\n");
    ask();
    opt->savefinaloutput = "data/output";//default output
  }
  
  if(opt->pathdatabase == NULL && opt->loadfile == NULL)
  {
    printf("No file to load\n");
    exit(1);
  }

  if(opt->savedelaunay  == NULL && opt->savebinfile == NULL)
  {
    printf("Warning no intermediate files, the process could be long, you will have to recalculate everything the next time\n");
    ask();
  }
  
  return 0;

}