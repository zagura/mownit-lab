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
    struct Edge{
        Edge(){
            resistance = 0.0;
            current = 0.0;
            id = 0;
        }
        double resistance;
        double current;
        unsigned long id;
    };
    unsigned long size;
    unsigned long edges;
    double sem;
    unsigned long begin;
    unsigned long end;
    std::vector <unsigned long> visited;
    mz::Matrix M;
    void get_data(std::string filename);
    std::vector <std::vector<Edge>> graph;

    void current_Kirchhoff();
    std::vector<std::vector<int>> current;
    std::vector<std::vector<double>> velocity;

    void velocity_Kirchhoff();

    void is_cycle(unsigned long begin_v, std::vector<unsigned long> path, unsigned long current_vertex);

    void add_cycle(std::vector<unsigned long> path, double val);
};


#endif //MOWNIT_PROBLEM_H
