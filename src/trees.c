#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "args.h"
#include "file.h"
#include "delaunayTriangulation.h"
#include "prim.h"
#include "graphics.h"
#include <tps.h>
#include <signal.h>

#define MAXSIZECSV 10000000
#define WIDTH 900

int main(int argc, char **argv)
{
    time_t t = time(NULL); //to time execution time
    
    signal(SIGINT,Siginthandler);
    
    Options_t opt;
    int sizecoord = 0;
    Point *coord;
    Triangle *triangles;
    int sizetriangles = 0;
    int *list;
    int **listadj;
    Edge *edge;
    int sizeedge =0;

    parse_args(argc,argv,&opt);


    if(opt.pathdatabase != NULL) //if we work on the .CSV
    {
        coord = malloc(sizeof(Point)*MAXSIZECSV);
        if(coord == NULL)
        {
            printf("error malloc in main\n");
            exit(1);
        }
        ExtractCoord(coord,&sizecoord,opt.pathdatabase);
        printf("******************************************\n\t Extraction from CSV done\n******************************************\n");
        
        if(opt.savebinfile != NULL)
        {
            SaveCoord(opt.savebinfile,coord,sizecoord,'B');
            printf("******************************************\n\t   Binary file saved\n******************************************\n");
        }

        TriangulationDelaunay(&coord,sizecoord,&triangles,&sizetriangles);
        printf("******************************************\n\tDelaunay triangulation done\n******************************************\n");

        if(opt.savedelaunay != NULL)
        {
            SaveDelaunay(opt.savedelaunay,coord,sizecoord,triangles,sizetriangles);
            printf("******************************************\n\t    Delaunay file saved\n******************************************\n");
        }

        ParseTriangleToList(sizecoord,sizetriangles,triangles,&list,&listadj);
        Prim(coord,sizecoord,list,listadj,&edge,opt.limitweight);
        printf("******************************************\n\t\tPrim done\n******************************************\n");

        SaveMST(opt.savefinaloutput,coord,sizecoord,edge,sizecoord -1);
        printf("******************************************\n\t\tMST saved\n******************************************\n");

        free(coord);
        free(triangles);
        free(list);
        for(int i = 0;i<sizecoord;i++)
        {
            free(listadj[i]);
        }
        free(listadj);
        free(edge);
    }   
    else if(opt.loadfile != NULL)
    {        
        if(opt.header == 'B') //if the loading file is the binary file of CSV
        {
            ReadSavedCoord(opt.loadfile,&coord,&sizecoord);
            printf("******************************************\n\t    Binary file loaded\n******************************************\n");
            
            TriangulationDelaunay(&coord,sizecoord,&triangles,&sizetriangles);
            printf("******************************************\n\tDelaunay triangulation done\n******************************************\n");

            if(opt.savedelaunay != NULL)
            {
                SaveDelaunay(opt.savedelaunay,coord,sizecoord,triangles,sizetriangles);
                printf("******************************************\n\t    Delaunay file saved\n******************************************\n");
            }

            ParseTriangleToList(sizecoord,sizetriangles,triangles,&list,&listadj);
            Prim(coord,sizecoord,list,listadj,&edge,opt.limitweight);
            printf("******************************************\n\t\tPrim done\n******************************************\n");

            SaveMST(opt.savefinaloutput,coord,sizecoord,edge,sizecoord -1);
            printf("******************************************\n\t\tMST saved\n******************************************\n");

            free(coord);
            free(triangles);
            free(list);
            for(int i = 0;i<sizecoord;i++)
            {
                free(listadj[i]);
            }
            free(listadj);
            free(edge);
        }
        else if(opt.header == 'D') //if the loading file is the delaunay file
        {
            ReadDelaunay(opt.loadfile,&coord,&sizecoord,&triangles,&sizetriangles);
            printf("******************************************\n\t   Delaunay file loaded\n******************************************\n");

            ParseTriangleToList(sizecoord,sizetriangles,triangles,&list,&listadj);
            Prim(coord,sizecoord,list,listadj,&edge,opt.limitweight);
            printf("******************************************\n\t\tPrim done\n******************************************\n");

            SaveMST(opt.savefinaloutput,coord,sizecoord,edge,sizecoord -1);
            printf("******************************************\n\t\tMST saved\n******************************************\n");

            free(coord);
            free(triangles);
            free(list);
            for(int i = 0;i<sizecoord;i++)
            {
                free(listadj[i]);
            }
            free(listadj);
            free(edge);
        }
        else if(opt.header == 'M') //if the loading file is the minimum spanning tree then display
        {
            ReadMST(opt.loadfile,&coord,&sizecoord,&edge,&sizeedge);
            
            double minx,miny,maxx,maxy;
            Minmax(coord,sizecoord,&minx,&miny,&maxx,&maxy);
            double minxt = minx,minyt = miny,maxxt = maxx,maxyt = maxy;
            
            tps_createWindow("Minimum spanning tree",WIDTH,WIDTH);
            
            DrawTree(sizeedge,coord,edge,WIDTH-10,minx,maxx,miny,maxy,5);
            int sizefactor = 2;
            

            while(tps_isRunning())
            {            
                int button = tps_mouseIsClicked();                  
                if(button & SDL_BUTTON(SDL_BUTTON_LEFT))
                {  
                    if(sizefactor < 10) //block a zoom too large
                    {
                        sizefactor+=2;
                        int x,y;
                        tps_getMousePosition(&x,&y);
                        double xt = (((x-5)*(maxxt-minxt)/(WIDTH-10)) + minxt);
                        double yt = (((y-5)*(maxyt-minyt)/(WIDTH-10)) + minyt);        

                        double minxtt = minxt,maxxtt = maxxt,minytt = minyt,maxytt = maxyt;
                        minxt = xt-(maxxtt-minxtt)/sizefactor;
                        maxxt = xt+(maxxtt-minxtt)/sizefactor;
                        minyt = yt-(maxytt-minytt)/sizefactor;
                        maxyt = yt+(maxytt-minytt)/sizefactor;

                        DrawTree(sizeedge,coord,edge,WIDTH,minxt,maxxt,minyt,maxyt,0);
                    }
                }
                
                
                tps_onKeyDown(&KeyDown);
                if(_lastkey == 'r')
                {
                    DrawTree(sizeedge,coord,edge,WIDTH-10,minx,maxx,miny,maxy,5);
                    sizefactor = 2;
                    minxt = minx,minyt = miny,maxxt = maxx,maxyt = maxy;
                }
                _lastkey = 0;
            }
            tps_closeWindow();

            free(coord);
            free(edge);
        }
    }

    
    
    printf("Executed in %ld seconds\n",time(NULL)-t);
    return 0;

    
}