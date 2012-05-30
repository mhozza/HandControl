#!/bin/bash
#./NeuralNet n $1 1 0 ../hand_images/hands_fft ../hand_images/other_fft train_new_fft.weights
echo "n $1 $2" | ./NeuralNet c $3 1 0 ../hand_images/hands_fft ../hand_images/other_fft train_new_fft_$1_$2.weights
