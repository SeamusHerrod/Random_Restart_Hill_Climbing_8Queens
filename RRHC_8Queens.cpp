#include <iostream>
#include <random>
#include <vector>
#include <ctime>
using namespace std;

static int EVALS = 0;     // running total number of evals
static int GENS = 10;    // number of generations 
static double pmut = 0.1; // mutation prob
static int k = 10;
double myRand(int low, int high) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(low, high);
    double r = dis(gen);
    return r;
}

//double myRand() {
//    return drand48();
//}

// returns 1 if queens q1 and q2 are attacking each other, 0 o.w.
int attacking(int q1col, int q1row, int q2col, int q2row) {
  if (q1col==q2col) return(1); // same column
  if (q1row==q2row) return(1); // same row
  int coldiff=q1col-q2col;
  int rowdiff=q1row-q2row;
  if (abs(coldiff)==abs(rowdiff)) return(1); // same diagonal
  return(0);
}

int fitness(const int config[]) {
    EVALS++;
    int fit = 28;
    for (int i = 0; i < 8; i++) {
        for (int j = i+1; j <= 8; j++) {
            fit -= attacking(i, config[i-1], j, config[j-1]);
        }
    }
    return fit;
}

void random_config(int state[]) {
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(1,8);
    for (int i = 0; i < 8; i++) {
        state[i] = distr(eng);
    }
}
void print_config(int config[]) {
    cout << "[";
    for (int j = 0; j < k; j++) {
        for (int i = 0; i < 8; i++){
            if (i == 7) {
                cout << config[i] << "]" << endl;
            }
            else{
                cout << config[i] << ", ";
            }
    }

    }
}

void mutate(int config[], double p) {
    for (int i = 1; i <= 8; i++) {
        double r = myRand(0,1);
        if (r < p) {
            config[i - 1] = (int) ceil(myRand(1,8)*8);
        }
    }
}
int main() {
    srand(time(0));
    /*----------------------------
            TEST CODE 
    -----------------------------*/ 
    // test myRand
    //cout << "rand between 0 and 1" << endl;
    //for (int i = 0; i < 10; i++) {
    //    cout << myRand(0, 1) << endl;
    //}
    //// test print_config
    //int test_config[8] = {3,2,1,4,3,2,1,2};
    //print_config(test_config);
    //// test fitness()
    //cout << "fitness of test_config: " << fitness(test_config) << endl;
    //// test random_config()
    //random_config(test_config);
    //print_config(test_config);
    //cout << "fitness of test_config: " << fitness(test_config) << endl;
    /*----------------------------
        START FUNCTIONAL CODE
    -----------------------------*/

    // create random starting configuration 
    int cur_config[k][8]; 
    int next_config[k][8];
    int pop_fitness[k];
    int best_fitness = 0;
    int cumprobs[k];
    // initial configuration - random
    for (int i = 0; i < k; i++) {
        random_config(cur_config[i]);
    }

    for ( int i = 0; i < 1; i++){
        for (int i = 0; i < k; i++){
            int pop_fitness = 0;
            print_config(cur_config[i]);
            cout << "========\nGeneration: "<< i << "\n========" << endl;
            pop_fitness = fitness(cur_config[i]);
            cout << "init fitness: " << pop_fitness << endl;
            if (pop_fitness == 28) {
                cout << "Solution Found" << endl;
                print_config(cur_config[i]);
                cout << "Fitness: " << pop_fitness << endl;
                return 0;
            }
        }

        // compute selection probability 
        
        
    }
    return 0;
}