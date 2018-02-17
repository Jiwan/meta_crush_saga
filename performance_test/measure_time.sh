#!/bin/sh

n_measurements=10

time_tool=/usr/bin/time
perftest_path=./performance_test
epoch_ms=1518906244300

measure_build_time() {
    cp $perftest_path/$game_field current_state.txt
    sum=0
    i=0
    printf "measuring "

    while [ $i -lt $n_measurements ]
    do
        printf "."
        res[i]=$($time_tool -f %e g++ -std=c++1z main.cpp -DKEYBOARD_INPUT="$keypressed" -DEPOCH_MS="$epoch_ms" -o renderer 2>&1)
        sum=$(echo "$sum + ${res[i]}" | bc)
        i=$(expr $i + 1)
    done

    printf "\n"
    avg=$(echo "$sum / ${#res[@]}" | bc -l)
    
    variance=0
    i=0

    while [ $i -lt $n_measurements ]
    do
        variance=$(echo "$variance + (${res[i]} - $avg) * (${res[i]} - $avg)" | bc -l)
        i=$(expr $i + 1)
    done  

    variance=$(echo "$variance / ($n_measurements - 1)" | bc -l)
    std=$(echo "sqrt($variance)" | bc -l)
    rsd=$(echo "100 * $std / $avg" | bc -l)
    printf "field=%-20s key=%-5s: %0.3fs (%0.0f%%)\n" $game_field $keypressed $avg $rsd
}


game_field=default.txt
keypressed='None'
measure_build_time

game_field=default.txt
keypressed='Up'
measure_build_time

game_field=default.txt
keypressed='Down'
measure_build_time

game_field=default.txt
keypressed='Left'
measure_build_time

game_field=default.txt
keypressed='Right'
measure_build_time

game_field=default.txt
keypressed='Space'
measure_build_time

game_field=swapping.txt
keypressed='Space'
measure_build_time

game_field=matched.txt
keypressed='None'
measure_build_time

game_field=falldown.txt
keypressed='None'
measure_build_time

# [eof]
