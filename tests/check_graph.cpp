#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <assert.h>
#include <stdint.h>

#include "webgraph/webgraph.hpp"

// first arg: bvg graph name
// second arg: ascii graph name

int main( int argc, char** argv ) {
    namespace ag = webgraph::ascii_graph;
    namespace bvg = webgraph::bv_graph;

    if( argc < 3 ) {
        std::cerr << "You have to provide names of two graph files.\n";
        return 1;
    }
    std::string bvg_name = argv[1];
    std::string ag_name = argv[2];

    typedef std::shared_ptr<bvg::graph> bvg_ptr;

    bvg_ptr bvg_gp = bvg::graph::load( bvg_name );
    bvg::graph::node_iterator bvg_n, bvg_n_end;
    std::tie(bvg_n, bvg_n_end) = bvg_gp->get_node_iterator(0);

    ag::offline_graph ag_gp = ag::offline_graph::load(ag_name);
    ag::offline_graph::node_iterator ag_n, ag_n_end;
    std::tie(ag_n, ag_n_end) = ag_gp.get_node_iterator(0);

    std::cerr << "num vertices: "<< bvg_gp->get_num_nodes() << std::endl;
    std::cerr << "num edges: "<< bvg_gp->get_num_edges() << std::endl;

    assert(bvg_gp->get_num_nodes() == ag_gp.get_num_nodes());
    assert(bvg_gp->get_num_edges() == ag_gp.get_num_edges());

    while( bvg_n != bvg_n_end ) {
        std::vector<int> bvg_successors;
        bvg_successors = bvg_n.successor_vector();

        std::vector<int> ag_successors;
        ag_successors = ag_n.successor_vector();

        assert(*bvg_n == *ag_n);
        assert(bvg_n.outdegree() == ag_n.outdegree());
        assert(bvg_successors.size() == ag_successors.size());
        assert((uint32_t)bvg_n.outdegree() == bvg_successors.size());

        //std::cerr<<"vertex: "<<*bvg_n<<std::endl;
        for(uint32_t i=0; i<bvg_successors.size(); i++) {
            assert(bvg_successors.at(i) == ag_successors.at(i));
            //std::cerr<<bvg_successors.at(i)<<", ";
        }
        //std::cerr<<std::endl;

        ++bvg_n;
        ++ag_n;
    }
    assert(ag_n == ag_n_end);
    std::cerr<<"OK!"<<std::endl;

    return 0;
}
