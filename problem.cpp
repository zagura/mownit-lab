//
// Created by misiek on 19.03.16.
//

#include "problem.h"
#include <algorithm>
#include <string>
#include <iostream>

void problem::get_data(std::string filename) {
    std::ifstream fs;
    try {
        fs.open(filename.c_str());
    }catch(...){
        std::cerr<<"Didn't open the file "<<filename<<std::endl;
    }
    unsigned long n = 0;
    unsigned long edges = 0;
    fs>>n>>edges;

    this->size = n;
    this->edges = edges;
    this->graph = std::vector <std::vector<Edge> >(n);
    for(unsigned long i = 0; i < n; i++){
    	graph.at(i) = std::vector<Edge>(n);
    }
    graph_edges = std::vector<std::pair<unsigned long, unsigned long>>(edges);
    for(unsigned long i = 0; i < edges; i++){
    	unsigned long r = 0, c = 0;
        double val = 0.0;
    	fs>>r>>c>>val;
    	r < c ? graph[r][c].resistance = val : graph[c][r].resistance = val;
        r < c ? graph[r][c].id = i : graph[c][r].id = i;
        graph_edges[i] = std::make_pair(r, c);
    }
    fs.close();
}


void problem::current_Kirchhoff(){
    /*
     *  tworzę układ równań dla prądowego prawa Kirchhoffa
     *  Każdemu węzłowi przyporządkowuje prądy wpływające i wypływające
     *
     */
    this->current = std::vector < std::vector <double> >(this->size);
    for(unsigned long i = 0; i< this->size; i++){
        this->current[i] = std::vector <double>(this->edges);
    }
    for(unsigned long i = 0; i< this->size; i++){
        for(unsigned long j = i+1; j < this->size; j++){
            if(this->graph[i][j].resistance > 0){
                this->current[i][graph[i][j].id] = 1;
                this->current[j][graph[i][j].id] = -1;
            }
        }
    }
    for(unsigned long i = 0; i < edges; i++){
        double val = current[begin][i] + current[end][i];
        current[begin][i] = val;
        current[end][i] = val;
    }
}

void problem::velocity_Kirchhoff(){
    this->velocity = std::vector <std::vector <double> > ();
    this->visited = std::vector<unsigned long> (this->size);
    for(unsigned long i = 0 ; i < this->size; i++){
        is_cycle(i, std::vector<unsigned long>(), i, i);
        visited.push_back(i);
    }
}
void problem::add_cycle(std::vector <unsigned long> path, double val){
    if(path.size() < 3) return;
/*    std::cout<<"CYCLE:\n";
    for(unsigned long i = 0; i < path.size(); i++){
        std::cout<<path[i]<<"\t";
    }*/
    std::vector<double> equation = std::vector<double>(this->edges);
    for(unsigned i = 0; i < path.size();i++){
        unsigned long a = path[i%path.size()];
        unsigned long b = path[(i+1)%path.size()];
        unsigned long lower = a < b ? a : b;
        unsigned long greater = a < b ? b : a;
        if(!((lower == begin && greater == end) || (lower == end) && (greater == begin))) {
            unsigned long edge_id = graph[lower][greater].id;
            double res = graph[lower][greater].resistance;
            if (lower != a) {
                res = -res;
            }
            equation[edge_id] = res;
        }
    }
    if(std::find(velocity.begin(), velocity.end(), equation) == velocity.end()){
        velocity.push_back(equation);
        velocity_b.push_back(val);
/*        std::cout<<"EQUATION:\n";
        for(unsigned long i = 0; i < equation.size(); i++){
            std::cout<<equation[i]<<"\t";
        }
        std::cout<<std::endl<<std::endl;*/
    }
}


void problem::is_cycle(unsigned long begin_v, std::vector<unsigned long> path, unsigned long current_vertex,
                       unsigned long last){
    path.push_back(current_vertex);
    if((this->graph[begin_v][current_vertex].resistance > 0
        || this->graph[current_vertex][begin_v].resistance > 0)
            && (last != begin_v)){
        this->add_cycle(path, 0);
    }
    if((begin_v == this->begin && current_vertex == this->end) ||
       (begin_v == this->end && current_vertex == this->begin)){
        this->add_cycle(path, this->sem);
    }
    if(current_vertex != begin_v) {
        for(unsigned long next = 0; next < current_vertex; next++){
            if(graph[next][current_vertex].resistance > 0 && next != last) {
                if (std::find(path.begin(), path.end(), next) == path.end() &&
                    std::find(visited.begin(), visited.end(), next) == visited.end()) {
                    this->is_cycle(begin_v, path, next, current_vertex);
                }
            }
        }
    }
    for (unsigned long next = current_vertex + 1; next < this->size; next++) {
        if (graph[current_vertex][next].resistance > 0 && next != last) {
            if (std::find(path.begin(), path.end(), next) == path.end() &&
                std::find(visited.begin(), visited.end(), next) == visited.end()) {
                this->is_cycle(begin_v, path, next, current_vertex);
            }
        }
    }
}

void problem::solve(void){
    unsigned long rows = this->size + this->velocity.size();
    unsigned long columns = this->edges;
    this->M = mz::Matrix(rows, columns);
    this->b = std::vector<double>(rows);
    for(unsigned long r = 0 ; r < this->size;r++){
        M.v[r] = current[r];
        b[r] = 0.0;
    }
    for(unsigned long r = this->size; r < rows; r++){
        M.v[r] = velocity[r - this->size];
        b[r] = velocity_b[r - this->size];
    }
  //  M.print();
    solution = std::vector<double>(rows);
    std::vector<double*> x = std::vector<double*>(rows);
    for(unsigned long i = 0; i < rows; i++) {
        x[i] = &(solution[i]);
    }
  //  M.print();
/*    for(int i = 0 ; i < b.size(); i++){
        std::cout<<b[i]<<"  ";
    }
    std::cout<<std::endl;*/
    mz::solve_normalized_LU(M, x, b);
}

void problem::print_result(void){
    std::ofstream ofs;
    try{
    ofs.open("results.txt", std::ios_base::out);
    }catch(...){
        std::cerr<<"Can't open results.txt file";
    }
    ofs<<this->size<<" "<<this->edges<<std::endl;
    for(unsigned long i = 0; i < this->edges; i++){
        ofs << graph_edges[i].first << " " << graph_edges[i].second << " " << std::abs(solution[i]) << std::endl;
    }
    ofs.close();
}
void problem::full_problem(std::string filename){
    this->get_data(filename);
   // std::cout<<"GET DATA FINISHED"<<std::endl;
    this->current_Kirchhoff();
  // std::cout<<"CURRENT KIRCHHOFF FINISHED"<<std::endl;
    this->velocity_Kirchhoff();
  //  std::cout<<"VELOCITY KIRCHHOFF FINISHED"<<std::endl;
    this->solve();
   // std::cout<<"SOLUTION FINISHED"<<std::endl;
    this->print_result();
    //std::cout<<"PRINTING FINISHED"<<std::endl;
}