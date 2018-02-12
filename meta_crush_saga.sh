#!/usr/bin/env bash

clear

IFS=

keypressed='None'

while :
do
    epoch_ms=$(date +%s%3N)

    g++ -std=c++1z main.cpp -DKEYBOARD_INPUT="$keypressed" -DEPOCH_MS="$epoch_ms" -o renderer

    if [ $? -ne 0 ]; then
        echo "      ---------------------------"
        echo "      -                         -"
        echo "      -        GAME OVER        -"
        echo "      -                         -"
        echo "      ---------------------------"
        break
    fi

    clear
    
    current_state=$(./renderer)
    echo current_state

    while :
    do 
        keypressed=''
        read -t0.1 -n1 key

        if [ $? -ne 0 ]; then
            keypressed='None'
            break
        fi

        if [ "$key" == ' ' ]
        then
            keypressed='Space'
        elif [ "$key" == 'n' ]
        then 
            keypressed='None'
            break
        else
            read -n2 key

            echo "$key"

            if [ "$key" == '[A' ]
            then
                keypressed='Up'
            elif [ "$key" == '[B' ]
            then
                keypressed='Down'
            elif [ "$key" == '[C' ]
            then
                keypressed='Right'
            elif [ "$key" == '[D' ]
            then
                keypressed='Left'
            fi
        fi

        if [ "$keypressed" != '' ]
        then
            break
        fi  
    done

    echo "R\"(" > current_state.txt
    echo $current_state >> game.txt
    echo ")\"" >> current_state.txt
done
