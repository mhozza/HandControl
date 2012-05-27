#!/bin/bash
echo "n $1" | ./NeuralNet2u c 1 1 0 ../hand_images/hands_r_fft ../hand_images/other_r_fft train_nn2u_$1.weights

