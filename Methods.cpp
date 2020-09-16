//
// Created by nijunjie on 2020/8/18.
//

#include <chrono>
#include <random>
#include <iostream>
#include "Methods.h"

Methods::Methods(int data_num_, double variance_, int outlier_num_)
{
    data_num=data_num_;
    variance=variance_;
    oulier_num=outlier_num_;
    generate_data();
}

void Methods::generate_data()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    srand(seed);
    normal_distribution<double> distribution(0.0, variance);
    for(int i=0;i<data_num;i++)
    {
        matches t;
        t.x1[0]=(float)(rand()%10000)/10000 * 100;
        t.x1[1]=(float)(rand()%10000)/10000 * 100;
        t.x1[2]=1;
        match.push_back(t);
    }
    double sum=0;
    for(int i=0;i<8;i++)
    {
        float a=(float)(rand()%10000)/10000 - 0.5;
        H_gt(i/3,i%3) = a;
        sum+= abs(a);
    }
    sum+=1;
    H_gt(2,2)=1;
    for(int i=0;i<9;i++)
    {
        H_gt(i/3,i%3) /= sum;
    }
    for(int i=0;i<data_num;i++) {
        Vector3d add_num(distribution(generator),distribution(generator),0);
        match[i].x2=H_gt*match[i].x1 + add_num;
        if(i<oulier_num)
        {
            Vector3d add_num2((float)(rand()%10000)/10000  * 10-5,(float)(rand()%10000)/10000  * 10-5,0);
            match[i].x2[0]/=match[i].x2[2];
            match[i].x2[1]/=match[i].x2[2];
            match[i].x2+=add_num2;
        }
    }
    shuffle(match.begin(),match.end(), std::mt19937(std::random_device()()));
}

double Methods::evaluate()
{
    double sum0=0;
    double sum1=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            sum0+=abs(H_gt(i,j));
            sum1+=abs(H_estimated(i,j));
        }
    }
    double sum=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            sum+=powf(H_gt(i,j)/sum0-H_estimated(i,j)/sum1,2);
        }
    }
    return sum;
}

bool Methods::compute_result(vector<matches>& the_match)
{
    if(the_match.size()<4)
        return false;
    VectorXd h = VectorXd::Zero(9);
    if(the_match.size()==4)
    {
        MatrixXd A=MatrixXd::Zero(9,9);
        VectorXd b=VectorXd::Zero(9);
        for(int i=0;i<4;i++)
        {
            A(i*2,3)=-the_match[i].x1[0]*the_match[i].x2[2];
            A(i*2,4)=-the_match[i].x1[1]*the_match[i].x2[2];
            A(i*2,5)=-the_match[i].x1[2]*the_match[i].x2[2];
            A(i*2,6)=the_match[i].x1[0]*the_match[i].x2[1];
            A(i*2,7)=the_match[i].x1[1]*the_match[i].x2[1];
            A(i*2,8)=the_match[i].x1[2]*the_match[i].x2[1];

            A(i*2+1,0)=the_match[i].x1[0]*the_match[i].x2[2];
            A(i*2+1,1)=the_match[i].x1[1]*the_match[i].x2[2];
            A(i*2+1,2)=the_match[i].x1[2]*the_match[i].x2[2];
            A(i*2+1,6)=-the_match[i].x1[0]*the_match[i].x2[0];
            A(i*2+1,7)=-the_match[i].x1[1]*the_match[i].x2[0];
            A(i*2+1,8)=-the_match[i].x1[2]*the_match[i].x2[0];

        }
        for(int i=0;i<9;i++)
        {
            A(8,i)=1;
        }
        b(8)=1;
        h=A.inverse()*b;
    } else {
        MatrixXd A = MatrixXd::Zero(the_match.size() * 2 + 1, 9);
        VectorXd b = VectorXd::Zero(9);

        for(int i=0;i<the_match.size();i++) {

            A(i * 2, 3) = -the_match[i].x1[0] * the_match[i].x2[2];
            A(i * 2, 4) = -the_match[i].x1[1] * the_match[i].x2[2];
            A(i * 2, 5) = -the_match[i].x1[2] * the_match[i].x2[2];
            A(i * 2, 6) = -the_match[i].x1[0] * the_match[i].x2[1];
            A(i * 2, 7) = -the_match[i].x1[1] * the_match[i].x2[1];
            A(i * 2, 8) = -the_match[i].x1[2] * the_match[i].x2[1];

            A(i * 2 + 1, 0) = the_match[i].x1[0] * the_match[i].x2[2];
            A(i * 2 + 1, 1) = the_match[i].x1[1] * the_match[i].x2[2];
            A(i * 2 + 1, 2) = the_match[i].x1[2] * the_match[i].x2[2];
            A(i * 2 + 1, 6) = -the_match[i].x1[0] * the_match[i].x2[1];
            A(i * 2 + 1, 7) = -the_match[i].x1[1] * the_match[i].x2[1];
            A(i * 2 + 1, 8) = -the_match[i].x1[2] * the_match[i].x2[1];
        }
        for(int i=0;i<9;i++)
        {
            A(the_match.size()*2,i)=1;
        }
        b(the_match.size())=1;
        h=(A.transpose()*A).inverse()*A.transpose()*b;
    }
    double sum=0;
    for(int i = 0; i < 3 ; i++)
    {
        for(int j = 0;j < 3; j++)
        {
            sum+=abs(h(i*3+j));
        }
    }
    for(int i = 0; i < 3 ; i++)
    {
        for(int j = 0;j < 3; j++)
        {
            H_estimated(i,j)=h(i*3+j)/sum;
        }
    }
//    cout<<H_estimated.determinant()<<endl;
    return true;
}

bool Methods::forward()
{
    return true;
}