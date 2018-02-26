#!/usr/bin/python3

import subprocess
import shutil
import statistics


def measure_build_time(game_field='default.txt', kbdinput='None'):
    dkeyboad='-DKEYBOARD_INPUT=' + kbdinput
    depoch='-DEPOCH_MS=1518906244300'
    nsamples=20
    results=[]
    shutil.copy( './performance_test/' + game_field, 'current_state.txt')
    print( 'field = {:<15s} key = {:5s}    measuring'.format(game_field, kbdinput), end='', flush=True)
    for i in range(0,nsamples):
        print('.', end='', flush=True)
        results.append(float(subprocess.check_output(['/usr/bin/time', '-f',
            '%e', 'g++', '-std=c++1z', 'main.cpp', dkeyboad, depoch, '-o', 'renderer'],
            stderr=subprocess.STDOUT).decode('utf-8')))
    average=statistics.mean(results)
    rstd=100 * statistics.stdev(results, average) / average
    print('  time = {:<.3f} ({:.0f}%)'.format(average, rstd))


if __name__ == '__main__':
    measure_build_time('default.txt', 'None')
    measure_build_time('default.txt', 'Up')
    measure_build_time('default.txt', 'Down')
    measure_build_time('default.txt', 'Left')
    measure_build_time('default.txt', 'Right')
    measure_build_time('default.txt', 'Space')
    measure_build_time('swapping.txt', 'Space')
    measure_build_time('matched.txt', 'None')
    measure_build_time('falldown.txt', 'None')
 
# [eof]
