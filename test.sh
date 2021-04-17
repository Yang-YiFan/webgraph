./build/tests/compress_webgraph --source=tests/graphs/rand1 --dest=tests/graphs/hello
sleep 5
./build/tests/print_webgraph tests/graphs/hello
sleep 5
./build/tests/check_graph tests/graphs/hello tests/graphs/rand1
