#include <stdio.h>
#include <stdlib.h>
#include "delaunayTriangulation.h"
#include "file.h"

void IsPointInCircumscribedCircle(Triangle triangles[],int counttriangles,int idnewpoint, Point *coord, int ids[], int *countid)
{
    for(int i =0;i<counttriangles;i++) //verify for each triangle in the triangulation if the point is inside the circumcircle
    {    
        if((triangles[i].x-coord[idnewpoint].x)*(triangles[i].x-coord[idnewpoint].x)+(triangles[i].y-coord[idnewpoint].y)*(triangles[i].y-coord[idnewpoint].y) < triangles[i].r) //circle equation
        {        
            ids[*countid]  = i; //add the id of the triangle to the array
            *countid +=1;
        }
    }
} 

void ComputeCircumcircle(Point *coord, Triangle *triangle)
{
    double  A = coord[triangle->vertices[1]].x - coord[triangle->vertices[0]].x,
            B = coord[triangle->vertices[1]].y - coord[triangle->vertices[0]].y,
            C = coord[triangle->vertices[2]].x - coord[triangle->vertices[0]].x,
            D = coord[triangle->vertices[2]].y - coord[triangle->vertices[0]].y,
            E = A * (coord[triangle->vertices[0]].x + coord[triangle->vertices[1]].x) + B * (coord[triangle->vertices[0]].y + coord[triangle->vertices[1]].y),
            F = C * (coord[triangle->vertices[0]].x + coord[triangle->vertices[2]].x) + D * (coord[triangle->vertices[0]].y + coord[triangle->vertices[2]].y),
            G = 2 * (A * (coord[triangle->vertices[2]].y - coord[triangle->vertices[1]].y) - B * (coord[triangle->vertices[2]].x - coord[triangle->vertices[1]].x)),
            dx,dy;
            //equation to find the center of the circle and the radius
        
        triangle->x = (D*E - B*F) / G;
        triangle->y = (A*F - C*E) / G;

        dx = triangle->x - coord[triangle->vertices[0]].x;
        dy = triangle->y - coord[triangle->vertices[0]].y;
        triangle->r = dx * dx + dy * dy;
}

int SameEdge(int id1,int id2, int idfalsetriangle, Triangle triangles[], int countfalsetriangle, int falsetriangles[])
{
    int i =0;
    int count;
    for(i = 0;i< idfalsetriangle;i++) //for each triangle concerned
    {
        count =0;
        for(int j = 0;j<3;j++) //for each vertices of this triangle
        {
            //verify if the falsetriangle and the actual triangle share an edge
            if(triangles[falsetriangles[i]].vertices[j] == triangles[falsetriangles[idfalsetriangle]].vertices[id1] || triangles[falsetriangles[i]].vertices[j] == triangles[falsetriangles[idfalsetriangle]].vertices[id2])
            {    
                count++;
            } 
        }
        if(count == 2){return i;}
    }
    i++; //skip the comparison between falsetriangle and itself
    for(;i< countfalsetriangle;i++) //same than previously
    {
        count =0;
        for(int j = 0;j<3;j++)
        {
            if(triangles[falsetriangles[i]].vertices[j] == triangles[falsetriangles[idfalsetriangle]].vertices[id1] || triangles[falsetriangles[i]].vertices[j] == triangles[falsetriangles[idfalsetriangle]].vertices[id2])
            {
                count++;
            } 
        }
        if(count == 2){return i;}
    } 
    return -1;
}

