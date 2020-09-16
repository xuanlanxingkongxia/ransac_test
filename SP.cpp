//
// Created by nijunjie on 2020/8/19.
//

#include <iostream>
#include "SP.h"

SP::SP(int data_num, double variance, int outlier_num)
:Methods(data_num,variance,outlier_num)
{
}

bool SP::forward()
{
    double d=sqrt(variance);
    vector<Vector3d> drift;
    Vector3d middle_d(0,0,0);
    Vector3d left_d(-d,0,0);
    Vector3d right_d(d,0,0);
    Vector3d up_d(0, d,0);
    Vector3d down_d(0, -d,0);
    drift.push_back(middle_d);
    drift.push_back(left_d);
    drift.push_back(right_d);
    drift.push_back(up_d);
    drift.push_back(down_d);
    double cost=MAXFLOAT;
    Matrix3d medium;
    for(int i=0;i<match.size();i++)
    {
        for(int j=0;j<5;j++)
        {
            matches it=match[i];
            it.x2+=drift[j];
            for(int k=0;k<sp_num;k++)
            {
                vector<matches> min_matches;
                medium=H_estimated;
                int one=rand()%match.size();
                int two=rand()%match.size();
                int three=rand()%match.size();
                min_matches.push_back(match[one]);
                min_matches.push_back(match[two]);
                min_matches.push_back(match[three]);
                min_matches.push_back(it);
                compute_result(min_matches);
                double cost_now=examine(H_estimated);
                if(cost_now<cost)
                {
                    cost=cost_now;
                    cout<<cost_now<<endl;
                }
                else
                    H_estimated=medium;
            }
        }
    }
    return true;
}

double SP::examine(Matrix3d H)
{
    double threshold=8*variance;
    double num=0;
    for(int i=0;i<match.size();i++)
    {
        double x=pow(match[i].x2[0]-(H*match[i].x1)[0],2);
        x+=pow(match[i].x2[1]-(H*match[i].x1)[1],2);
        x+=pow(match[i].x2[2]-(H*match[i].x1)[2],2);
        if(x>threshold || isnan(x))
            num++;
    }
    return num;
}