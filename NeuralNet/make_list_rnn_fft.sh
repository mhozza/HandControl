#!/bin/bash

#cmd='train_nn_fft.sh';
cmd='test_rnn_fft.sh';

echo "#!/bin/bash" > list_rfft.sh;

for i in train_rnn_*_*.weights; do 
j=${i%.weights};
echo ${j:10} >> list_fft.tmp;
done

sed "s/_/ /" < list_fft.tmp > list_fft.tmp2;
sed "s!^\(.*\)\$!echo \\1 \& ./$cmd \\1 | tail -n 6!" < list_fft.tmp2 >> list_rfft.sh;
#sed 's/$/ | tail -n 6/' < list_fft.tmp >> list_fft.sh;
rm list_fft.tmp list_fft.tmp2;

