# diseasesFile.txt $1
# countriesFile.txt $2
# DIR $3
# numFilesPerDirectory $4
# numRecordsPerFile $5

if [ "$#" -ne 5 ]; then
    echo "You must enter 5 arguments!"
    exit 1
fi

first_name[0]="Adam"
first_name[1]="Barbara"
first_name[2]="Kevin"
first_name[3]="Sandra"
first_name[4]="James"
first_name[5]="Susan"
first_name[6]="Robert"
first_name[7]="Karen"
first_name[8]="David"
first_name[9]="Lisa"


last_name[0]="Aalen"
last_name[1]="Boonie"
last_name[2]="Smith"
last_name[3]="Silva"
last_name[4]="Garcia"
last_name[5]="Jones"
last_name[6]="Wilson"
last_name[7]="Taylor"
last_name[8]="Harris"
last_name[9]="Clark"

size=${#first_name[@]}


input="countriesFile.txt"
country="diseasesFile.txt"

while IFS= read -r line
do
    fileCounter=0
    mkdir -p $3/$line
    while [ $fileCounter -lt $4 ]
    do
        #dhmiourgia random hmeras
        day=$((1 + RANDOM % 29))
        if [ "$day" -le 9 ]; then
            day=0$day
        fi
        #dhmiourgia random mhna
        month=$((1 + RANDOM % 11))
        if [ "$month" -le 9 ]; then
            month=0$month
        fi
        #dhmiourgia random xronou metaksi 1900-2020
        date=$day-$month-$((RANDOM %120 + 1900))

        entryCounter=0
        while [ $entryCounter -lt $5 ]
        do
            id=$((RANDOM % 999))
            #to id metaksi 1-999 me mhdenika mprosta an xreiastei
            if [ "$id" -le 9 ]; then
            id=00$id
            fi

            if [ "$id" -le 99 ]; then
            id=0$id
            fi
            
            state="EXIT"
            ran=$((RANDOM % 10))
            # 60% pithanothta na mpei
            if [ "$ran" -le 6 ]; then
                state="ENTER"
            fi

            #create the entry
            entry=$id" "$state" "${first_name[$(($RANDOM % $size))]}" "${last_name[$(($RANDOM % $size))]}" "$(shuf -n 1 $country)" "$((RANDOM % 120))
            echo $entry >> $3/$line/$date.txt
            ((entryCounter++))
        done
        ((fileCounter++))
    done
done < "$input"