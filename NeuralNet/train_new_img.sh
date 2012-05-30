#!/bin/bash
echo "n $1 $2" | ./NeuralNet c $3 1 1 ../hand_images/hands ../hand_images/other train_new_img_$1_$2.weights
#./NeuralNet n 0 1 1 ../hand_images/hands ../hand_images/other train_new_img.weights
#echo "n $1" | ./NeuralNet c $2 1 3 ../hand_images/hands ../hand_images/other train_new_img.weights

