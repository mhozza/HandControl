#!/bin/bash
echo "n $1" | ./NeuralNet c 0 1 0 ../hand_images/test/hands_r_fft ../hand_images/test/other_r_fft train_nn2u_$1.weights

