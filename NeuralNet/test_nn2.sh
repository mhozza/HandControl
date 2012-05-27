#!/bin/bash
echo "n $1" | ./NeuralNet2 c 1 1 3 ../hand_images/test/hands_r ../hand_images/test/other_r train_nn2i_$1.weights

