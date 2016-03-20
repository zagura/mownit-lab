//
// Created by misiek on 20.03.16.
//

#include "mz_axb.h"
#include "problem.h"
#include <iostream>
#include <string>

int main(){
    try {
        mz::Matrix matrix();
        unsigned long s, t;
        double E;
        std::cin >> s >> t >> E; // 0 11 120 graph.txt
        std::string filename;
        std::cin >> filename;
        problem p = problem();
        p.begin = s;
        p.end = t;
        p.sem = E;
        p.full_problem(filename);
    } catch (std::string msg) {
        std::cerr << msg << std::endl;
    }
    return 0;
}