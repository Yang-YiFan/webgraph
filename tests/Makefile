include ../flags.mk

all: compress_webgraph print_webgraph

compress_webgraph: compress_webgraph.o
	g++ -L$(LIBS) -o compress_webgraph compress_webgraph.o \
			 -lwebgraph -lboost_regex -lboost_program_options -lboost_filesystem -lboost_system

print_webgraph: print_webgraph.o
	make print_webgraph.o
	make -C ../webgraph everything_for_webgraph
	g++ $(FLAGS) -o print_webgraph print_webgraph.o \
				 ../webgraph/webgraph.o ../asciigraph/offline_graph.o \
				 ../webgraph/iterators/node_iterator.o \
				 ../bitstreams/input_bitstream.o \
				 ../bitstreams/output_bitstream.o \
				 ../webgraph/compression_flags.o \
				 ../utils/fast.o \
                                 ../asciigraph/offline_vertex_iterator.o \
				 ../properties/properties.o \
				 ../asciigraph/offline_edge_iterator.o \
				 -lboost_regex -lboost_filesystem -lboost_program_options -lboost_system

install:
	cp compress_webgraph ~/random-bin

%.o : %.cpp
	g++ -I$(INCLUDES) $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
