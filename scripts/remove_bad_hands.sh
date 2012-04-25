#!/bin/bash

mkdir ../hands_old/nu
mkdir ../hands_old_fft/nu
mkdir ../hands_old_fft_images/nu

echo "Removing bad hands..."
for j in `cat remove.txt`; do
i=${j%.pbm}
echo $i;
#rm ../hands/$i.pbm
rm ../hands_fft/$i.trn
rm ../hands_fft_images/$i.trn.pbm

mv ../hands_old/$i.pbm ../hands_old/nu
mv ../hands_old_fft/$i.trn ../hands_old_fft/nu
mv ../hands_old_fft_images/$i.trn.pbm ../hands_old_fft_images/nu
done
echo "[Done]"

