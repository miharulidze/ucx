#!/usr/bin/python3

import sys

def check_perf_results(filename, threshold):
    max_regression = 0
    test_name_max = test_name = None
    with open(filename, 'r') as f:
        for line in f:
            if '+osu' in line:
                test_name = line.split()[0].strip('+')
            elif '% worse' in line:
                worse_value = float(line.split('%')[0].split()[-1])
                if worse_value > max_regression:
                    max_regression = worse_value
                    test_name_max = test_name
    if max_regression > threshold:
        print(f"Max performance regression detected in {test_name_max}, regression: {max_regression}%")
        print("##vso[task.complete result=Failed;]Performance regression detected")
        exit(1)
    print("No significant performance regression detected.")
    return 0

if __name__ == "__main__":
    check_perf_results(sys.argv[1], float(sys.argv[2]))
