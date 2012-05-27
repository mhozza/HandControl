#!/bin/bash
#echo "r $1 $2" | ./NeuralNet c 0 1 0 ../hand_images/hands_r_fft ../hand_images/other_r_fft train_rnn_$1_$2.weights
echo "r $1" | ./NeuralNetRa c 0 1 0 ../hand_images/hands_r_fft ../hand_images/other_r_fft train_rnna_$1.weights