void DetectEnvelop(int falsetriangles[],int countfalsetriangle,int idsenvelop[],int *countidenvelop, Triangle triangles[])
{
    if(countfalsetriangle == 1)//if the hole is a triangle, return idsenvelop in this format 1-2 2-3 3-1
    {
        *countidenvelop = 6;
        idsenvelop[0] = triangles[falsetriangles[0]].vertices[0];
        idsenvelop[1] = triangles[falsetriangles[0]].vertices[1];
        idsenvelop[2] = triangles[falsetriangles[0]].vertices[1];
        idsenvelop[3] = triangles[falsetriangles[0]].vertices[2];
        idsenvelop[4] = triangles[falsetriangles[0]].vertices[2];
        idsenvelop[5] = triangles[falsetriangles[0]].vertices[0];
        return;
    }

    for(int i =0;i<countfalsetriangle;i++) //for each triangle concerned
    {
        for(int j = 0;j<3;j++) //for each vertex
        {
            if(SameEdge(j,(j+1)%3,i,triangles,countfalsetriangle,falsetriangles) == -1) //if they don't share an edge
            {
                idsenvelop[*countidenvelop] = triangles[falsetriangles[i]].vertices[j];
                idsenvelop[*countidenvelop+1] = triangles[falsetriangles[i]].vertices[(j+1)%3]; 
                *countidenvelop +=2;
                //add the edge to the envelop
            } 

        } 
    }
} 

void TriangulationDelaunay(Point **coord,int sizecoord,Triangle **triangles,int *counttriangles)
{  
    *coord = realloc(*coord,(sizecoord + 3)*sizeof(Point)); //allocate memory for the vertices of the super triangle
    if(*coord == NULL)
    {
        printf("malloc error in TriangulationDelaunay\n");
        exit(1);
    }
    (*coord)[sizecoord].x =-1000000000; // != infinity
    (*coord)[sizecoord].y =-1000000000;
    (*coord)[sizecoord+1].x =0;
    (*coord)[sizecoord+1].y =1000000000;
    (*coord)[sizecoord+2].x =1000000000;
    (*coord)[sizecoord+2].y =-1000000000;

    Triangle SuperTriangle = {.vertices[0] = sizecoord,.vertices[1] = sizecoord+1,.vertices[2] = sizecoord+2}; //create supertriangle
    ComputeCircumcircle(*coord,&SuperTriangle); //circumcircle of super triangle

    Triangle *trianglescp = malloc(430000*(int)sizeof(Triangle)); //store the triangles of the triangultions
    if(trianglescp == NULL)
    {
        printf("malloc error in TriangulationDelaunay\n");
        exit(1);
    }
    trianglescp[0] = SuperTriangle;//add supertriangle
    *counttriangles = 1;                    
    
    int falsetriangles[100000];       //array of triangles which are no longer valid
    int countfalsetriangle = 0;

    int idsenvelop[300000];        //array of vertices of the envelop
    int countidenvelop = 0;

    for(int i =0;i<sizecoord;i++) //add point 1 by 1
    {
        IsPointInCircumscribedCircle(trianglescp,*counttriangles,i,*coord,falsetriangles, &countfalsetriangle);//get ids of triangles who contains the new point
        DetectEnvelop(falsetriangles,countfalsetriangle,idsenvelop,&countidenvelop,trianglescp);//get ids of points which constitute the envelope
        
        int trianglerempl = 0;
        int trianglerempl2 = 0;
        for(;trianglerempl<countfalsetriangle;trianglerempl++)//replace the old triangles with new ones
        {
            Triangle temp = {.vertices[0] = i,.vertices[1] = idsenvelop[trianglerempl2],.vertices[2] = idsenvelop[trianglerempl2+1]}; 
            ComputeCircumcircle(*coord,&temp);  //compute the circumcircle of the triangle
            trianglescp[falsetriangles[trianglerempl]] = temp;
            trianglerempl2+=2;
        }
        for(;trianglerempl2<countidenvelop;trianglerempl2+=2)//add the new ones at the end of the array
        {
            Triangle temp = {.vertices[0] = i,.vertices[1] = idsenvelop[trianglerempl2],.vertices[2] = idsenvelop[trianglerempl2+1]};
            ComputeCircumcircle(*coord,&temp);
            trianglescp[*counttriangles] = temp;
            *counttriangles+=1;
            
        }
        countfalsetriangle = 0;
        countidenvelop = 0;
        
    }
    
    int ct = 0;
    for(int i =0;i<*counttriangles;i++) //for each triangle in the triangulation
    {
        if(!((trianglescp[i].vertices[0]>=sizecoord)||(trianglescp[i].vertices[1]>=sizecoord)||(trianglescp[i].vertices[2]>=sizecoord)))//if the triangles don't share any vertex with super triangle
        {
            ct++;
        }
    }
    *triangles = malloc(sizeof(Triangle)*(*counttriangles));//malloc an array of triangles of the exact size
    if(*triangles == NULL)
    {
        printf("malloc error in TriangulationDelaunay\n");
        exit(1);
    }
    
    int pos = 0;
    for(int i =0;i<*counttriangles;i++)
    {
        if(!((trianglescp[i].vertices[0]>=sizecoord)||(trianglescp[i].vertices[1]>=sizecoord)||(trianglescp[i].vertices[2]>=sizecoord)))
        {
            (*triangles)[pos] = trianglescp[i]; //add good triangles to final triangulation
            pos++;
        }
    }
    *counttriangles = ct;
    free(trianglescp);
}

