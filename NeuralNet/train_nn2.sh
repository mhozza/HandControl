#!/bin/bash
echo "n $1" | ./NeuralNet2 c 0 1 3 ../hand_images/hands_r ../hand_images/other_r train_nn2i_$1.weights

