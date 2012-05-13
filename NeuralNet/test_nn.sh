#!/bin/bash
echo "n $1 $2" | ./NeuralNet c 1 1 3 ../hand_images/test/hands_r ../hand_images/test/other_r train_nn_$1_$2.weights

