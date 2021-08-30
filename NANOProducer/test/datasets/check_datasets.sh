
declare -a DATASETS=(
    "data_2016.txt"
    "data_2017.txt"
    "data_2018.txt"
)

get_entries(){
    for DATASET in ${DATASETS[@]}; do
	echo "#############"
	echo $DATASET
	echo "#############"
	echo
	while read i; do
	    if [ "$i" == "" ]; then
		continue
	    fi
	    echo $i
	    dasgoclient -query="dataset=$i | grep dataset.status" | awk '{$1=$1};1' | grep -v -e '^$' | tail -n 1
	    dasgoclient -query="dataset=$i | grep dataset.nevents" | awk '{$1=$1};1' | grep -v -e '^$'
	    echo
	done <$DATASET
    done
}

get_entries | tee check_$(date '+%Y-%m-%d_%H-%M-%S').log
