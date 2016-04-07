#include <iostream>
#include <vector>
#include <fstream>
//#include <cstdlib>
#include <cmath>
using namespace std;
#define SEED 20160402
#define TMP_MIN 100.00
#define TEMP 0.999998
#define MAX_ITER 1000000
#define STATES 6
#define SWAP 5 //arbitrary

ofstream energy_file;
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
    long long energy;
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
            this->perm = s.perm;
            progress = 0;
            int count = 8;
            while (count--) {
                int p1 = 0;
                int p2 = 0;
                while (p1 == p2) {
                    p1 = rand() % static_cast<int> (s.perm.size());
                    if(SWAP == 0) {
                        p2 = rand() % static_cast<int> (s.perm.size());
                    }else if(SWAP > 1) {
                        p2 = (p1 + 1) % (static_cast<int>(s.perm.size()));
                    }
                }
                point tmp = perm[p1];
                perm[p1] = perm[p2];
                perm[p2] = tmp;
            }
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
    if(e2 < e1){
        int tmp = e1;
        e1 = e2;
        e2 = tmp;
    }
    double e = e2 - e1;
    e /= (t);
    e *= -1;
    return exp(e);
}

void make_step(state& current_state, double& temperature){
    current_state.progress++;
    state next = current_state;
    for(int i = 0; i < STATES; i++){
        state next1 = current_state;
        if(next.energy > next1.energy){
            next = next1;
        }
    }
    if(next.energy > current_state.energy){
        double p = probablity(current_state.energy, next.energy, temperature);
        double r = ((double) rand())/ ((double) RAND_MAX);
        temperature = step_temp(temperature);
        if(r <= p) {
            current_state = next;
            temperature *= 0.99999998;
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
    state s2(filename);
    long iter = 0;
    res = s2.perm;
    ofstream os2;
    energy_file.open("energy.csv", fstream::out);
    os2.open("first.csv", fstream::out);
    os2<<res[0].print()<<";"<<res[res.size()-1].print()<<endl;
    for(int i = 1; i < res.size(); i++){
        os2<<res[i].print()<<";"<<res[i-1].print()<<endl;
    }
    os2.close();
    state smin;
    smin = s2;
   // long timing = 0;
    long global_iter = 0;
    for(int i = 0; i < 5; i++){
        state s = smin;
        temperature = TMP_MIN;
        while (temperature > 0 && s.progress < 100 && iter < MAX_ITER) {
            make_step(s, temperature);
            if(s.energy < smin.energy) smin = s;
            iter++;
            global_iter++;
            energy_file<<global_iter<<","<<s.energy<<"\n";
        }
        if (s.progress == 100) {
            //Heating, when there is no progress
            iter -= 1000;
            temperature += 200.0;
            s.progress = 0;
            while (temperature > 0 && s.progress < 100 && iter < MAX_ITER) {
                make_step(s, temperature);
                if(s.energy < smin.energy) smin = s;
                iter++;
                global_iter++;
                energy_file<<global_iter<<","<<s.energy<<"\n";
            }
        }
        iter = 0;
    }
    res = smin.perm;
    ofstream os;
    os.open("last.csv", fstream::out);
    os<<res[res.size()-1].print()<<";"<<res[0].print()<<endl;
    for(int i = 1; i < res.size(); i++){
        os<<res[i-1].print()<<";"<<res[i].print()<<endl;
    }
    os<<res[res.size()-1].print()<<";"<<res[0].print()<<endl;
    os.close();
    energy_file.close();
    return 0;
}
