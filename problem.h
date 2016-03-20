//
// Created by misiek on 19.03.16.
//

#ifndef MOWNIT_PROBLEM_H
#define MOWNIT_PROBLEM_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "mz_axb.h"

class problem {
public:
    struct Edge{
        Edge(){
            resistance = 0.0;
            id = 0;
        }
        double resistance;
        unsigned long id;
    };
    unsigned long size;
    unsigned long edges;
    double sem;
    unsigned long begin;
    unsigned long end;
    std::vector <unsigned long> visited;
//    std::vector <std::vector<double>> equations;
    std::vector <double> b;
    mz::Matrix M;
    void get_data(std::string filename);
    std::vector <std::vector<Edge>> graph;
    std::vector <std::pair<unsigned long, unsigned long>> graph_edges;
    std::vector<std::vector<double>> current;
    std::vector<std::vector<double>> velocity;
    std::vector<double> velocity_b;
    std::vector<double> solution;

    void current_Kirchhoff();

    void velocity_Kirchhoff();

    void is_cycle(unsigned long begin_v, std::vector<unsigned long> path,
                  unsigned long current_vertex, unsigned long last);

    void add_cycle(std::vector<unsigned long> path, double val);

    void solve(void);

    void print_result();

    void full_problem(std::string filename);
};


#endif //MOWNIT_PROBLEM_H
