#!/usr/bin/env bash

clear

IFS=

keypressed='None'

while :
do
    epoch_ms=$(date +%s%3N)
    
    g++ -std=c++1z main.cpp -DKEYBOARD_INPUT="$keypressed" -DEPOCH_MS="$epoch_ms"  &
    wait

    if [ $? -ne 0 ]; then
        break
    fi

    clear
    echo "            Meta Crush Saga"
    echo "------------------------------------------"
    echo

    board_output=$(./a.out)
    echo $board_output

    while :
    do 
        keypressed=''
        read -t1 -n1 key
#        read -n1 key

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

    echo $keypressed
    echo "R\"(" > board.txt
    echo $board_output >> board.txt
    echo ")\"" >> board.txt
done