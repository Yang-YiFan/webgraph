#ifndef BASENODEITERATOR_HPP_
#define BASENODEITERATOR_HPP_

#include <utility>
#include <vector>

namespace webgraph {
// offer base interface for node iterators of all other graphs: offline_vertex_iterator, node_iterator

class base_node_iterator {
    public:
        virtual const std::vector<int>& successor_vector() = 0;
        virtual int outdegree() = 0;
};
} // namespace webgraph

#endif /*BASENODEITERATOR_HPP_*/
