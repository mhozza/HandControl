#!/bin/bash
#./NeuralNet n $1 1 3 ../hand_images/hands_old ../hand_images/other_old train_old_img.weights
echo "n $1 $2" | ./NeuralNet c $3 1 3 ../hand_images/hands_old ../hand_images/other_old train_old_img_$1_$2.weights
