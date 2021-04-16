#ifndef BASEGRAPH_HPP_
#define BASEGRAPH_HPP_

#include <string>
#include <stdint.h>

namespace webgraph {
// offer base interface for all other graphs: ascii_graph, bv_graph

template<class node_iterator>
class base_graph {
    public:
        virtual std::pair<node_iterator, node_iterator> get_node_iterator(int from) const = 0;
        virtual long get_num_nodes() const = 0;
        virtual long get_num_edges() const = 0;
};
} // namespace webgraph

#endif /*BASEGRAPH_HPP_*/