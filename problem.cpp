//
// Created by misiek on 19.03.16.
//

#include "problem.h"
#include <algorithm>

void problem::get_data(std::string filename) {
    std::ifstream fs;
    fs.open(filename.c_str());
    unsigned long n;
    unsigned long edges;
    fs>>n>>edges;
    this->size = n;
    this->edges = edges;
    std::vector <std::vector<Edge> > graph(n);
    for(unsigned long i = 0; i < n; i++){
    	graph.at(i) = std::vector<Edge>(n);
    }
    for(unsigned long i = 0; i < edges; i++){
    	unsigned long r, c;
    	double val;
    	fs>>r>>c>>val;
    	r < c ? graph[r][c].resistance = val : graph[c][r].resistance = val;
        r < c ? graph[r][c].id = i : graph[c][r].id = i;

    }
    fs.close();
}


void problem::current_Kirchhoff(){
    this->current = std::vector < std::vector <int> >(this->size);
    for(unsigned long i = 0; i< this->size; i++){
        this->current[i] = std::vector <int>(this->edges);
    }
    for(unsigned long i = 0; i< this->size; i++){
        for(unsigned long j = i+1; j < this->size; j++){
            if(this->graph[i][j].resistance > 0){
                this->current[i][graph[i][j].id] = 1;
                this->current[j][graph[i][j].id] = -1;
            }
        }
    }
}

void problem::velocity_Kirchhoff(){
    this->velocity = std::vector <std::vector <double> > (this->size);
    this->visited = std::vector<unsigned long> (this->size);
    for(unsigned long i = 0 ; i < this->size; i++){
        this->visited.push_back(i);
        unsigned long d = 0;
    }
}
void problem::add_cycle(std::vector <unsigned long> path, double val){
    std::vector<double> equation;
    for(unsigned i = 0; i < path.size();i++){
       // vector
    }
}


void problem::is_cycle(unsigned long begin_v, std::vector<unsigned long> path, unsigned long current_vertex){
    path.push_back(current_vertex);
    if(this->graph[begin_v][current_vertex].resistance > 0 || this->graph[current_vertex][begin_v].resistance > 0){
        this->add_cycle(path, 0);
    }
    if((begin_v == this->begin && current_vertex == this->end) ||
       (begin_v == this->end && current_vertex == this->begin)){
        this->add_cycle(path, this->sem);
    }
    for(unsigned long next = 0; next < current_vertex; next++){
        if(std::find(path.begin(), path.end(), next) == path.end() &&
                std::find(this->visited.begin(), this->visited.end(), next) == this->visited.end()){
            this->is_cycle(begin_v, path, next);
        }
    }
}


