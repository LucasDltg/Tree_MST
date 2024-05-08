#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../include/args.h"
#include "../include/delaunayTriangulation.h"
#include "../include/file.h"
#include "../include/prim.h"
#include "tps_unit_test.h"

#define MAXSIZE  4096
#define EPS 0.0000001

int __remaining_alloc = -1;

void Test_Save_Read_Binary()
{
  srand(time(NULL));
  
  int sizein = rand()%MAXSIZE;
  Point in[sizein];   //initialisation array of Point randomly
  for(int i =0;i<sizein;i++)
  {
    in[i].x = rand();
    in[i].y = rand();
  }
  SaveCoord("test",in,sizein,'B');//save

  int sizeout;
  Point *out;
  ReadSavedCoord("test",&out,&sizeout);//read

  int verif = 1;
  for(int i = 0;i<sizein;i++) //compare the 2 arrays of Point
  {
    if(in[i].x != out[i].x || in[i].y != out[i].y)
    {
      verif = 0;
    }
  }
  if(!verif)
  {
    printf("\tTest_Save_Read_Binary\t\t\tFailed\n");
  }
  else
  {
    printf("\tTest_Save_Read_Binary\t\t\tOK\n");
  }   //if the test is OK then read and save fonction are correct
  free(out);
}

void Test_Save_Read_Delaunay()
{
  srand(time(NULL));
  
  int sizein = rand()%MAXSIZE;
  Point in[sizein];           //initialisation array of Point randomly
  for(int i =0;i<sizein;i++)
  {
    in[i].x = rand();
    in[i].y = rand();
  }

  int sizetriin = rand()%MAXSIZE;
  Triangle intri[sizetriin];  //initialisation array of Triangle randomly
  for(int i =0;i<sizetriin;i++)
  {
    intri[i].vertices[0] = rand();
    intri[i].vertices[1] = rand();
    intri[i].vertices[2] = rand();
  }

  SaveDelaunay("test",in,sizein,intri,sizetriin); //save the file

  int sizeout,sizetriout;
  Point *out;
  Triangle *triout;
  ReadDelaunay("test",&out,&sizeout,&triout,&sizetriout); //read the file

  int verif = 1;
  for(int i = 0;i<sizein;i++)//compare the 2 arrays of points
  {
    if(in[i].x != out[i].x || in[i].y != out[i].y)
    {
      verif = 0;
    }
  }
  for(int i = 0;i<sizetriin;i++) //compare the 2 arrays of Triangle
  {
    if(intri[i].vertices[0] != triout[i].vertices[0] ||intri[i].vertices[1] != triout[i].vertices[1] || intri[i].vertices[2] != triout[i].vertices[2])
    {
      verif = 0;
    }
  }


  if(!verif)
  {
    printf("\tTest_Save_Read_Delaunay\t\t\tFailed\n");
  }
  else
  {
    printf("\tTest_Save_Read_Delaunay\t\t\tOK\n");
  } //if the test is OK then read and save fonction are correct

  free(out);
  free(triout);
}

void Test_Save_Read_MST()
{
  srand(time(NULL));
  
  int sizein = rand()%MAXSIZE;
  Point in[sizein];   //initialisation array of Point randomly
  for(int i =0;i<sizein;i++)
  {
    in[i].x = rand();
    in[i].y = rand();
  }
  int sizeedgein = rand()%MAXSIZE;
  Edge edgein[sizeedgein]; //initialisation array of Edge randomly
  for(int i =0;i<sizeedgein;i++)
  {
    edgein[i].src = rand();
    edgein[i].dest = rand();
  }
  SaveMST("test",in,sizein,edgein,sizeedgein);//save

  int sizeout,sizeedgeout;
  Point *out;
  Edge *edgeout;
  ReadMST("test",&out,&sizeout,&edgeout,&sizeedgeout);//read

  int verif = 1;
  for(int i = 0;i<sizein;i++) //compare the 2 arrays of Point
  {
    if(in[i].x != out[i].x || in[i].y != out[i].y)
    {
      verif = 0;
    }
  }
  for(int i = 0;i<sizeedgein;i++) //compare the 2 arrays of Edge
  {
    if(edgein[i].dest != edgeout[i].dest || edgein[i].src != edgeout[i].src)
    {
      verif = 0;
    }
  }

  if(!verif)
  {
    printf("\tTest_Save_Read_MST\t\t\tFailed\n");
  }
  else
  {
    printf("\tTest_Save_Read_MST\t\t\tOK\n");
  }   //if the test is OK then read and save fonction are correct
  
  free(out);
  free(edgeout);
}



