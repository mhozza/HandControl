#!/bin/bash
#./NeuralNet n $1 1 0 ../hand_images/hands_old_fft ../hand_images/other_old_fft train_old_fft.weights
echo "n $1 $2" | ./NeuralNet c $3 1 0 ../hand_images/hands_old_fft ../hand_images/other_old_fft train_old_fft_$1_$2.weights
