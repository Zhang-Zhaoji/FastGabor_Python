#include <cmath>
#include <stdio.h>

int mirror_index(int var, int change, int bound) {
    // just a simple reflect function.
    int current_result = var + change;

    if (current_result < 0) {
        return -current_result - 1; 
    } else if (current_result >= bound) {
        return 2 * bound - 1 - current_result; 
    } else {
        return current_result; 
    }
}
extern "C" {
    void gabor_filter(float* image, int M, int N, float Orientation, float Frequency, int ksize) {
        // image is the point of the true image, 
        // M, N stand for height and width of the origin image,usually N >= M.
        // Orientation is the orientation of the gabor filter, 
        // Frequency determines the frequency of the gabor filter.
        // ksize for the size of gabor kernel.
        
        // define RJ
        float** RJ = (float**)malloc(M * sizeof(float*));
        for (int i = 0; i < M; ++i) {
            RJ[i] = (float*)malloc(N * sizeof(float));
        }
        // define IJ
        float** IJ = (float**)malloc(M * sizeof(float*));
        for (int i = 0; i < M; ++i) {
            IJ[i] = (float*)malloc(N * sizeof(float));
        }
        //printf("pass 2\n");
        
        // define F
        float** F = (float**)malloc(M * sizeof(float*));
        for (int i = 0; i < M; ++i) {
            F[i] = (float*)malloc(N * sizeof(float));
        }
        // define S_sigma
        //std::vector<float> S_sigma(ksize,0);
        float S_sigma[50];
        // define sin and cos array
        float cos_c[50];
        float sin_c[50];
        float cos_s[50];
        float sin_s[50];       
        
        // define sigma = 2 pi ^ 2 /omega
        float sigma = 2 * M_PI * M_PI / Frequency;
        float w_c_theta = Frequency * std::cos(Orientation);
        float w_s_theta = Frequency * std::sin(Orientation);
        // initialize S_sigma function
        for (int index = 0; index < ksize+1; ++index){
            S_sigma[index] = std::exp(-index * index / (2 * sigma * sigma)) ; // gaussian_distance(index,0,sigma);
            cos_c[index] = std::cos(index * w_c_theta);
            sin_c[index] = std::sin(index * w_c_theta);
            cos_s[index] = std::cos(index * w_s_theta);
            sin_s[index] = std::sin(index * w_s_theta);
        }
        //define cos w_c_theta_x, sin w_c_theta_x; cos w_s_theta_y, sin w_s_theta_y;
        float *cos_wc_x = (float*)malloc(N * sizeof(float));
        float *sin_wc_x = (float*)malloc(N * sizeof(float));
        float *cos_ws_y = (float*)malloc(M * sizeof(float));
        float *sin_ws_y = (float*)malloc(M * sizeof(float));
        for (int x = 0; x<N; ++x){
            cos_wc_x[x] = std::cos(w_c_theta *x);
            sin_wc_x[x] = std::sin(w_c_theta *x);
        }
        for (int y = 0; y<M; ++y){
            cos_ws_y[y] = std::cos(w_s_theta *y);
            sin_ws_y[y] = std::sin(w_s_theta *y);
        }

        // horizental filter
        for (int y = 0; y < M; ++y){
            for (int _x = 0; _x < N; ++_x){
                RJ[y][_x] = 0;
                IJ[y][_x] = 0;
                for (int _kernel = -ksize; _kernel<ksize+1;++_kernel){
                    RJ[y][_x] += cos_wc_x[_x] * cos_c[std::abs(_kernel)] * image[y * N + mirror_index(_x,_kernel,N)] * S_sigma[std::abs(_kernel)] \
                    + sin_wc_x[_x] * sin_c[std::abs(_kernel)] * image[y * N + mirror_index(_x,_kernel,N)] * S_sigma[std::abs(_kernel)] * (1?_kernel>0:-1);
                    IJ[y][_x] += - cos_wc_x[_x] * cos_c[std::abs(_kernel)] * image[y * N + mirror_index(_x,_kernel,N)] * S_sigma[std::abs(_kernel)] \
                    + sin_wc_x[_x] * sin_c[std::abs(_kernel)] * image[y * N + mirror_index(_x,_kernel,N)] * S_sigma[std::abs(_kernel)] * (1?_kernel>0:-1);
                }
            }
        }
        
        // verticle filter
        for (int x = 0; x < N; ++x){
            for (int _y = 0; _y < M; ++_y){
                F[_y][x] = 0;
                for (int __kernel = - ksize; __kernel < ksize+1; ++__kernel){
                    F[_y][x] += cos_ws_y[_y] * S_sigma[std::abs(__kernel)] *\
                            (cos_s[std::abs(__kernel)] * RJ[mirror_index(_y,__kernel,M)][x] \
                           + sin_s[std::abs(__kernel)] * IJ[mirror_index(_y,__kernel,M)][x] * (1?__kernel>0:-1)) \
                              + sin_ws_y[_y] * S_sigma[std::abs(__kernel)] *\
                            (sin_s[std::abs(__kernel)] * RJ[mirror_index(_y,__kernel,M)][x] * (1?__kernel>0:-1)\
                           - cos_s[std::abs(__kernel)] * IJ[mirror_index(_y,__kernel,M)][x])  ;
                           
                }
            }
        }
        // write answer
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                // send results towards original matrix
                image[i * N + j] = F[i][j];
            }
        }
       for (int i = 0; i < M; ++i) {
        free(F[i]); 
        free(RJ[i]); 
        free(IJ[i]);
        }
        free(F); 
        free(RJ);
        free(IJ);
        free(cos_wc_x);
        free(sin_wc_x);
        free(cos_ws_y);
        free(sin_ws_y);
    }
}
// g++ -O3 -shared -o gabor_filter.dll gabor_filter.cpp
// 0.0019989013671875

