#!/bin/csh

mkdir Results
for T in '10' #'20' '30' '40'
do
	mkdir Results/$T
	for S in '6' #'12' '18'
	do
		for bh in '2' '8' # '12'
		do
			for TBO in '4' #2'  '4' '6'
			do
						k=1
						while [ $k -le 2 ]
						do

							echo  ./Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S $gamma
							T2=$T+10
							T3=$T+20
							> Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt
							(env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S "1" Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt &&			env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog Data/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S "3" Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt  &&	env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt $T "12" "1" Results/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt  &&			env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt $T "12" "3" Results/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt) &
							#env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T2"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S "1" Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt [${e}] &
							#env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog Data/$T/"D_"$T2"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S "3" Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt  [${f}] &
							#env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T2"_12_"$TBO"_"$bh"_"$k.txt $T "12" "1" Results/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt [${g}] &
							#env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T2"_12_"$TBO"_"$bh"_"$k.txt $T "12" "3" Results/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt [${h}]  &
							#env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T3"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S "1" Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt [${i}] &
							#env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog Data/$T/"D_"$T3"_"$S"_"$TBO"_"$bh"_"$k.txt $T $S "3" Results/$T/"D_"$T"_"$S"_"$TBO"_"$bh"_"$k.txt  [${j}] &
							#env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T3"_12_"$TBO"_"$bh"_"$k.txt $T "12" "1" Results/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt [${k}] &
							#env LD_LIBRARY_PATH=/home-isis/auto/dapi/s18theve/xp/lib/ ./myprog  Data/$T/"D_"$T3"_12_"$TBO"_"$bh"_"$k.txt $T "12" "3" Results/$T/"D_"$T"_12_"$TBO"_"$bh"_"$k.txt [${l}]  &

							k = $k+1

						done
			done
		done
	done
		
done
	