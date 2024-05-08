#pragma once
#include "file.h"

#define MAXNEIGH 120

/**
 * @defgroup DelaunayTriangluation DelaunayTriangluation
 * @{
*/

/**
  * @brief Compute the circumcircle of a given triangle
  * 
  * @param coord array of Point
  * @param triangle pointer on a triangle
  */
void ComputeCircumcircle(Point *coord, Triangle *triangle);

/**
  * @brief Verify if a point is inside a circumcircle
  * 
  * @param triangles array of Triangle
  * @param counttriangles size of triangles
  * @param idnewpoint id of the point to verify if it is in a circle
  * @param coord array of Point
  * @param ids array returned with the ids of triangles who contain the new point
  * @param countid size of ids
  */
void IsPointInCircumscribedCircle(Triangle triangles[],int counttriangles,int idnewpoint, Point *coord, int ids[], int *countid);

/**
  * @brief verify if an edge is shared by 2 triangles
  * 
  * @param id1 id of the first point of the edge
  * @param id2 id of the second point of the edge
  * @param idfalsetriangle id of the triangle to skip in the comparison
  * @param triangles array of Triangle
  * @param countfalsetriangle size of falsetriangles
  * @param falsetriangles array of ids of the false triangles
  */
int SameEdge(int id1,int id2, int idfalsetriangle, Triangle triangles[], int countfalsetriangle, int falsetriangles[]);

/**
  * @brief Detect the envelope with the false triangles
  * 
  * @param falsetriangles array of ids of falsetriangles
  * @param countfalsetriangle size of falsetriangles
  * @param idsenvelop return the array of ids of Point who form the envelope
  * @param countidenvelop return the size of idsenvelop
  * @param triangles array of Triangle
  */
void DetectEnvelop(int falsetriangles[],int countfalsetriangle,int idsenvelop[],int *countidenvelop, Triangle triangles[]);

/**
  * @brief Realise a triangulation with the algorithm of Bowyer-Watson
  * 
  * @param coord pointer on an array of Point
  * @param sizecoord size of coord
  * @param triangles returned, pointer on an array of Triangle
  * @param counttriangles returned, size of triangles
  */
void TriangulationDelaunay(Point **coord,int sizecoord,Triangle **triangles,int *counttriangles);

/**
  * @brief Verify if an element is in a list
  * 
  * @param elem element
  * @param list the list
  * @param size size of the list  
  */
int IsIn(int elem,int *list,int size);

/**
  * @brief parse edge of triangles to adjacency lists
  * 
  * @param sizecoord number of vertices
  * @param sizetriangle size of triangles
  * @param triangles array of Triangle
  * @param list returned, list which contain the number of neighboor of each point
  * @param listadj returned, list of list which contain the id of neighboors
  */
void ParseTriangleToList(int sizecoord,int sizetriangle,Triangle *triangles,int **list,int ***listadj);

/**@}*/