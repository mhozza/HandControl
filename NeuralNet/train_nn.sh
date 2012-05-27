#!/bin/bash
echo "n $1 $2"
echo "n $1 $2" | ./NeuralNet c 0 1 3 ../hand_images/hands_r ../hand_images/other_r train_nni_$1_$2.weights

