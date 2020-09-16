//
// Created by nijunjie on 2020/8/18.
//

#include <iostream>
#include "RANSAC.h"

RANSAC::RANSAC(int data_num,double variance,int outlier_num)
:Methods(data_num,variance,outlier_num)
{
}

bool RANSAC::forward()
{
    double cost=MAXFLOAT;
    vector<matches> min_matches;
    Matrix3d medium;
    for(int i=0;i<try_num;i++)
    {
        medium=H_estimated;
        min_matches.clear();
        min_matches.push_back(match[rand()%match.size()]);
        min_matches.push_back(match[rand()%match.size()]);
        min_matches.push_back(match[rand()%match.size()]);
        min_matches.push_back(match[rand()%match.size()]);
        compute_result(min_matches);
        double cost_now=examine(H_estimated);
        if(cost_now<cost && !isnan(cost_now))
        {
            cost=cost_now;
            cout<<cost_now<<endl;
        }

        else
            H_estimated=medium;
    }
    return true;
}

double RANSAC::examine(Matrix3d H)
{
    double threshold=8*variance;
    double num=0;
    for(int i=0;i<match.size();i++)
    {
        double x=pow(match[i].x2[0]-(H*match[i].x1)[0],2);
        x+=pow(match[i].x2[1]-(H*match[i].x1)[1],2);
        x+=pow(match[i].x2[2]-(H*match[i].x1)[2],2);
//        cout<<x<<endl;
        if(x>threshold || isnan(x))
            num++;
    }
    return num;
}