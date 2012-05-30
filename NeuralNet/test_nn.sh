#!/bin/bash
echo "n $1 $2" | ./NeuralNet c 0 1 3 ../hand_images/test/hands_r ../hand_images/test/other_r train_nni_$1_$2.weights