void Test_ComputeCircumcircle()
{
  srand(time(NULL));
  int it = rand()%MAXSIZE;
  int verif = 1;

  Point coord[3];
  Triangle tri = {.vertices[0] = 0,.vertices[1] = 1,.vertices[2] = 2};
  for(int i =0;i<it;i++)
  {
    for(int j =0;j<3;j++)
    {
      double theta = (double)rand()/(double)RAND_MAX * M_PI *2; //generate circle : center (0,0) radius 1
      coord[j].x = cos(theta);
      coord[j].y = sin(theta);
    }
    ComputeCircumcircle(coord,&tri);
    
    if((tri.r - 1.0F) > EPS  || tri.x > EPS  || tri.y > EPS)
    {
      verif =0;
    }
  }

  if(!verif)
  {
    printf("\tTest_ComputeCircumcircle\t\tFailed\n");
  }
  else
  {
    printf("\tTest_ComputeCircumcircle\t\tOK\n");
  }   //if the test is OK then the function is correct
}

void Test_IsPointInCircumscribedCircle()
{
  srand(time(NULL));
  int size = rand()%MAXSIZE;
  Point coord[size];
  Triangle tri = {.x = 0, .y = 0,.r = 1,.vertices[0] = 0,.vertices[1] = 1,.vertices[2] = 2};
  
  for(int i = 0;i<size/2;i++) //generate point inside the circle
  {
    coord[i].x = cos((double)rand()/(double)RAND_MAX * 2 * M_PI);
    coord[i].y = sqrt(1 - pow(coord[i].x,2)) - EPS; //need EPS because of sqrt and pow
  }
  for(int i = size/2;i<size;i++) //generate points outside the circle
  {
    coord[i].x = rand() + 2;
    coord[i].y = rand() + 2;
  }


  int verif = 1;
  int ids[20];
  int count =0;
  for(int i = 0;i<size;i++)
  {
    IsPointInCircumscribedCircle(&tri,1,i,coord,ids,&count);
    
    if(count == 0 && i < size/2) //verify if the point is outside when i<size/2
    {
      verif = 0;
    }
    if(count == 1 && i >= size/2) //verify if the point is inside when i>=size/2
    {
      verif = 0;
    }
    count = 0;
  }
  if(!verif)
  {
    printf("\tTest_IsPointInCircumscribedCircle\tFailed\n");
  }
  else
  {
    printf("\tTest_IsPointInCircumscribedCircle\tOK\n");
  }   //if the test is OK then the function is correct
}

void Test_IsIn()
{
  srand(time(NULL));
  int it = rand()%MAXSIZE;
  int list[it],id;
  for(int i =0;i<it;i++)
  {
    list[i] = rand();
  }
  id = list[rand()%it];
  
  if(!IsIn(id,list,it))
  {
    printf("\tTest_IsIn\t\t\t\tFailed\n");
  }
  else
  {
    printf("\tTest_IsIn\t\t\t\tOK\n");
  }   //if the test is OK then the function is correct
}

void Test_ParseTriangleToList()
{
  Triangle triangles[2];          //to do this test, we take a square with an edge between vertex 1 & 2
  triangles[0].vertices[0] = 0;   //vertex 0 : up left
  triangles[0].vertices[1] = 1;   //vertex 1 : up right
  triangles[0].vertices[2] = 2;   //vertex 2 : bottom left
  triangles[1].vertices[0] = 1;
  triangles[1].vertices[1] = 2;
  triangles[1].vertices[2] = 3;   //vertex 3 : bottom right
  
  int *list, **listadj;
  ParseTriangleToList(4,2,triangles,&list,&listadj);

  if(list[0] == 2 && list[1] == 3 && list[2] == 3 && list[3] == 2)//test if the vertices have the right amout of neigboors
  {
    if(IsIn(1,listadj[0],list[0]) && IsIn(2,listadj[0],list[0]))//test on the neigboor of vertex 0
    {
      if(IsIn(0,listadj[1],list[1]) && IsIn(2,listadj[1],list[1]) && IsIn(3,listadj[1],list[1]))//same on vertex 1
      {
        if(IsIn(0,listadj[2],list[2]) && IsIn(1,listadj[2],list[2]) && IsIn(3,listadj[2],list[2])) //vertex 2
        {
          if(IsIn(1,listadj[3],list[3]) && IsIn(2,listadj[3],list[3]))
          {
            printf("\tTest_ParseTriangleToList\t\tOK\n");
          }
        }
      }
    }
  }
  else
  {
    printf("\tTest_IsIn\t\tFailed\n");
  }

  free(list);
  free(listadj[0]);
  free(listadj[1]);
  free(listadj[2]);
  free(listadj[3]);
  free(listadj);

}



