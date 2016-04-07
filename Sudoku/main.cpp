#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;
#define SEED 20160407
#define TMP_MIN 100.00
#define TEMP 0.9999998
#define FIELD 9
#define MAX_ITER 100000

ofstream t_file;
ofstream p_file;


typedef vector <vector<int>> _vec;
typedef vector<int> vec;

vector<double> s_temp;
vector<double> s_prob;
struct field{
    _vec map;
    _vec movable;
    field(){
        map = _vec();
        movable = _vec();
/*        for(int i = 0; i < FIELD; i++ ){
            map[i] = vec(FIELD);
        }*/
    }
    const vec& operator[](const int a) const{
        return this->map[a];
    }
    vec& operator[](const int a){
        return this->map[a];
    }
    int& operator() (const int n, const int m){
        // representation { 0 1 2
        //                  3 4 5
        //                  6 7 8 } as all
        int x = ((n % 3) * 3) + (m % 3);
        int y = ((n / 3) * 3) + (m / 3);
        return this->map[x][y];
    }
    const int& operator() (const int n, const int m) const{
        // representation { 0 1 2
        //                  3 4 5
        //                  6 7 8 } as all
        int x = ((n % 3) * 3) + (m % 3);
        int y = ((n / 3) * 3) + (m / 3);
        return this->map[x][y];
    }



};

struct state{
    long energy;
    field map;
    int progress;
    state(){
        energy = 100;
        map = field();
        progress = 1000;
    }
    void count_energy(){
        int e = 0;
        for(int i = 0; i < FIELD; i++){
            vec r = vec(FIELD);
            vec c = vec(FIELD);
            for(int j = 0; j < FIELD; j++){
                r[map[i][j]-1]++;
                c[map[j][i]-1]++;
            }
            int mr = r[0];
            int mc = c[0];
            for(int k = 1; k < FIELD; k++){
                if(r[k] > mr) mr = r[k];
                if(c[k] > mc) mc = c[k];
            }
            e += (mr + mc);
        }
        energy = e;
    }
    state(string filename){
        ifstream fs;
     //   cout<<filename;
        fs.open(filename.c_str());
     //   cout<<filename;
        map = field();
        vec v;
        int val = 0;
        while(!fs.eof()){
            if (v.size() == FIELD){
                map.map.push_back(v);
                map.movable.push_back(v);
                v = vec();
            }
            fs>>val;
            v.push_back(val);
            if(fs.eof()) break;
        }
        map.map.push_back(v);
        map.movable.push_back(v);
        fs.close();
      //  cerr<<"FILE CLOSED"<<endl;
        this->fill();
        this->count_energy();
        progress = 0;
    }
    state(const state& s){
        int n = 0;
        int m1 = 0;
        int m2 = 0;
        progress = 0;
        map.map = s.map.map;
        map.movable = s.map.movable;
        for(int i = 0; i < 3; i++) {
            n = rand() % FIELD;
            m1 = 0;
            m2 = 0;
            while (m1 == m2) {
                int v;
                do {
                    m2 = rand() % FIELD;
                    v = map.movable[((n % 3) * 3) + (m2 % 3)][((n / 3) * 3) + (m2 / 3)];
                } while (v > 0);
                do {
                    m1 = rand() % FIELD;
                    v = map.movable[((n % 3) * 3) + (m1 % 3)][((n / 3) * 3) + (m1 / 3)];
                } while (v > 0);
            }
            int tmp = map(n, m1);
            map(n, m1) = map(n, m2);
            map(n, m2) = tmp;
        }
        this->count_energy();
    }
    void operator=(const state& s){
        this->map = field();
        this->map.map = s.map.map;
        this->map.movable = s.map.movable;
        this->count_energy();
        this->progress = 10;
    }
    void fill(){
        vector<bool>pattern = vector<bool>();
        for(int i = 0; i < FIELD; i++){
            pattern.push_back(false);
        }
        vector<bool>used;
        for(int n = 0; n < FIELD; n++){
            used = pattern;
            for(int m = 0; m < FIELD; m++){
                if(map(n,m) > 0){
                    used[map(n,m) - 1] = true;
                }
            }
            int i_min = 0;
            for(int m = 0; m < FIELD; m++){
                if(map(n,m) == 0){
                    while(used[i_min] && i_min < FIELD){
                        i_min++;
                    }
                    if(i_min < FIELD){
                        map(n, m) = i_min+1;
                        used[i_min] = true;
                    }
                }
            }
        }
    }
    void print(ostream& os){
        for(int i = 0 ; i< FIELD; i++){
            for(int j = 0; j< FIELD; j++){
                os<<map[i][j]<<" ";
            }
            os<<endl;
        }
    }
};
double step_temp(double t){
    return TEMP*t;
}
double probablity(long e1, long e2, double t){
    //e1 < e2
    if(e2 < e1){
        long tmp = e1;
        e1 = e2;
        e2 = tmp;
    }
    double e = e2 - e1;
    e /= t;
    e *= -1;
    return exp(e);
}

void make_step(state& current_state, double& temperature){
    current_state.progress++;
    state next = current_state;
    for(int i = 0; i < 5; i++){
        state next2 = current_state;
        if(next2.energy < next.energy){
            next = next2;
        }
    }
    double p = probablity(current_state.energy, next.energy, temperature);
  //  p_file<<p<<endl;
    s_prob.push_back(p);
    if(next.energy > current_state.energy){
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
    s_temp.push_back(temperature);
  //  t_file<<temperature<<endl;
}

int main(int argc, char** argv) {
    srand(SEED);
    ofstream e_file;
    e_file.open("energy.csv", fstream::out);
    t_file.open("temperature.csv", fstream::out);
    p_file.open("probability.csv", fstream::out);
    double temperature;
    string filename = argv[1];

    state s(filename);
    long iter = 0;
    state s2 = state(s);
    state smin;
    smin = s;
    cout<<"BEGIN\n";
    ofstream os3;
    os3.open("res.csv", fstream::out);
    s_prob = vector<double>();
    s_temp = vector<double>();
    for(int i = 0; i < 5; i++){
        state s = smin;
        temperature = TMP_MIN;
        while (temperature > 0 && s.progress < 100 && iter < MAX_ITER) {
            make_step(s, temperature);
           // cerr << s.energy << endl;
          //  e_file<<s.energy<<endl;
            if(s.energy < smin.energy){
                smin = s;
                s.print(os3);
                os3<<endl;
            }
            iter++;
            if(iter % 10000 == 0) cout<<iter<<"\n";
        }
        if (s.progress == 100 && s.energy > 18) {
            //Heating, when there is no progress
            iter -= 1000;
            temperature += 200.0;
            s.progress = 0;
            while (temperature > 0 && s.progress < 100 && iter < MAX_ITER) {
                make_step(s, temperature);
        //        e_file << s.energy << endl;
                if (s.energy < smin.energy) {
                    smin = s;
                }
                iter++;
            }
        }
        iter = 0;
        if(smin.energy == 18) break;
    }
    ofstream os;
    os.open("sudoku.csv", fstream::out);
    smin.print(os);
    e_file.close();
    os.close();
    os3.close();
    cout<<smin.energy;
    return 0;
}
