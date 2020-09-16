//
// Created by nijunjie on 2020/8/18.
//

#ifndef RANSAC_SIGMA_TEST_METHODS_H
#define RANSAC_SIGMA_TEST_METHODS_H
#include <Eigen/Eigen>
#include <vector>
#include <random>
#include <time.h>
using namespace Eigen;
using namespace std;

struct matches
{
    Vector3d x1;
    Vector3d x2;
};

class Methods {
public:
    Methods(int data_num,double variance,int outlier_num);
    double evaluate();
    virtual bool forward();

    void generate_data();
    bool compute_result(vector<matches>& the_match);
    int data_num;
    int oulier_num;
    Matrix3d H_gt;
    Matrix3d H_estimated;
    Matrix3d variance_estimated;
    vector<matches> match;
    double variance;
};

#endif //RANSAC_SIGMA_TEST_METHODS_H