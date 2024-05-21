#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
int main() {
    const int N = 3000; 
    vector<vector<int>> matA(N, vector<int>(N, 1));
    vector<vector<int>> matB(N, vector<int>(N, 2));
    vector<vector<int>> matC(N, vector<int>(N, 0));

    double starttime = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                matC[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    double endtime = omp_get_wtime();
    cout<<"done in : "<<endtime-starttime<<"s"<<endl;
    return 0;
}
