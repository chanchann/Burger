#!/bin/sh

killall server
timeout=${timeout:-100}
bufsize=${bufsize:-16384}
nothreads=1

for nosessions in 1 10 100 1000 10000; do
# for nosessions in 8000; do
  sleep 5
  echo "Bufsize: $bufsize Threads: $nothreads Sessions: $nosessions"
  taskset -c 1 ../../build/release-cpp11/bin/pingpong_coServer 127.0.0.1 8888 1 2> /dev/null & srvpid=$!
  sleep 1
  taskset -c 2 ../../build/release-cpp11/bin/pingpong_client 127.0.0.1 8888 1 $bufsize $nosessions $timeout 1>> res_co.txt
  kill -9 $srvpid
done
