#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>
using namespace std;

static size_t EVALS = 0;     // running total number of evals
static int GENS = 1000;    // number of generations 
static double pmut = 0.15; // mutation prob
static int k = 1000;
static int total_runs = 100;
double myRand(int low, int high) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(low, high);
    double r = dis(gen);
    return r;
}
// returns 1 if queens q1 and q2 are attacking each other, 0 o.w.
int attacking(int q1col, int q1row, int q2col, int q2row) {
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
        for (int j = i+1; j < 8; j++) {
            fit -= attacking(i, config[i], j, config[j]);
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
        for (int i = 0; i < 8; i++){
            if (i == 7) {
                cout << config[i] << "]" << endl;
            }
            else{
                cout << config[i] << ", ";
            }
    }
}
void mutate(int config[], double p) {
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(1,8);
    uniform_real_distribution<> dis(0, 1);
    for (int i = 1; i <= 8; i++) {
        double r = dis(eng);
        if (r < p) {
            config[i - 1] = distr(eng);
        }
    }
}
void crossover (int parent1[], int parent2[], int new_indv1[8], int new_indv2[8]) {

    int partition = (int)ceil(myRand(1,7));
    for(int i = 0; i < partition; i++) {
        new_indv1[i] = parent1[i];
        new_indv2[i] = parent2[i];
    }
    for (int i = partition; i < 8; i++) {
        new_indv1[i] = parent2[i];
        new_indv2[i] = parent1[i];
    }
}
size_t average_runs(vector<size_t> evals, int succ){
    int cum_evals = 0;
    for (int i = 0; i < total_runs; i++){
        cum_evals += evals[i];
    }
    return ceil(cum_evals / succ);
}
int main() {
    srand(time(0));
    int cur_config[k][8]; 
    int next_config[k][8];
    int pop_fitness[k];
    int best_fitness = 0;
    double cumprobs[k];
    vector<size_t> evals_per_run(0);
    int successfulRuns = 0;

    for (int runs = 0; runs < total_runs; runs++){
        EVALS = 0; // reset EVALS counter at beginning of each run 
        bool found_solution = false; // reset found_solution at beginning of each run
        // initial configuration - random
        // create random starting configuration 
        for (int i = 0; i < k; i++) {
            random_config(cur_config[i]);
        }
        cout << "RUN: " << runs << endl;
        for ( int i = 0; i < GENS; i++){
            // here's the restart, now we are only 
            // accepting runs that are completed in sub 5,000 evaluations
            if (EVALS > 5000) {
                break;
            }
            //cout << "========\nGeneration: "<< i << "\n========" << endl;
            int fit = 0;
            int sum_fit = 0;
            for (int i = 0; i < k; i++){
                //print_config(cur_config[i]);
                fit = fitness(cur_config[i]);
                pop_fitness[i] = fit;
                sum_fit += fit;
                if (pop_fitness[i] == 28) {
                    found_solution = true;
                    print_config(cur_config[i]);
                    cout << "evals for run: " << EVALS << endl;
                    evals_per_run.push_back(EVALS);
                    successfulRuns++;
                    break;
                }
            }
            if (found_solution) { // if a solution has been found in this run, go to next run
                break;
            }
            // cumulative probabilities for mutation
            cumprobs[0] = ((double) (pop_fitness[0] / sum_fit));
            for (int i = 1; i < k; i++) {
                cumprobs[i] = cumprobs[i-1] + ((double) pop_fitness[i] / sum_fit);
            }

            // selection 
            for (int i = 0; i < k; i++) {
                int idx;
                double r = myRand(0,1);
                for (int j = 0; j < k; j++) {
                    if (cumprobs[j] >= r) {
                        idx = j;
                        break;
                    }
                }
                for (int j = 0; j < 8; j++) {
                    next_config[i][j] = cur_config[i][j];
                }
            }

            for ( int i = 0; i < k; i+=2){
                crossover(next_config[i], next_config[i+1],next_config[i], next_config[i+1]);
                mutate(next_config[i], pmut);
                mutate(next_config[i+1], pmut);

            }
            for (int i = 0; i < k; i++){
                for (int j = 0; j < 8; j++){
                cur_config[i][j] = next_config[i][j];

                }
            }
        }
    }
    cout << "successful runs: " << successfulRuns << endl;
    cout << "average evals: " << average_runs(evals_per_run, successfulRuns) << endl;;
    return 0;
}