#!/bin/bash

mkdir ../test

mkdir ../test/hands
mkdir ../test/other
mkdir ../test/hands_fft
mkdir ../test/other_fft
mkdir ../test/hands_fft_images
mkdir ../test/other_fft_images

mkdir ../test/hands_old
mkdir ../test/other_old
mkdir ../test/hands_old_fft
mkdir ../test/other_old_fft
mkdir ../test/hands_old_fft_images
mkdir ../test/other_old_fft_images

echo "Moving hands..."
#hands
for j in `cat hands.txt`; do
i=${j%.pbm}
echo $i;
#mv ../old/$i.pbm ../hands_old/
mv ../new/$i.pbm ../test/hands/
mv ../old/$i.trn ../test/hands_old_fft/
mv ../new/$i.trn ../test/hands_fft/
mv ../old/$i.trn.pbm ../test/hands_old_fft_images/
mv ../new/$i.trn.pbm ../test/hands_fft_images/
done

echo "Moving other..."
#other
mv ../old/*.trn.pbm ../test/other_old_fft_images/
mv ../new/*.trn.pbm ../test/other_fft_images/
mv ../old/*.trn ../test/other_old_fft/
mv ../new/*.trn ../test/other_fft/
mv ../old/*.pbm ../test/other_old/
mv ../new/*.pbm ../test/other/

echo "[Done]"

