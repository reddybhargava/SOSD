mkdir -p results_csv/perf/results/
mkdir -p results_csv/cold/results/
mkdir -p results_csv/lookups/results/
mkdir -p results_csv/inserts/results/


function perf() {
	for filename in results/*perf*.txt; do
		awk  -F , '{gsub(/RESULT:[ ]*/,"")}1 && (NF == 20 || NF == 22)' $filename > results_csv/perf/$filename
	done
}

function cold() {
	for filename in results/*cold*.txt; do
		awk  -F , '{gsub(/RESULT:/,"")}1 && NF == 6' $filename > results_csv/cold/$filename
	done
}

function lookups() {
	for filename in results/*perf*.txt; do
		awk  -F , '{gsub(/RESULT:/,"")}1 && NF == 6' $filename > results_csv/lookups/$filename
	done
}

function inserts() {
	for filename in results/*perf*.txt; do
		awk  -F , '{gsub(/RESULT:/,"")}1 && NF == 7' $filename > results_csv/inserts/$filename
	done
}


case $1 in
	"perf") 
		perf
	;;
	"cold") 
		cold
	;;
esac
