#ifndef BASENODEITERATOR_HPP_
#define BASENODEITERATOR_HPP_

#include <utility>
#include <vector>

namespace webgraph {

// unsigned it might be wrong, because there is delta encoding going on
typedef int vertex_label_t;

// offer base interface for node iterators of all other graphs: offline_vertex_iterator, node_iterator
class base_node_iterator {
    public:
        virtual const std::vector<vertex_label_t>& successor_vector() = 0;
        virtual int outdegree() = 0;
};
} // namespace webgraph

#endif /*BASENODEITERATOR_HPP_*/
