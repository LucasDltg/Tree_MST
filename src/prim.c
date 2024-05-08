#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "prim.h"

double WeightEdge(int id1, int id2,Point *coord)
{
    //compute the weight of an edge (norm formula)
    return ((coord[id1].x - coord[id2].x)*(coord[id1].x - coord[id2].x))+((coord[id1].y - coord[id2].y)*(coord[id1].y - coord[id2].y));
}

void FindMinEdge(int *idnewvertex,int idantvertex,Point *coord,int sizecoord,double maxweight)
{
    double min = 100000000000;
    double temp;
    int i;

    for(i = 0;i<idantvertex;i++)
    {
        temp = WeightEdge(idantvertex,i,coord);
        if(temp < min)
        {
            min = temp;
            *idnewvertex = i;
        }
    }
    i++;//skip the comparison between antvertex and itself
    for(;i<sizecoord;i++)
    {
        temp = WeightEdge(idantvertex,i,coord);
        if(temp < min)
        {
            min = temp;
            *idnewvertex = i;
        }
    }
    if(min>maxweight)
    {
        *idnewvertex = idantvertex;
    }
}

void Prim(Point *coord,int sizecoord, int *list, int **listadj,Edge **edge, double maxweight)
{
    int *idsvertex = calloc(sizecoord,sizeof(int)); //store ids of the vertex in the tree
    *edge = malloc((sizecoord-1)*sizeof(Edge));
    int *isin = calloc(sizecoord,sizeof(int));
    if(idsvertex == NULL || *edge == NULL || isin == NULL)
    {
        printf("error malloc in Prim\n");
        exit(1);
    }
    int pos=0;
    int sizevertex = 0; //size of the array
    int idnewvertex =  0, idantvertex =0;//store the vertex of the actual edge
    
     
    for(int i = 0;i<sizecoord-1;i++) //Prim
    {
        idsvertex[sizevertex] = idnewvertex; //add vertex to the tree
        sizevertex++;
        isin[idnewvertex] = 1;
        FindMinEdgeList(&idnewvertex,&idantvertex,idsvertex,sizevertex,coord,list,listadj,isin,maxweight);
        (*edge)[pos].src =idantvertex;
        (*edge)[pos].dest = idnewvertex;
        pos++;
    }
    isin[idnewvertex] = 1;
    pos--;


    for(int i =0;i<sizecoord;i++)               //part normaly optional but there is a bug with Delaunay's Triangulation, see desc.md 
    {                                           //Find the minimum weight for missing vertice in Delaunay's Triangulation (on these datas 3 vertices)
        if(isin[i] == 0)
        {
            FindMinEdge(&idnewvertex,i,coord,sizecoord,maxweight);
            (*edge)[pos].src =idnewvertex;
            (*edge)[pos].dest = i;
            pos--;
        }
    }

    free(idsvertex);
    free(isin);
} 

void FindMinEdgeList(int *idnewvertex,int *idantvertex,int *idsvertex,int sizevertex,Point *coord,int *list, int **listadj,int *isin,double maxweight)
{
    double min = 100000000000;
    double temp;
    int ant,new;
    
    for(int i =0;i<sizevertex;i++) //for each vertex
    {
        for(int j =0;j<list[idsvertex[i]];j++) //for each neighbour 
        {
            if(isin[listadj[idsvertex[i]][j]] == 0) //if the vertex isn't in the tree
            {
                temp = WeightEdge(idsvertex[i],listadj[idsvertex[i]][j],coord); 
                if(temp < min)
                {
                    min = temp;
                    ant = idsvertex[i];
                    new = listadj[idsvertex[i]][j];

                }
            }
        }
    }
    if(min<maxweight)
    {
        *idantvertex = ant;
        *idnewvertex = new;
    }
}
