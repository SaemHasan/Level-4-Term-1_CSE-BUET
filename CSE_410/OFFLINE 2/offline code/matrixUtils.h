#include<iostream>
#include<math.h>
#include<vector>
#include<string>


#define MATRIX_SIZE 4

using namespace std;

struct point{
    double x;
    double y;
    double z;
};


struct point getPoint(double x, double y, double z){
    point p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

struct point normalizePoint(struct point p){
    double norm = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    p.x /= norm;
    p.y /= norm;
    p.z /= norm;
    return p;
}



void printColumnMatrix(double *matrix[1]){
    for(int i = 0; i < MATRIX_SIZE; i++){
        cout<<matrix[i][0]<<" ";
    }
    cout<<endl;
}

void printHomogenousMatrix(double *matrix[MATRIX_SIZE]){
    for(int i=0; i<MATRIX_SIZE;i++){
        for(int j=0; j<MATRIX_SIZE; j++){
            cout<<matrix[i][j]<<" ";
        }
        cout<<endl;
    }
}

double** getMatrixForTranslation(double tx, double ty, double tz){
    double** matrix = new double*[MATRIX_SIZE];
    for(int i=0; i<MATRIX_SIZE; i++){
        matrix[i] = new double[MATRIX_SIZE];
    }
    //have to chnage here if matrix size != 4
    for(int i=0;i<MATRIX_SIZE;i++){
        for(int j=0; j<MATRIX_SIZE; j++){
            if(i==j){
                matrix[i][j] = 1;
            }
            else if(i==0 && j==3){
                matrix[i][j] = tx;
            }
            else if(i==1 && j==3){
                matrix[i][j] = ty;
            }
            else if(i==2 && j==3){
                matrix[i][j] = tz;
            }
            else{
                matrix[i][j] = 0;
            }
        }
    }
    return matrix;
}

double** getMatrixForScaling(double sx, double sy, double sz){
    double** matrix = new double*[MATRIX_SIZE];
    for(int i=0; i<MATRIX_SIZE; i++){
        matrix[i] = new double[MATRIX_SIZE];
    }
    //have to chnage here if matrix size != 4
    for(int i=0;i<MATRIX_SIZE;i++){
        for(int j=0; j<MATRIX_SIZE; j++){
            if(i==0 && j==0){
                matrix[i][j] = sx;
            }
            else if(i==1 && j==1){
                matrix[i][j] = sy;
            }
            else if(i==2 && j==2){
                matrix[i][j] = sz;
            }
            else if(i==j){
                matrix[i][j]=1;
            }
            else{
                matrix[i][j] = 0;
            }
        }
    }
    return matrix;
}

double** getIdentityMatrix(){
    double** matrix = new double*[MATRIX_SIZE];
    for(int i=0; i<MATRIX_SIZE; i++){
        matrix[i] = new double[MATRIX_SIZE];
    }
    for(int i=0;i<MATRIX_SIZE;i++){
        for(int j=0; j<MATRIX_SIZE; j++){
            if(i==j){
                matrix[i][j] = 1;
            }
            else{
                matrix[i][j] = 0;
            }
        }
    }
    return matrix;
}