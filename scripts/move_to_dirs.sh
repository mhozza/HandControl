#!/bin/bash
echo "Moving hands..."
#hands
for j in `cat hands.txt`; do
i=${j%.pbm}
echo $i;
#mv ../old/$i.pbm ../hands_old/
mv ../new/$i.pbm ../hands/
mv ../old/$i.trn ../hands_old_fft/
mv ../new/$i.trn ../hands_fft/
mv ../old/$i.trn.pbm ../hands_old_fft_images/
mv ../new/$i.trn.pbm ../hands_fft_images/
done

echo "Moving other..."
#other
mv ../old/*.trn.pbm ../other_old_fft_images/
mv ../new/*.trn.pbm ../other_fft_images/
mv ../old/*.trn ../other_old_fft/
mv ../new/*.trn ../other_fft/
mv ../old/*.pbm ../other_old/
mv ../new/*.pbm ../other/

echo "[Done]"