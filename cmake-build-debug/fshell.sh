#!/bin/csh

mkdir Results
for T in '10' '20' '30' '40'
do
	mkdir Results/$T
	for S in '6' #'12' '18'
	do
		for bh in '2' #'8' '12'
		do
			for TBO in '4' #2'  '4' '6'
			do
						k=1
						while [ $k -le 2 ]
						do
						  for gamma in '1' '3' '5'
			        do
							echo  ./Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S $gamma
							> Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt
							./untitled.exe Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S $gamma Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt
							let k=1+$k
							done
						done
			done
		done
	done
		
done
	