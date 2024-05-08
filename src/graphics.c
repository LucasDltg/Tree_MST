#include "file.h"
#include "graphics.h"
#include "tps.h"

int _lastkey;

void Minmax(Point *coord,int sizecoord,double *minx,double *miny,double *maxx,double *maxy)//find the min/max in the array of Point
{
    *minx = 100,*miny = 5,*maxx = 0,*maxy=0;
    for(int i = 0;i<sizecoord;i++)
    {
        if(coord[i].x < *minx){*minx = coord[i].x;}
        if(coord[i].y < *miny){*miny = coord[i].y;}
        if(coord[i].x > *maxx){*maxx = coord[i].x;}
        if(coord[i].y > *maxy){*maxy = coord[i].y;}
    }
}

void DrawTree(int sizeedge,Point *coord,Edge *edge,int width,double minx,double maxx,double miny,double maxy,int space)
{
    //draw only edge, to be fast and clear
    tps_setColor(0,0,0);
    tps_background(255,255,255);
    for(int i = 0;i<sizeedge;i++)
    {
        tps_drawLine((((coord[edge[i].dest].x)/(maxx-minx)) - ((minx)/(maxx-minx)))*(width)+space,(((coord[edge[i].dest].y)/(maxy-miny)) - ((miny)/(maxy-miny)))*(width)+space,(((coord[edge[i].src].x)/(maxx-minx)) - ((minx)/(maxx-minx)))*(width)+space,(((coord[edge[i].src].y)/(maxy-miny)) - ((miny)/(maxy-miny)))*(width)+space);        
    }
    tps_render();
}

void KeyDown(int button)
{
    if(button == 'r')
    {
        _lastkey  = 'r';
    }
}

