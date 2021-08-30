
declare -a DATASETS=(
    "pd_mc.txt"
)

find_datasets(){
    for DATASET in ${DATASETS[@]}; do
	echo "#############"
	echo $DATASET
	echo "#############"
	echo
	while read i; do
	    if [ "$i" == "" ]; then
		continue
	    fi
	    echo -e "\033[1m$i\033[0m"
	    for y in 16 17 18; do
		sd="RunIISummer20UL${y}MiniAOD*v2-106X*-v*/MINIAODSIM"
		full="$i/$sd"
		echo -e "\033[4msearching for $sd ... \033[0m"
		dasgoclient -query="dataset status=* dataset=$full"
		dasgoclient -query="dataset status=* dataset=$full | grep dataset.status"
		echo
	    done
	    echo
	done <$DATASET
    done
}

find_datasets | tee find_$(date '+%Y-%m-%d_%H-%M-%S').log
