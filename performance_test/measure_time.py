#!/usr/bin/python3

import subprocess
import shutil
import statistics
from argparse import ArgumentParser
import json

default_nsamples = 10
usage_scenarios = [('default.txt', 'None'), ('default.txt', 'Up'),
                   ('default.txt', 'Down'), ('default.txt', 'Left'),
                   ('default.txt', 'Right'), ('default.txt', 'Space'),
                   ('swapping.txt', 'Space'), ('matched.txt', 'None'),
                   ('falldown.txt', 'None')]
version_major = 1
version_minor = 0
version_patch = 0


class BuildTimeMeasurer:
    def __init__(self, nsamples, filename):
        self.nsamples = nsamples
        self.filename = filename
        self.aggregated_result = []

    def measure(self, game_field, kbdinput):
        dkeyboad = '-DKEYBOARD_INPUT=' + kbdinput
        depoch = '-DEPOCH_MS=1518906244300'
        results = []
        shutil.copy('./performance_test/' + game_field, 'current_state.txt')
        print('field = {:<15s} key = {:5s}    measuring '.format(
            game_field, kbdinput),
            end='',
            flush=True)
        for i in range(0, self.nsamples):
            print('.', end='', flush=True)
            results.append(
                float(
                    subprocess.check_output(
                        [
                            '/usr/bin/time', '-f', '%e', 'g++', '-std=c++1z',
                            '-fconcepts', 'main.cpp', dkeyboad, depoch, '-o',
                            'renderer'
                        ],
                        stderr=subprocess.STDOUT).decode('utf-8')))
        tmean = statistics.mean(results)
        rstdev = 100 * statistics.stdev(results, tmean) / tmean
        print('  time = {:<.3f} ({:.0f}%)'.format(tmean, rstdev))
        self.aggregated_result.append({
            'playfield': game_field,
            'KEYBOARD_INPUT': kbdinput,
            'nsamples': self.nsamples,
            'mean': tmean,
            'rstdev': rstdev
        })

    def provide_aggregated_result(self):
        if self.filename is not None:
            with open(self.filename, 'w') as outfile:
                json.dump(self.aggregated_result, outfile)


if __name__ == '__main__':
    parser = ArgumentParser(
        description=('Utility to measure build time for MCS. '
                     'Shall be executed in the application root directory'))
    parser.add_argument(
        "-n",
        "--nsamples",
        dest="nsamples",
        metavar="N",
        type=int,
        default=default_nsamples,
        help='number of samples per usage scenario, default {}'.format(
            default_nsamples))
    parser.add_argument("-o",
                        "--outfile",
                        dest="outfile",
                        help="file to store measurement results",
                        metavar="FILENAME")
    parser.add_argument("--version",
                        action="version",
                        help="show version",
                        version='%(prog)s {}.{}.{}'.format(
                            version_major, version_minor, version_patch))
    args = parser.parse_args()

    btm = BuildTimeMeasurer(int(args.nsamples), args.outfile)
    for game_field, kbdinput in usage_scenarios:
        btm.measure(game_field, kbdinput)
    btm.provide_aggregated_result()

# [eof]