int IsIn(int elem,int *list,int size)
{
    for(int i = 0;i<size;i++)//verify if vertex is in list
    {
        if(list[i] == elem)
        {
            return 1;
        } 
    } 
    return 0;
}

void ParseTriangleToList(int sizecoord,int sizetriangle,Triangle *triangles,int **list,int ***listadj)
{
    *list = calloc(sizecoord,sizeof(int));
    *listadj = calloc(sizecoord,sizeof(int*));
    if(*list == NULL || *listadj == NULL)
    {
        printf("malloc error in ParseTriangleToList\n");
        exit(1);
    }
    for(int i = 0;i<sizecoord;i++)
    {
        (*listadj)[i] = calloc(MAXNEIGH,sizeof(int));
        if((*listadj)[i] == NULL)
        {
            printf("malloc error in ParseTriangleToList\n");
            exit(1);
        }
        for(int j =0;j<MAXNEIGH;j++)
        {
            (*listadj)[i][j] = -1;
        }
    }
    for(int i=0;i<sizetriangle;i++) //for each triangle
    {
        if(!IsIn(triangles[i].vertices[1],(*listadj)[triangles[i].vertices[0]],(*list)[triangles[i].vertices[0]]))//verify if vertex of the triangle is inside the adjacency list
        {
            (*listadj)[triangles[i].vertices[0]][(*list)[triangles[i].vertices[0]]] = triangles[i].vertices[1]; //add the vertex to the adjacency list
            (*list)[triangles[i].vertices[0]] +=1;
            (*listadj)[triangles[i].vertices[1]][(*list)[triangles[i].vertices[1]]] = triangles[i].vertices[0]; //same, we store 1-2 and 2-1
            (*list)[triangles[i].vertices[1]] +=1;
        }
        if(!IsIn(triangles[i].vertices[2],(*listadj)[triangles[i].vertices[0]],(*list)[triangles[i].vertices[0]]))//same for an other vertex
        {
            (*listadj)[triangles[i].vertices[0]][(*list)[triangles[i].vertices[0]]] = triangles[i].vertices[2];
            (*list)[triangles[i].vertices[0]] +=1;
            (*listadj)[triangles[i].vertices[2]][(*list)[triangles[i].vertices[2]]] = triangles[i].vertices[0];
            (*list)[triangles[i].vertices[2]] +=1;
        }
        if(!IsIn(triangles[i].vertices[2],(*listadj)[triangles[i].vertices[1]],(*list)[triangles[i].vertices[1]]))//same for the last vertex
        {
            (*listadj)[triangles[i].vertices[1]][(*list)[triangles[i].vertices[1]]] = triangles[i].vertices[2];
            (*list)[triangles[i].vertices[1]] +=1;
            (*listadj)[triangles[i].vertices[2]][(*list)[triangles[i].vertices[2]]] = triangles[i].vertices[1];
            (*list)[triangles[i].vertices[2]] +=1;
        }
    }
}