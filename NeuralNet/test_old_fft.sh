#!/bin/bash
#./NeuralNet n 0 1 0 ../hand_images/test/hands_old_fft ../hand_images/test/other_old_fft train_old_fft.weights
echo "n $1 $2" | ./NeuralNet c 0 1 0 ../hand_images/test/hands_old_fft ../hand_images/test/other_old_fft train_old_fft_$1_$2.weights
