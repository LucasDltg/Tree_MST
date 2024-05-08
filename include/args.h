#pragma once

/**
 * @defgroup Args Args
 * @{
*/

/**
 * @brief Struct with options
 */
typedef struct Options_t {
   
    char *pathdatabase;         //path to .CSV
    char *savebinfile;          //path to CSV parsed in binary
    char *savedelaunay;         //path to Delaunay's triangulation , if null triangulation isn't save
    char *savefinaloutput;      //path to Minimum Spanning Tree
    char *loadfile;             //path to file to load
    char header;
    double limitweight;
   
}Options_t;

/**
 * @brief parse the arguments given when the game is started
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @param opt pointer on an Options_t struct
 * 
 * @return 0 if everything went well
 */
int parse_args(int argc, char **argv, Options_t *opt);

/**
 * @brief ask if the user wants to continue
 */
void ask();

/**@}*/