#!/bin/bash
echo "n $1 $2"
echo "n $1 $2" | ./NeuralNet c 1 1 0 ../hand_images/test/hands_r_fft ../hand_images/test/other_r_fft train_nn_$1_$2.weights