void Test_WeightEdge()
{
  srand(time(NULL));

  int size = rand()%MAXSIZE;
  Point coord[size];
  for(int i =0;i<size;i++) //generate points on the circle of radius 1 and center (0,0)
  {
    double theta = 2*M_PI * (double)rand()/(double)RAND_MAX;
    coord[i].x = cos(theta);
    coord[i].y = sin(theta);
  }
  coord[0].x = 0;
  coord[0].y = 0;

  int verif = 1;
  for(int i =1;i<size;i++)
  {
    if((WeightEdge(0,i,coord) - 1) > EPS) //norm must be equal to 1
    {
      verif = 0;
    }
  }

  if(!verif)
  {
    printf("\tTest_WeightEdge\t\t\t\tFailed\n");
  }
  else
  {
    printf("\tTest_WeightEdge\t\t\t\tOK\n");
  }   //if the test is OK then the function is correct

}

void Test_Prim()
{
  Triangle triangles[2];          //Prim on the same square than Test_ParseTriangleToList
  triangles[0].vertices[0] = 0;
  triangles[0].vertices[1] = 1;
  triangles[0].vertices[2] = 2;
  triangles[1].vertices[0] = 1;
  triangles[1].vertices[1] = 2;
  triangles[1].vertices[2] = 3;
  
  int *list, **listadj;
  ParseTriangleToList(4,2,triangles,&list,&listadj);

  Point coord[4];
  coord[0].x = 0;
  coord[0].y = 1;
  coord[1].x = 1;
  coord[1].y = 1;
  coord[2].x = 0;
  coord[2].y = 0;
  coord[3].x = 1;
  coord[3].y = 0;

  Edge *edge;
  Prim(coord,4,list,listadj,&edge,10000000);

  if(edge[0].src == 0 && edge[0].dest == 1 && edge[1].src == 0 && edge[1].dest == 2 && edge[2].src == 1 && edge[2].dest == 3) //verify if the edge are good
  {  //we know that Prim start at the vertex 0, and take the edge with the minimum index
    printf("\tTest_Prim\t\t\t\tOK\n");
  }
  else
  {
    printf("\tTest_Prim\t\t\t\tFailed\n");
  }   //if the test is OK then the function is correct

  free(list);
  free(listadj[0]);
  free(listadj[1]);
  free(listadj[2]);
  free(listadj[3]);
  free(listadj);
  free(edge);
}

void Test_FindMinEdge()
{
  int size = 16;
  Point coord[size];
  for(int i =0;i<size;i++)
  {
    coord[i].x = pow(2,i);
    coord[i].y = coord[i].x;
  }
  int new = 0;
  int verif = 1;
  for(int i = size-1;i>1;i--)
  {
    FindMinEdge(&new,i,coord,size,10000000000000);
    if(new != i-1)
    {
      verif = 0;
    }
  }
  if(!verif)
  {
    printf("\tTest_FindMinEdge\t\t\tFailed\n");
  }
  else
  {
    printf("\tTest_FindMinEdge\t\t\tOK\n");
  }   //if the test is OK then the function is correct
}


int main(void)
{
  printf("----Test of functions in file.h----\n");
  Test_Save_Read_Binary();
  Test_Save_Read_Delaunay();
  Test_Save_Read_MST();

  printf("----Test of functions in delaunayTriangulation.h----\n");
  Test_ComputeCircumcircle();
  Test_IsPointInCircumscribedCircle();
  Test_IsIn();
  Test_ParseTriangleToList();

  printf("----Test of functions in prim.h----\n");
  Test_WeightEdge();
  Test_Prim();
  Test_FindMinEdge();

  return 0;
}
