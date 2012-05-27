#!/bin/bash

#cmd='train_nn_fft.sh';
cmd='test_nn_fft2u_tr.sh';

echo "#!/bin/bash" > list_fft2u_train.sh;

for i in train_nn2u_*.weights; do 
j=${i%.weights};
echo ${j:11} >> list_fft.tmp;
done

#sed "s/_/ /" < list_fft.tmp > list_fft.tmp2;
sed "s!^\(.*\)\$!echo \\1 \& ./$cmd \\1 | tail -n 6!" < list_fft.tmp >> list_fft2u_train.sh;
#sed 's/$/ | tail -n 6/' < list_fft.tmp >> list_fft.sh;
rm list_fft.tmp;
#list_fft.tmp2;

