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

							echo  ./Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S $gamma
							> Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt
							env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S "1" Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt &
							env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S "3" Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt  &
							env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S "5" Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt &
							env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt $T "12" "1" Results/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt &
							env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt $T "12" "3" Results/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt  &
							env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt $T "12" "5" Results/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt &
							 wait

							let k=1+$k

						done
			done
		done
	done
		
done
	