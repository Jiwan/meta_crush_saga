#!/bin/sh

time_tool=/usr/bin/time
perftest_path=./performance_test
epoch_ms=1518906244300

measure_build_time() {
    game_field=$1
    keypressed=$2
    cp $perftest_path/$game_field current_state.txt
    sum=0
    printf "field=%-20s key=%-5s, measuring " $game_field $keypressed 

    for i in $(seq 1 10)
    do
        printf "."
        res[i]=$($time_tool -f %e g++ -std=c++1z main.cpp -DKEYBOARD_INPUT="$keypressed" -DEPOCH_MS="$epoch_ms" -o renderer 2>&1)
        sum=$(echo "$sum + ${res[i]}" | bc)
    done

    avg=$(echo "$sum / ${#res[@]}" | bc -l)
    
    variance=0
    for i in $(seq 1 ${#res[@]})
    do
        variance=$(echo "$variance + (${res[i]} - $avg) * (${res[i]} - $avg)" | bc -l)
    done  

    variance=$(echo "$variance / (${#res[@]} - 1)" | bc -l)
    std=$(echo "sqrt($variance)" | bc -l)
    rsd=$(echo "100 * $std / $avg" | bc -l)
    printf "\ttime=%0.3fs (%0.0f%%)\n" $avg $rsd
}

measure_build_time default.txt 'None'
measure_build_time default.txt 'Up'
measure_build_time default.txt 'Down'
measure_build_time default.txt 'Left'
measure_build_time default.txt 'Right'
measure_build_time default.txt 'Space'
measure_build_time swapping.txt 'Space'
measure_build_time matched.txt 'None'
measure_build_time falldown.txt 'None'

# [eof]
