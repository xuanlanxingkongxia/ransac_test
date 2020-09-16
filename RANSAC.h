//
// Created by nijunjie on 2020/8/18.
//

#ifndef RANSAC_SIGMA_TEST_RANSAC_H

#include "Methods.h"
#define RANSAC_SIGMA_TEST_RANSAC_H

class RANSAC: public Methods {
public:
    RANSAC(int data_num,double variance,int outlier_num);
    bool forward();

private:
    double examine(Matrix3d H);
    int try_num=10000;
};


#endif //RANSAC_SIGMA_TEST_RANSAC_H
