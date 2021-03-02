#include <vector>
#include "NN.h"
#include <ctime>
#include <iostream>
using namespace std;
float NN::Activation(float x) {
	return x < 0 ? x * (float)0.01 : x; // Leaky ReLU
}
float NN::Derivative(float x) {
	return x < 0 ? (float)0.01 : 1; // LeakyReLU's derivative
}
float NN::Error(float x, float y) {
	return (x - y) * (x - y); // MSE
}
NN::NN(vector<int> structure) {
	srand((unsigned int)time(NULL));
	for (auto l : structure) { // filling structure with neurons
		this->structure.push_back(vector<Neuron>(l, Neuron()));
		size_t size = this->structure.size(); // count of filled layers
		if (size == 1) continue; // if firts layer continue
		for (auto& n : this->structure[size - 1]) { // else for each neuron
			n.weights = vector<float>(this->structure[size - 2].size());
			for (auto& i : n.weights) i = (float)rand() / RAND_MAX; // set random weights
		}
	}
}
vector<float> NN::FeedForward(vector<float> input) {
	if (input.size() != this->structure[0].size())
		throw invalid_argument("Invalid size of input");
	for (size_t l = 0; l < this->structure.size(); ++l) { // each layer
		if (l == 0) { // first layer
			for (size_t i = 0; i < input.size(); ++i) { // each neuron
				this->structure[0][i].input = input[i]; // set inputs
				this->structure[0][i].output = this->Activation(input[i]);
			}
		}
		else { // from second to last layers
			for (size_t n = 0; n < this->structure[l].size(); ++n) {
				float sum = 0;
				for (size_t i = 0; i < this->structure[l - 1].size(); ++i)
					sum += this->structure[l - 1][i].output * this->structure[l][n].weights[i];
				this->structure[l][n].input = sum;
				this->structure[l][n].output = this->Activation(sum);
			}
		}
	}
	vector<float> output(this->structure[this->structure.size() - 1].size());
	for (size_t i = 0; i < output.size(); ++i)
		output[i] = this->structure[this->structure.size() - 1][i].output;
	return output;
}
void NN::Train(vector<float> input, vector<float> right_output) {
	vector<float> output = this->FeedForward(input);
	for (size_t i = 0; i < this->structure[this->structure.size() - 1].size(); ++i) {
		Neuron n = this->structure[this->structure.size() - 1][i];
		n.error = this->Error(n.output, right_output[i]);
	}
	for (int l = (int)this->structure.size() - 2; l >= 0; --l) { // each layer from last to second
		for (size_t n = 0; n < this->structure[l].size(); ++n) { // each neuron
			float error = 0;
			for (auto i : this->structure[l + 1])
				error += i.error * i.weights[n];
			this->structure[l][n].error = error;
		}
	}
	for (size_t l = 1; l < this->structure.size(); ++l) { // each layer
		for (size_t n = 0; n < this->structure[l].size(); ++n) { // each neuron of current layer
			for (size_t i = 0; i < this->structure[l - 1].size(); ++i) { // each neuron of last layer

			}
		}
	}
}
float NN::getMSE(vector<float> input, vector<float> right_output) {
	vector<float> output = this->FeedForward(input);
	vector<float> error(output.size());
	float sum = 0;
	for (size_t i = 0; i < output.size(); ++i) {
		error[i] = (right_output[i] - output[i]) * (right_output[i] - output[i]);
		sum += error[i];
	}
	return sum / error.size();
}
void NN::PrintStructure() {
	for (auto l : this->structure) {
		cout << l.size() << " ";
	}
}