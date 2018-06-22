/*
 * Program determining
 */
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

#include "Graph.h"

using namespace std;
using namespace graph;

//function checks whether first number in both files (amount of vertices) is highest of all other, vertices numbered <0;n-1>
void check_files(char* path_1, char* path_2)
{
    ifstream f_1,f_2;

    f_1.open(path_1, ios::in);

    int a,b;

    f_1>>a;

    while(!f_1.eof())
    {
        f_1>>b;

        if(b>=a)
        {
            f_1.close();
            throw 2;
        }
    }

    f_1.close();

    f_2.open(path_2, ios::in);

    f_2>>a;

    while(!f_2.eof())
    {
        f_2>>b;

        if(b>=a)
        {
            f_2.close();
            throw 2;
        }
    }

    f_2.close();

}

int main(int argc, char* argv[])
{
    ofstream result;
    result.open("result.txt", ios::trunc);

    try
    {
        if(argc!=3)
            throw 1;

        check_files(argv[1],argv[2]);
    }
    catch(int)
    {
        result << "Conducting algorithm did not succeed.";
        result.close();
        exit(EXIT_FAILURE);
    }


    Graph firstGraph(argv[1]);

    Graph secondGraph(argv[2]);

    unsigned int isomorphism_table[firstGraph.get_size()];

    unsigned int i;

    auto clock_start = std::chrono::high_resolution_clock::now();

    for(i=0;i<firstGraph.get_size();++i)
    {
        isomorphism_table[i]=i;
    }

    try
    {
        if(firstGraph.get_size()!=secondGraph.get_size())
            {
                throw 3;
            }

        firstGraph.check_isomorphism(secondGraph, isomorphism_table);
    }
    catch(int)
    {
        auto clock_end = std::chrono::high_resolution_clock::now();
        result << "Graphs are not isomorphic\nTime: \n"<< std::chrono::duration_cast<std::chrono::nanoseconds>(clock_end-clock_start).count();

        result.close();
    }
    catch(bool)
    {
        auto clock_end = std::chrono::high_resolution_clock::now();
        result << "Graphs are isomorphic\n";
        for(i=0;i<firstGraph.get_size();i++)
            result << i <<" --> " << isomorphism_table[i] << "\n";
        result << "Time: \n"<< std::chrono::duration_cast<std::chrono::nanoseconds>(clock_end-clock_start).count()<<endl;
        result.close();
    }

    return 0;
}
