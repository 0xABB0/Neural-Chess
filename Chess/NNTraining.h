#pragma once
#include "opennn.h"

class Training {
public:

	Training(OpenNN::NeuralNetwork &nn);
	void performAndSaveTraining();

private:
	OpenNN::DataSet ds;
	OpenNN::LossIndex li;
	OpenNN::TrainingStrategy ts;
};