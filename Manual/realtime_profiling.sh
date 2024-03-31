#!/bin/bash

while true; do
    perf record -g -p 67912 -o perf.data.tmp && \
    perf script -i perf.data.tmp | ~/FlameGraph/stackcollapse-perf.pl | ~/FlameGraph/flamegraph.pl 2> error.log > perf.svg
    sleep 1  # Adjust sleep time based on how frequently you want to update the graph
done

