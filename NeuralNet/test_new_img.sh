#!/bin/bash
#./NeuralNet n 0 1 1 ../hand_images/test/hands ../hand_images/test/other train_new_img.weights
echo "n $1 $2" | ./NeuralNet c 0 1 1 ../hand_images/test/hands ../hand_images/test/other train_new_img_$1_$2.weights
