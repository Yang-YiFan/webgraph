all: compute_indegree compute_outdegree

include ../flags.mk

INCLUDES = /data/sanchez/users/yifany/graph_compression_prj/webgraph/bitstreams
LIBS = /data/sanchez/users/yifany/graph_compression_prj/webgraph/bitstreams

bitstream_stress_test: bitstream_stress_test.o
	g++ $(FLAGS) -o bitstream_stress_test -I$(INCLUDES) -L$(LIBS) \
			-L../bitstreams bitstream_stress_test.o \
			-lbitstreams -lutil

compute_indegree: compute_indegree.o
	g++ $(FLAGS) -o compute_indegree compute_indegree.o -L.. \
			-lwebgraph -lboost_filesystem -lboost_regex -lboost_system

compute_outdegree: compute_outdegree.o
	g++ $(FLAGS) -o compute_outdegree compute_outdegree.o -L.. \
			-lwebgraph -lboost_filesystem -lboost_regex -lboost_system

%.o: %.cpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
