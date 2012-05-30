#!/bin/bash
#./NeuralNet n 0 1 3 ../hand_images/test/hands_old ../hand_images/test/other_old train_old_img.weights
echo "n $1 $2" | ./NeuralNet c 0 1 3 ../hand_images/test/hands_old ../hand_images/test/other_old train_old_img_$1_$2.weights

