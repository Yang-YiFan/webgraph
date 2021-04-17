#ifndef CSRGRAPH_HPP_
#define CSRGRAPH_HPP_

#include <stdint.h>
#include <tuple>

#include "base_graph.hpp"
#include "csr_node_iterator.hpp"

namespace webgraph { namespace csrgraph {

// csr frontend, can be compressed into bv_graph
// also enable integration with libworklist
class csr_graph : public base_graph<csr_node_iterator> {
    private:
        long numVertices;
        long numEdges;
        // the csr array is managed externally, i.e. in libworklist
        vertex_label_t* offsets;
        vertex_label_t* edges;

    public:
        csr_graph(long _numVertices, long _numEdges, vertex_label_t* _offsets, vertex_label_t* _edges) :
            numVertices(_numVertices), numEdges(_numEdges), offsets(_offsets), edges(_edges) { }

        long get_num_nodes() const { return numVertices; }
        long get_num_edges() const { return numEdges; }

        typedef csr_node_iterator node_iterator;
        std::pair<node_iterator, node_iterator> get_node_iterator(int from = 0) const {
            return std::make_pair( csr_node_iterator(numVertices, offsets+from, edges+offsets[from], from),
                                   csr_node_iterator());
        }
};

} } // namespace webgraph

#endif /*CSRGRAPH_HPP_*/