#pragma once
#include <memory>
#include <armadillo>
#include <vector>
#include "../BaseLayer/BaseLayer.h"
#include "../ElementwiseLayer/ElementwiseLayer.h"
#include "../LinearAdditionLayer/LinearAdditionLayer.h"
#include "../ActivationLayer/ActivationLayer.h"
#include "BaseLayer_LSTM.h"

namespace NeuralNet {

    class RNN_LSTM {
        
    public:
        RNN_LSTM(int numHiddenLayers0, int hiddenLayerInputDim0,
        int hiddenLayerOutputDim0, int inputDim0, int outputDim0, 
        std::shared_ptr<arma::mat> trainingX0, std::shared_ptr<arma::mat> trainingY0);
        void forward();
        void backward();
        void train();
        void savePara(std::string filename); // try to save all the parameters in the LSTM for further use
        void test();
    private:
        std::vector<BaseLayer_LSTM> inGateLayers, forgetGateLayers, outputGateLayers, informationLayers, cellStateLayers;
        std::vector<ElementwiseLayer> outputElementLayers, forgetElementGateLayers, inputElementGateLayers;
        std::vector<LinearAdditionLayer> cellLinearAdditionLayers;
        std::vector<ActivationLayer> cellStateActivationLayers;
//        std::vector<BaseLayer> layerOutput_prev, cellState_prev;
        BaseLayer_LSTM* netOutputLayer;
        std::shared_ptr<arma::mat> trainingY, trainingX;
        int numHiddenLayers, hiddenLayerInputDim, hiddenLayerOutputDim;
        int rnnInputDim, rnnOutputDim;
        
        

    };

}


