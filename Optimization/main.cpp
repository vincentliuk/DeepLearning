#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <armadillo>
#include "optimization.h"
#include <vector>

void loadData_MNIST(std::shared_ptr<arma::mat> X,
                    std::shared_ptr<arma::mat> Y);

using namespace Optimization;

struct MyFunc: public ObjectFunc{

    MyFunc(int dim):ObjectFunc(dim){
        x_init = std::make_shared<arma::vec>(dim,arma::fill::randn);
    }
    
    virtual double operator()(arma::vec &x, arma::vec &grad){
        
        grad = 2.0* x;
        return arma::as_scalar(x.st() * x);
    }
    
};

struct MyFunc2: public ObjectFunc{

    arma::mat W;
    arma::vec b;
    MyFunc2(int dim):ObjectFunc(dim){
        W.eye(dim,dim);
        b = arma::randu(dim);
        
        x_init = std::make_shared<arma::vec>(dim,arma::fill::randn);
    }
    
    virtual double operator()(arma::vec &x, arma::vec &grad){
        
        
        grad = W * x + b;
        return arma::as_scalar(0.5 * x.st() * W * x + b.st()*x);
    }
    
};

struct MyFunc3: public ObjectFunc{

    MyFunc3(int dim = 2):ObjectFunc(dim){
        
        x_init = std::make_shared<arma::vec>(dim,arma::fill::randn);
    }
    
    virtual double operator()(arma::vec &x, arma::vec &grad){
        
        double F = 10.0 * (x[1] - x[0] * x[0]) * (x[1] - x[0] * x[0]) + (x[0] - 1) * (x[0] - 1);
        
        grad.resize(2);
        grad[0] = -40.0 * x[0] * x[1] + 40.0 * pow(x[0],3.0) + 2 * x[0] - 2;
        grad[1] = 20 * x[1] - 20.0 * x[0] * x[0];
        return F;
    }
    
};

struct MyFunc4: public ObjectFunc{

    MyFunc4(int dim = 5000):ObjectFunc(dim){
         /* Initialize the variables. */
        x_init = std::make_shared<arma::vec>(dim,arma::fill::randn); 
    for (int i = 0;i < dim;i += 2) {
        (*x_init)[i] = -1.2;
        (*x_init)[i+1] = 1.0;
    }

    }
    
    virtual double operator()(arma::vec &x, arma::vec &grad){
        double fx = 0;
        grad.resize(dim);
    for (int i = 0;i < dim; i += 2) {
        double t1 = 1.0 - x[i];
        double t2 = 10.0 * (x[i+1] - x[i] * x[i]);
        grad[i+1] = 20.0 * t2;
        grad[i] = -2.0 * (x[i] * grad[i+1] + t1);
        fx += t1 * t1 + t2 * t2;
    }
        
    return fx;
    }
    
};


int main(int argc, char *argv[]) {
    arma::wall_clock timer;
    int dim = 5;
    
    MyFunc myFunc(dim);
    
    arma::vec x(dim);
    arma::vec grad;
//    x.randu();
    
//    x.print();
//    std::cout << myFunc(x,grad) << std::endl;
//    grad.print();
    LBFGS::LBFGS_param param(200,20);

//    std::cout << "test case 1" << std::endl;
//    LBFGS lbfgs_opt(myFunc,param);
//    lbfgs_opt.minimize();
 /*    
    std::cout << "test case 2" << std::endl;
    MyFunc2 myFunc2(dim);
    LBFGS  *lbfgs_opt2=new LBFGS(myFunc2,param,LBFGS::Armijo);
    lbfgs_opt2->minimize();
    delete lbfgs_opt2;
    lbfgs_opt2 = new LBFGS(myFunc2,param,LBFGS::Wolfe);
    lbfgs_opt2->minimize();
    delete lbfgs_opt2;
   
    std::cout << "test case 3" << std::endl;
    MyFunc3 myFunc3;
    lbfgs_opt2=new LBFGS(myFunc3,param,LBFGS::Armijo);
    lbfgs_opt2->minimize();
    delete lbfgs_opt2;
    lbfgs_opt2 = new LBFGS(myFunc3,param,LBFGS::Wolfe);
    lbfgs_opt2->minimize();
    delete lbfgs_opt2;
    
    
    timer.tic();
    std::cout << "test case 4" << std::endl;
//    MyFunc3 myFunc3;
    SteepDescent::SteepDescent_param param0(1e-6, 0.01, 2000);
    SteepDescent sd_opt4(myFunc3,param0);
    sd_opt4.minimize();

    
    
    timer.tic();
    
   */
  std::cout << "test case 5" << std::endl;
    MyFunc4 myFunc4;
    LBFGS  *lbfgs_opt5=new LBFGS(myFunc4,param,LBFGS::Armijo);
    lbfgs_opt5->minimize();
    delete lbfgs_opt5;
    lbfgs_opt5 = new LBFGS(myFunc4,param,LBFGS::Wolfe);
    lbfgs_opt5->minimize();
    delete lbfgs_opt5;
    
    std::cout << "time cost is" << timer.toc() << std::endl;
    return 0;
}


void loadData_MNIST(std::shared_ptr<arma::mat> X,
                    std::shared_ptr<arma::mat> Y) {

    std::string filename_base("../MNIST/data");
    std::string filename;
    char tag[50];
    char x;
    int count;
    int numFiles = 10;
    int featSize = 28*28;
    int labelSize = 10;
    int numSamples = 1000;
    X->set_size(numFiles*numSamples,featSize);
    Y->set_size(numFiles*numSamples,labelSize);
    Y->fill(0);
//  std::cout << Y.Len() << std::endl;
//  std::cout << X.NumR() << std::endl;
//  std::cout << X.NumC() << std::endl;

    for (int i = 0 ; i < numFiles ; i++) {
        sprintf(tag,"%d",i);
        filename=filename_base+(std::string)tag;
        std::cout << filename << std::endl;
        std::ifstream infile;
        infile.open(filename,std::ios::binary | std::ios::in);
        if (infile.is_open()) {

            for (int j = 0 ; j < numSamples ; j++) {

                for (int k =0 ; k <featSize; k ++) {
                    infile.read(&x,1);
//        std::cout << x << std::endl;
                    (*X)(j+i*numSamples,k)=(unsigned char)x;
                }
                (*Y)(j+i*numSamples,i) = 1;
            }

        } else {
            std::cout << "open file failure!" << std::endl;
        }

// for (int j = 0 ; j < numSamples ; j++){
//       for (int k =0 ; k <featSize; k ++){

//	           std::cout << x << std::endl;
//	   std::cout<<  (*X)(j,k) << " ";
//	   }
//	   }

        std::cout << "dataloading finish!" <<std::endl;
    }
}
