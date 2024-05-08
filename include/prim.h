#pragma once

/**
 * @defgroup Prim Prim
 * @{
*/

/**
  * @brief compute the weight of a given edge (norm squared)
  * 
  * @param id1 first point
  * @param id2 second point  
  * @param coord array of Point
  */
double WeightEdge(int id1, int id2,Point *coord);

/**
  * @brief Prim's algorithm
  * 
  * @param coord array of Point
  * @param sizecoord size of the array of Point
  * @param list list which contain the number of neighboor of each point
  * @param listadj list of list which contain the id of neighboors
  * @param edge adresse of an array of edge
  * @param maxweight weight max of an edge
  */
void Prim(Point *coord,int sizecoord, int *list, int **listadj,Edge **edge, double maxweight);

/**
  * @brief Find the minimum weigth of edges with an adjacency list
  * 
  * @param idnewvertex pointer on the id of the point who minimize the weight, it's outside the tree
  * @param idantvertex same but this point is in the tree
  * @param idsvertex store the ids of Point who are in the tree
  * @param sizevertex size of idsvertex
  * @param coord array of Point
  * @param list list which contain the number of neighboor of each point
  * @param listadj list of list which contain the id of neighboors
  * @param isin array must contain 1 if the vertex is in the tree, 0 else
  * @param maxweight weight max of an edge
  */
void FindMinEdgeList(int *idnewvertex,int *idantvertex,int *idsvertex,int sizevertex,Point *coord,int *list, int **listadj,int *isin,double maxweight);

/**
  * @brief Find the minimum weight between idantvertex and the rest of point in coord
  * 
  * @param idnewvertex at the end, store the id of the vertex which minimize the weight
  * @param idantvertex starting point to find the minimum weight
  * @param coord array of Point
  * @param sizecoord size of the array of Point
  * @param maxweight weight max of an edge
  */
void FindMinEdge(int *idnewvertex,int idantvertex,Point *coord,int sizecoord, double maxweight);

/**@}*/