#!/bin/bash
#./NeuralNet 1 1 0 ../hand_images/test/hands_fft ../hand_images/test/other_fft train_new_fft.weights
echo "n $1 $2" | ./NeuralNet c 0 1 0 ../hand_images/test/hands_fft ../hand_images/test/other_fft train_new_fft_$1_$2.weights
