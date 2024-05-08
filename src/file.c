#include <stdio.h>
#include <stdlib.h>
#include "file.h"

void Siginthandler()
{
    printf("\nProcess interrupted\n");
    exit(1);
}

void ExtractCoord(Point *coord, int *sizecoord, char path[])
{
    FILE *fcsv = fopen(path,"r");
    if(fcsv == NULL)
    {
        printf("Unable to load %s\nAre you sure the file is existing ?\n",path);
        exit(1);                                          
    }

    char letter = 0;            //store each caracter
    short counter = 0;           //counter of ';'
    int i = 0;                  //counter of line
    double coordtemp = 0;

    char temp[250];
    fgets(temp,250,fcsv);  //remove the first line of the file

    do
    {
        if(letter == ';')
        {
            counter++;
            if(counter == 16)        //if we reach coordinates
            {
                fscanf(fcsv,"%lf",&coordtemp); //get first coordinate
                coord[i].x = coordtemp;
                letter = fgetc(fcsv);       //remove ';'
                fscanf(fcsv,"%lf",&coordtemp); //get second coordinate
                coord[i].y = coordtemp;
                
            }
        }
        else if(letter == '\n')//if we reach the end of the line
        {
            if(counter < 16)
            {
                printf("Field missing in .CSV\n");
            }
            i++;
            counter=0;
        }
        letter = fgetc(fcsv);
        
    } while (letter != EOF);
    *sizecoord = i+1;
    
    fclose(fcsv);
}

void SaveCoord(char path[],Point *coord,int sizecoord, char header)
{
    FILE *save = fopen(path,"wb");
    if(save == NULL)
    {
        printf("Unable to create file %s\n",path);
        exit(1);   
    }

    fwrite(&header,sizeof(char),1,save); //header of the file
    fwrite(&sizecoord,sizeof(int),1,save); //write number of the following coordinates
    fwrite(coord,sizeof(Point),sizecoord,save); //write coordinates
    fclose(save);
}

void ReadSavedCoord(char path[], Point **coord, int *sizecoord)
{
    FILE *save = fopen(path,"rb");
    if(save == NULL)
    {
        printf("Unable to load %s\nAre you sure the file is existing ?\n",path);
        exit(1);   
    }
    fseek(save,sizeof(char),SEEK_SET);//skip the header
    fread(sizecoord,sizeof(int),1,save); //read the number of coordinates
    *coord = malloc((int)sizeof(Point)*(*sizecoord));
    if(*coord == NULL)
    {
        printf("malloc error in ReadSavedCoord\n");
        exit(1);
    }
    fread(*coord,sizeof(Point),*sizecoord,save);  //read coordinates
    fclose(save);
}

void SaveDelaunay(char path[],Point *coord,int sizecoord,Triangle *triangles,int sizetriangles)
{
    SaveCoord(path,coord,sizecoord,'D'); //firstly, write coordinates
    FILE *fsave = fopen(path,"ab");
    if(fsave == NULL)
    {
        printf("Unable to create file %s\n",path);
        exit(1);   
    }
    
    fwrite(&sizetriangles,sizeof(int),1,fsave);// write number of triangle
    
    for(int i =0;i<sizetriangles;i++)//write triangles
    {
        fwrite(&triangles[i].vertices[0],sizeof(int),1,fsave);
        fwrite(&triangles[i].vertices[1],sizeof(int),1,fsave);
        fwrite(&triangles[i].vertices[2],sizeof(int),1,fsave);       
    }
    fclose(fsave);
}

void ReadDelaunay(char path[], Point **coord,int *sizecoord,Triangle **triangles, int *sizetriangles)
{
    FILE * file = fopen(path,"rb");
    if(file == NULL)
    {
        printf("Unable to load %s\nAre you sure the file is existing ?\n",path);
        exit(1);   
    }
    int i1,i2,i3;

    fseek(file,sizeof(char),SEEK_SET);//skip header

    fread(sizecoord,sizeof(int),1,file); 
    *coord = malloc((int)sizeof(Point)*(*sizecoord));
    if(*coord == NULL)
    {
        printf("malloc error in ReadDelaunay\n");
        exit(1);
    }
    fread(*coord,sizeof(Point),*sizecoord,file);

    fread(sizetriangles,sizeof(int),1,file);//read size of triangles
    *triangles = malloc((int)sizeof(Triangle)*(*sizetriangles));
    if(*triangles == NULL)
    {
        printf("malloc error in ReadDelaunay\n");
        exit(1);
    }
    
    for(int i =0;i<*sizetriangles;i++)//read triangles
    {
        fread(&i1,sizeof(int),1,file);
        fread(&i2,sizeof(int),1,file);
        fread(&i3,sizeof(int),1,file);
        (*triangles)[i].vertices[0] = i1;
        (*triangles)[i].vertices[1] = i2;
        (*triangles)[i].vertices[2] = i3;
    }
    fclose(file);
}

void SaveMST(char path[],Point *coord,int sizecoord,Edge *edge,int sizeedge)
{
    SaveCoord(path,coord,sizecoord,'M'); //firstly, save coordinates
    FILE *fsave = fopen(path,"ab");
    if(fsave == NULL)
    {
        printf("Unable to create file %s\n",path);
        exit(1);   
    }
    
    fwrite(&sizeedge,sizeof(int),1,fsave); //write the number of edge
    fwrite(edge,sizeof(Edge),sizeedge,fsave);//write edges
    
    fclose(fsave);
}

void ReadMST(char path[],Point **coord,int *sizecoord,Edge **edge,int *sizeedge)
{
    FILE * file = fopen(path,"rb");
    if(file == NULL)
    {
        printf("Unable to load %s\nAre you sure the file is existing ?\n",path);
        exit(1);   
    }

    fseek(file,sizeof(char),SEEK_SET);//skip header

    fread(sizecoord,sizeof(int),1,file);
    *coord = malloc((int)sizeof(Point)*(*sizecoord));
    if(*coord == NULL)
    {
        printf("malloc error in ReadMST\n");
        exit(1);
    }
    fread(*coord,sizeof(Point),*sizecoord,file);

    fread(sizeedge,sizeof(int),1,file);//read size of edge
    *edge = malloc((int)sizeof(Triangle)*(*sizeedge));
    if(*edge == NULL)
    {
        printf("malloc error in ReadMST\n");
        exit(1);
    }
    fread(*edge,sizeof(Edge),*sizeedge,file); //read edges

    fclose(file);
}