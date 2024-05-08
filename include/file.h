#pragma once

/**
 * @defgroup IOFile IOFile
 * @{
*/

/**
 * @brief Struct who represents a point in 2D
 */
typedef struct Point
{
    double x;
    double y;
}Point;

/**
 * @brief Struct who represents a Triangle in 2D and its circumsribed circle
 */
typedef struct Triangle
{
    int vertices[3];
    double x;
    double y;
    double r; //radius squared
}Triangle;

/**
 * @brief Struct who represents an edge
 */
typedef struct Edge
{
    int src;
    int dest;
}Edge;

/**
  * @brief handle ctrl+C
  */
void Siginthandler();

/**
  * @brief extract coordinates from a CSV file
  * 
  * @param coord array of Point
  * @param sizecoord size of the array
  * @param path path to the CSV file
  */
void ExtractCoord(Point *coord, int *sizecoord, char path[]);

/**
  * @brief save an array of point in a binary file
  * In the format : -An header 'B'
  *                 -Number of Point (int)
  *                 -Array of Point (2*double)
  * 
  * @param path path to save file
  * @param coord array of Point
  * @param sizecoord size of the array
  * @param header header of the file
  */
void SaveCoord(char path[],Point *coord,int sizecoord, char header);

/**
  * @brief read coordinates in a binary file
  * 
  * @param path path to the file to read
  * @param coord adress of an array Point
  * @param sizecoord pointer on the size of the array
  */
void ReadSavedCoord(char path[], Point **coord, int *sizecoord);

/**
  * @brief save Delaunay triangulation (coordinates of points and triangles)
  * In the format : -An header 'D'
  *                 -Number of Point (int)
  *                 -Array of Point (2*double)
  *                 -Number of Triangle (int)
  *                 -Array of Triangle (3*int + 3*double)
  * 
  * @param path path to the save file
  * @param coord array of Point
  * @param sizecoord size of the array coord
  * @param triangles array of Triangle
  * @param sizetriangles size of the array triangles
  */
void SaveDelaunay(char path[],Point *coord,int sizecoord,Triangle *triangles,int sizetriangles);

/**
  * @brief read Delaunay triangulation ina binary file (coordinates of points and triangles)
  * 
  * @param path path to the file to read
  * @param coord adress of an array of Point
  * @param sizecoord pointer on the size of the array coord
  * @param triangles adress of an array of Triangle
  * @param sizetriangles pointer on the size of the array triangles
  */
void ReadDelaunay(char path[], Point **coord,int *sizecoord,Triangle **triangles, int *sizetriangles);

/**
  * @brief save minimum spanning tree
  * In the format : -An header 'M'
  *                 -Number of Point (int)
  *                 -Array of Point (2*double)
  *                 -Number of Edge (int)
  *                 -Array of Edge (2*int)
  * 
  * @param path path to the save file
  * @param coord array of Point
  * @param sizecoord size of the array coord
  * @param edge array of edge
  * @param sizeedge size of the array edge
  */
void SaveMST(char path[],Point *coord,int sizecoord,Edge *edge,int sizeedge);

/**
  * @brief read minimum spanning tree in a binary file (coordinates of points and edges)
  * 
  * @param path path to the file to read
  * @param coord adress of an array of Point
  * @param sizecoord pointer on the size of the array coord
  * @param edge adress of an array of edge
  * @param sizeedge pointer on the size of the array edge
  */
void ReadMST(char path[],Point **coord,int *sizecoord,Edge **edge,int *sizeedge);

/**@}*/