#!/bin/bash
echo "n $1" | ./NeuralNet c $2 1 0 ../hand_images/hands_fft ../hand_images/other_fft train_nn2u_$1.weights

