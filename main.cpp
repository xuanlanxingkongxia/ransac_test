#include <iostream>
#include "RANSAC.h"
#include "SP.h"


int main() {
    Methods* m=new RANSAC(1000,5,100);
//    Methods* m =new SP(1000,5,100);
    cout<<m->H_gt<<endl;
    m->forward();
    cout<<m->H_estimated<<endl;
    cout<<m->evaluate()<<endl;
    return 0;
}