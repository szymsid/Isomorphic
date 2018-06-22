#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>
#include <iterator>

namespace graph
{

class Graph
{
    std::vector<std::pair<unsigned int,unsigned int> > edges;
    std::vector<int> permutation;
    unsigned int vertex_count; //done
    //index represents number of neighbors, stored value represents number of vertices with index-number of neighbors
    unsigned int* vertex_by_neighbors_array;
    //number of vertices self-connected in each bucket from above
    unsigned int* vertex_by_neighbors_array_self_connected;
    bool** matrix;
    unsigned int* neighbor_count;
    unsigned int factorial[20];
    bool* self_connected;

public:
    Graph(char* path);
    Graph(const Graph& other);
    ~Graph();
    void check_isomorphism(Graph& other, unsigned int* table);
    unsigned int get_size();
private:
    void permutate(Graph& other,std::vector<int>::iterator v_end,std::vector<int>::iterator beg, std::vector<int>::iterator _end, unsigned int j, unsigned int* table);
    void make_matrix(Graph& other, unsigned int* table);
};

}
#endif //GRAPH_H
