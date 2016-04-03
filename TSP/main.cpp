#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;
#define SEED 20160402
#define TMP_MIN 100.00
#define TEMP 0.9999998


struct point{
    int x, y;
    point(int x, int y){
        this->x = x;
        this->y = y;
    }
    long len(point& p){
        long xs = this->x - p.x;
        long ys = this->y - p.y;
        return xs*xs + ys*ys;
    }
    point(const point& p){
        this->x = p.x;
        this->y = p.y;
    }
    string print(){
        return to_string(x) + "," + to_string(y);
    }
};

struct state{
    long energy;
    vector<point> perm;
    int progress;
    state(){
        energy = 10000;
        perm = vector<point>();
        progress = 1000;
    }
    void count_energy(){
        long e = perm[ perm.size() -1 ].len(perm[0]);
        for(int i = 1; i < perm.size(); i++){
            e += perm[i].len(perm[i-1]);
        }
        energy = e;
    }
    state(string filename){
        ifstream fs;
        fs.open(filename.c_str());
        cout<<filename;
        perm = vector<point>();
        int x,y;
        while(!fs.eof()){
            if(fs.eof()) break;
            fs>>x;
            if(fs.eof()) break;
            fs>>y;
            if(fs.eof()) break;
          //  cout<<x<<" "<<y<<"\t";
            perm.push_back(point(x,y));
        }
        fs.close();
        this->count_energy();
        progress = 0;
    }
    state(const state& s){
        int p1 = 0;
        int p2 = 0;
        while(p1 == p2) {
            p1 = rand() % static_cast<int> (s.perm.size());
            p2 = rand() % static_cast<int> (s.perm.size());
        }
        progress = 0;
        this->perm = s.perm;
        point tmp = perm[p1];
        perm[p1] = perm[p2];
        perm[p2] = tmp;
        this->count_energy();
    }
    state(const state& s, bool m) {
        int p1 = 0;
        int p2 = 0;
        while (p1 == p2) {
            p1 = rand() % static_cast<int> (s.perm.size());
            p2 = rand() % static_cast<int> (s.perm.size());
        }
        progress = 0;
        this->perm = s.perm;
        if (m) {
            point tmp = perm[p1];
            perm[p1] = perm[p2];
            perm[p2] = tmp;
        }
        this->count_energy();
    }
    state& operator=(const state& s) = default;
};
double step_temp(double t){
    return TEMP*t;
}
double probablity(long e1, long e2, double t){
    //e1 < e2
    double e = e2 - e1;
    e /= t;
    e *= -1;
    return exp(e);
}

void make_step(state& current_state, double& temperature){
    current_state.progress++;
    state next = current_state;
    if(next.energy > current_state.energy){
        double p = probablity(current_state.energy, next.energy, temperature);
        double r = ((double) rand())/ ((double) RAND_MAX);
        temperature = step_temp(temperature);
        if(r <= p) {
            current_state = next;
            temperature *= 0.999998;
        }
    }
    else{
        current_state = next;
        temperature = step_temp(temperature)*1.000003;
    }
}

int main(int argc, char** argv) {
    srand(SEED);
    vector<point> res;
    double temperature;
    string filename = argv[1];
    state s(filename);
    long iter = 0;
    state s2 = state(s,true);
    state smin;
    smin = s;
    for(int i = 0; i < 5; i++){
        s = s2;
        temperature = TMP_MIN;
        while (temperature > 1.5 && s.progress < 100 && iter < 5000000) {
            make_step(s, temperature);
            cout << s.energy << endl;
        }
        if (s.progress == 100) {
            //Heating, when there is no progress
            iter -= 10000;
            temperature += 20.0;
            s.progress = 0;
            while (temperature > 1.5 && s.progress < 100 && iter < 5000000) {
                make_step(s, temperature);
                cout << s.energy << endl;
            }
        }
        if(smin.energy > s.energy){
            smin = s;
            res = s.perm;
        }
    }

    ofstream os;
    os.open("last.csv", fstream::out);
    os<<res[0].print()<<"  "<<res[res.size()-1].print()<<endl;
    for(int i = 1; i < res.size(); i++){
        os<<res[i].print()<<"  "<<res[i-1].print()<<endl;
    }
    os.close();
    return 0;
}
