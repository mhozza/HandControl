#!/bin/bash
#echo "r $1 $2" | ./NeuralNet c $3 1 3 ../hand_images/hands_r ../hand_images/other_r train_rnn_$1_$2.weights
echo "r $1" | ./NeuralNet c $2 1 3 ../hand_images/hands_r ../hand_images/other_r train_rnni_$1.weights

