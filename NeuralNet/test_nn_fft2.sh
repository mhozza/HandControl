#!/bin/bash
echo "n $1" | ./NeuralNet2 c 1 1 0 ../hand_images/test/hands_r_fft ../hand_images/test/other_r_fft train_nn_$1.weights

