#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <vector>
#include <iterator>
#include "Graph.h"

using namespace graph;

Graph::Graph(char* path)
{
    std::ifstream read;
    read.open(path, std::ios::in);

    read >> vertex_count;
    unsigned int i,j;

    matrix=new bool*[vertex_count];

    for(i=0;i<vertex_count;++i)
    {
        matrix[i]= new bool[vertex_count];
    }

    self_connected = new bool[vertex_count];
    neighbor_count = new unsigned int[vertex_count];
    vertex_by_neighbors_array = new unsigned int[vertex_count];
    vertex_by_neighbors_array_self_connected = new unsigned int[vertex_count];

    for(i=0;i<vertex_count;++i)
    {
        vertex_by_neighbors_array[i]=0;
        vertex_by_neighbors_array_self_connected[i]=0;
        neighbor_count[i]=0;
        self_connected[i]=false;
    }

    for(i=0;i<vertex_count;++i)
    {
        for(j=0;j<vertex_count;++j)
        {
            matrix[i][j]=false;
        }
    }

    unsigned int v,u;

    while(read >> v >> u)
    {
        edges.push_back(std::make_pair(v,u));
        matrix[v][u]=true;
        matrix[u][v]=true;
        if(u!=v)
        {
            neighbor_count[u]++;
            neighbor_count[v]++;
        }
        else
        {
            self_connected[v]=true;
        }
    }
    read.close();

    for(i=0;i<vertex_count;++i)
    {
        vertex_by_neighbors_array[neighbor_count[i]]++;
        if(self_connected[i]==true)
            vertex_by_neighbors_array_self_connected[neighbor_count[i]]++;
    }

    for(i=0;i<20;++i)
    {
        if(i==0)
            factorial[i]=1;
        else
            factorial[i]=i*factorial[i-1];
    }

}

Graph::Graph(const Graph& other)
{
    vertex_count=other.vertex_count;

    self_connected = new bool[vertex_count];
    neighbor_count = new unsigned int[vertex_count];
    vertex_by_neighbors_array = new unsigned int[vertex_count];
    vertex_by_neighbors_array_self_connected = new unsigned int[vertex_count];

    matrix=new bool*[vertex_count];

    for(unsigned int k=0;k<vertex_count;++k)
    {
        matrix[k]= new bool[vertex_count];
    }

    for(unsigned int i=0;i<vertex_count;++i)
    {
        self_connected[i] = other.self_connected[i];
        neighbor_count[i] = other.neighbor_count[i];
        vertex_by_neighbors_array[i]=other.vertex_by_neighbors_array[i];
        vertex_by_neighbors_array_self_connected[i]=vertex_by_neighbors_array_self_connected[i];
        for(unsigned int j=0;j<vertex_count;++j)
        {
            matrix[i][j]=other.matrix[i][j];
        }
    }

    for(auto it=other.edges.begin();it!=other.edges.end();++it)
        edges.push_back(std::make_pair((*it).first,(*it).second));

    for(auto it=other.permutation.begin();it!=other.permutation.end();++it)
        permutation.push_back((*it));

    for(int j=0;j<20;++j)
        factorial[j]=other.factorial[j];
}

Graph::~Graph()
{
    delete[] vertex_by_neighbors_array;
    delete[] vertex_by_neighbors_array_self_connected;
    delete[] self_connected;
    delete[] neighbor_count;

    for(unsigned int i=0;i<vertex_count;++i)
        delete[] matrix[i];
    delete[] matrix;
    edges.erase(edges.begin(),edges.end());
    permutation.erase(permutation.begin(),permutation.end());
}

void Graph::check_isomorphism(Graph& other, unsigned int* table)
{
    unsigned int i,j;
    bool flag=true;

    for(i=0;i<vertex_count;++i)
    {
        if(vertex_by_neighbors_array[i]!=other.vertex_by_neighbors_array[i] || vertex_by_neighbors_array_self_connected[i]!=other.vertex_by_neighbors_array_self_connected[i])
        {
                throw 4;
        }
    }

    for(i=0;i<vertex_count;++i)
    {
        for(j=0;j<vertex_count;++j)
        {
            if(matrix[i][j]!=other.matrix[i][j])
            {
                flag=false;
                break;
            }
        }
        if(flag==false)
            break;
    }

    if(flag==true)
        throw flag;

    for(i=1;i<vertex_count;++i)
    {
        if(vertex_by_neighbors_array[i]!=0)
        {
            for(j=0;j<vertex_count;++j)
            {
                if(neighbor_count[j]==i)
                {
                    permutation.push_back(j);
                }
            }
        }
    }

    j=0;
    do
    {
        ++j;
    }while(vertex_by_neighbors_array[j]==0);

    permutate(other,permutation.end(),permutation.begin(),permutation.begin()+vertex_by_neighbors_array[j],j,table);
    throw 6;
}

void Graph::permutate(Graph& other,std::vector<int>::iterator v_end, std::vector<int>::iterator beg, std::vector<int>::iterator _end, unsigned int j, unsigned int* table)
{
    unsigned int k=j;
    do
    {
        ++k;
    }while(k<vertex_count && vertex_by_neighbors_array[k]==0);

    for(unsigned int i=0;i<factorial[vertex_by_neighbors_array[j]];++i)
    {
        make_matrix(other,table);
        std::next_permutation(beg,beg+vertex_by_neighbors_array[j]);
        if(_end!=v_end)
            permutate(other,v_end,_end,_end+vertex_by_neighbors_array[k],k,table);
    }
}

void Graph::make_matrix(Graph& other, unsigned int* table)
{
    auto it=permutation.begin();
    unsigned int i,j,k,tab[vertex_count];
    bool t[vertex_count], flag=true;

    for(i=0;i<vertex_count;++i)
        t[i]=false;

    it=permutation.begin();

    for(i=0;i<vertex_count;++i)
    {
        if(neighbor_count[i]==0)
        {
            for(j=0;j<vertex_count;++j)
            {
                if(other.neighbor_count[j]==0 && self_connected[i]==other.self_connected[j] && t[j]==false)
                {
                    t[j]=true;
                    tab[j]=i;
                }
            }
        }
    }

    for(i=0;i<vertex_count;++i)
    {
        if(vertex_by_neighbors_array[i] !=0)
        {
            for(j=vertex_by_neighbors_array[i];j>0;--j)
            {
                for(k=0;k<vertex_count;++k)
                {
                    if(other.neighbor_count[k]==i && self_connected[(*it)]==other.self_connected[k] && t[k]==false)
                    {
                        tab[k]=(*it);
                        ++it;
                        t[k]=true;
                    }
                }

            }
        }
    }

    for(i=0;i<vertex_count;++i)
        for(j=0;j<vertex_count;++j)
            if(tab[j]==i)
                table[i]=j;

    bool new_matrix[vertex_count][vertex_count];

    for(i=0;i<vertex_count;++i)
        for(j=0;j<vertex_count;++j)
            new_matrix[i][j]=false;

    for(auto iter=edges.begin();iter!=edges.end();++iter)
    {
        new_matrix[table[(*iter).first]][table[(*iter).second]]=true;
        new_matrix[table[(*iter).second]][table[(*iter).first]]=true;
    }

    //transposing result matrix to complete algotithm
    for(i=0;i<vertex_count;++i)
    {
        for(j=0;j<vertex_count;++j)
        {
            if(new_matrix[i][j]!=other.matrix[i][j])
            {
                flag=false;
                break;
            }
        }
        if(flag==false)
            break;
    }

    if(flag==true)
        throw flag;

}

unsigned int Graph::get_size()
{
    return vertex_count;
}
