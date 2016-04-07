
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <cmath>

using namespace std;

#define SEED 20160402
#define TMP_MIN 100.00
#define TEMP 0.9999998
#define SIZE 200
#define NEIGHBOUR_N 9

typedef vector<vector<int>> _vec;
typedef vector<int> vec;

struct d2map{
    int s;
    _vec v;
    int size(){
        return this->s;
    }
    d2map(void) = default;
    d2map(int n){
        v = _vec(n);
        s = n;
        for(int i = 0; i < n; i++){
            v[i] = vec(n);
        }
    }
    void read(string filename){
        std::ifstream bs;
        bs.open(filename.c_str());
        int x,y;
        while(!bs.eof()){
            if(bs.eof()) break;
            bs>>x;
            if(bs.eof()) break;
            bs>>y;
            v[x][y] = 1;
        }
        bs.close();
    }
    vec& operator[](const int x){
        return v[x];
    }
    const vec& operator[](const int x) const{
        return v[x];
    }
    void print(ostream& os){
        string res;
        for(int i = 0; i < s; i++){
            for(int j = 0; j < s; j++){
                if(v[i][j] == 1){
                    os<<i<<";"<<j<<endl;
                }
            }
        }
    }
    d2map& operator=(const d2map& map2) = default;
};

struct state{
    long energy;
    d2map s = d2map(0);
    int progress;
    state(){
        energy = INT_MAX;
        s = d2map(0);
        progress = INT_MAX;
    }
    void count_energy(){
        long r,c;
        long mc = 0, mr = 0;
        for(int i = 0; i < s.size(); i++){
            r = 0;
            c = 0;
            for(int j = 0; j < s.size(); j++){
                r += this->s[i][j];
                c += this->s[j][i];
            }
            if( r > mr) mr = r;
            if( c > mc) mc = c;
        }
        energy = (SIZE - mr)*(SIZE - mc);
    }
    state(string filename){
        s = d2map(SIZE);
        s.read(filename);
        this->count_energy();
        progress = 0;
    }
/*    void change(int x, int y, int p1, int p2){
        int x1 = x + p1 % 3 - 1;
        int x2 = x + p2 % 3 - 1;
        int y1 = y + i / 3 - 1;
        int y2 = y + p2 / 3 - 1;
        int tmp = s[x1][y1];
        s[x1][y1] = s[x2][y2];
        s[x2][y2] = tmp;
    }*/
    int count_mass(int x, int y){
        int counter = 0;
        for(int i = 0; i < NEIGHBOUR_N; i ++){
            int x1 = x + i%3 - 1;
            int y1 = y + i/3 - 1;
            counter += s[x1][y1];
        }
        return counter;
    }
    void move_mass(int x_from, int y_from, int x_to, int y_to){
        int next_x = 0;
        int diff_x = 0;
        int next_y = 0;
        int diff_y = 0;
        if( x_from < x_to) {
            next_x = 1;
            diff_x = x_to - x_from;
        }
        if( x_from > x_to) {
            next_x = -1;
            diff_x = x_from - x_to;
        }
        if( y_from < y_to) {
            next_y = 1;
            diff_y = y_to - y_from;
        }
        if( y_from > y_to) {
            next_y = -1;
            diff_y = y_from - y_to;
        }
        for(int i = -1; i <= 1; i++){
            int x1 = x_from + i;
            for(int j = -1; j <= 1; j++){
                int y1 = y_from + i;
                int x2 = x1;
                int y2;
                bool done = false;
                int m = 0;
                while((!done) && (m < diff_x)){
                    m += 1;
                    x2 = x1 + m*next_x;
                    if(s[x2][y1] == 0){
                        s[x2][y1] = 1;
                        done = true;
                    }
                }
                if(done){
                    s[x1][y1] = 0;
                    x1 = x2;
                }
                done = false;
                m = 0;
                while((!done) && (m < diff_y)){
                    m += 1;
                    y2 = y1 + m*next_y;
                    if(s[x1][y2] == 0){
                        s[x1][y2] = 1;
                        done = true;
                    }
                }
                if(done){
                    s[x1][y1] = 0;
                }
            }
        }
    }
    state(const state& prev){
        this->s = prev.s;
        for(int iter = 0; iter < 3; iter++) {
            int x1 = 0;
            int x2 = 0;
            int y1 = 0;
            int y2 = 0;
            x1 = rand() % (s.size() - 2);
            y1 = rand() % (s.size() - 2);
            x1 += 1;
            y1 += 1;
            x2 = rand() % (s.size() - 2);
            y2 = rand() % (s.size() - 2);
            x2++;
            y2++;
            // x' -> i % 3 - 1
            // y' -> i / 3 - 1
/*            int x = x1;
            int y = y1;
            int counter = 0;
            int i = 9;
            long res = 1;*/
            int w1 = count_mass(x1, y1);
            int w2 = count_mass(x2, y2);
            if(w1 < w2){
                move_mass(x1, y1, x2, y2);
            }
            else{
                move_mass(x2,y2, x1, y1);
            }
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
    temperature = step_temp(temperature);
    if(next.energy > current_state.energy){
        double p = probablity(current_state.energy, next.energy, temperature);
        double r = ((double) rand())/ ((double) RAND_MAX);
        if(r <= p) {
            current_state = next;
            temperature *= 0.99999998;
        }
    }
    else{
        current_state = next;
        temperature *= 1.00000003;
    }
}

int main(int argc, char** argv) {
    srand(SEED);
    d2map res;
    double temperature;
    string filename = argv[1];
    state s(filename);
    long iter = 0;
    state s2 = state(s);
    state smin(s);
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
            res = s.s;
        }
    }

    ofstream os;
    os.open("final.csv", fstream::out);
    smin.s.print(os);
    os.close();
    return 0;
}
