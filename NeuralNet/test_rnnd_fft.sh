#!/bin/bash
#echo "r $1 $2" | ./NeuralNet c 1 1 0 ../hand_images/test/hands_r_fft ../hand_images/test/other_r_fft train_rnn_$1_$2.weights
echo "r $1" | ./NeuralNetRd c 1 1 0 ../hand_images/test/hands_r_fft ../hand_images/test/other_r_fft train_rnnd_$1.weights
