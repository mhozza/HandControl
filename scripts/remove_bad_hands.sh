#!/bin/bash
echo "Removing bad hands..."
for j in `cat remove.txt`; do
i=${j%.pbm}
echo $i;
#rm ../hands/$i.pbm
rm ../hands_fft/$i.trn
rm ../hands_fft_images/$i.trn.pbm
done
echo "[Done]"