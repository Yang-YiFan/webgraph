#!/bin/bash

check() {
  ./build/tests/compress_webgraph --source=tests/graphs/$1 --dest=tests/graphs/hello
  sleep 3
  ./build/tests/print_webgraph tests/graphs/hello
  sleep 3
  ./build/tests/check_graph tests/graphs/hello tests/graphs/$1
  sleep 3

  ./build/tests/compress_webgraph_csr --source=tests/graphs/$1 --dest=tests/graphs/hello
  sleep 3
  ./build/tests/print_webgraph tests/graphs/hello
  sleep 3
  ./build/tests/check_graph tests/graphs/hello tests/graphs/$1
  sleep 3

  echo "graph $1 check passed!"
}

check $1
