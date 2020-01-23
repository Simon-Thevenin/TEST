#!/bin/csh

mkdir Results
for T in '10' '20' '30' '40'
do
	mkdir Results/$T
	for S in '6' '12' '18'
	do
		for bh in '2' '8' '12'
		do
			for TBO in '2' '4' '6'
			do
						k=1
						while [ $k -le 5 ]
						do		
							echo  $T $S $TBO $bh $k  
							> Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt
							./bin/Debug/multiper.exe Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt
							let k=1+$k
						done
			done
		done
	done
		
done
	