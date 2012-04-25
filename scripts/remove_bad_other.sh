#!/bin/bash
echo "Removing bad hands..."
ls -1 *.pbm> remove.txt
for j in `cat remove.txt`; do
i=${j%.pbm}
echo $i;
rm ../other*/$i.*
rm ./$j
done
rm remove.txt
echo "[Done]"

