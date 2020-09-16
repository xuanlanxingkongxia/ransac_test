//
// Created by nijunjie on 2020/8/19.
//

#ifndef RANSAC_SIGMA_TEST_SP_H

#include "Methods.h"
#define RANSAC_SIGMA_TEST_SP_H


class SP : public Methods {
public:
    SP(int data_num,double variance,int outlier_num);
    bool forward();

private:
    double examine(Matrix3d H);
    int try_num=100;
    int sp_num=10;
};


#endif //RANSAC_SIGMA_TEST_SP_H
