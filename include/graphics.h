#pragma once
#include "file.h"

/**
 * @defgroup Graphics Graphics
 * @{
*/

/**
  * @brief the last key pressed by the user
  */
extern int _lastkey;

/**
  * @brief function which give minimums and maximums of an array of Point
  * 
  * @param coord array of Point
  * @param sizecoord size of the array of Point
  * @param minx pointer on the min of x coordinates
  * @param miny pointer on the min of y coordinates
  * @param maxx pointer on the max of x coordinates
  * @param maxy pointer on the max of y coordinates 
  */
void Minmax(Point *coord,int sizecoord,double *minx,double *miny,double *maxx,double *maxy);

/**
  * @brief Draw edges stored in edge
  * 
  * @param sizeedge size of the array of edges
  * @param coord array of Point
  * @param edge array of edges
  * @param width size of the window (must be a square)
  * @param minx pointer on the min of x coordinates
  * @param maxx pointer on the max of x coordinates
  * @param miny pointer on the min of y coordinates
  * @param maxy pointer on the max of y coordinates
  * @param space size of the border at the first print
  */
void DrawTree(int sizeedge,Point *coord,Edge *edge,int width,double minx,double maxx,double miny,double maxy,int space);

/**
 * @brief give the last key pressed
 * 
 * @param button last button pressed
 */
void KeyDown(int button);

/**@}*/
