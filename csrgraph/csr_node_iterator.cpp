#include "csr_node_iterator.hpp"

namespace webgraph { namespace csrgraph {

void csr_node_iterator::copy(const csr_node_iterator& other) {
    numVertices = other.numVertices;
    offsets = other.offsets;
    edges = other.edges;
    outdegree = other.outdegree;
    curr = other.curr;
    end_marker = other.end_marker;
}

void csr_node_iterator::increment() {
    curr++;
    if(curr == numVertices) {
        end_marker = true;
    } else {
        assert(curr < numVertices);
        assert(!end_marker);
        offsets += 1;
        edges += outdegree;
        outdegree = offsets[1] - offsets[0];
    }
}

bool csr_node_iterator::equal(const csr_node_iterator& rhs) const {
    if( (rhs.end_marker && numVertices == curr) ||
        (rhs.numVertices == rhs.curr && end_marker) ||
        (rhs.end_marker && end_marker) ||
        (rhs.numVertices == rhs.curr && numVertices == curr) ) {
        return true;
    }
    // otherwise, do a real comparison
    return ((rhs.numVertices == numVertices) &&
            (rhs.offsets == offsets) &&
            (rhs.edges == edges) &&
            (rhs.outdegree == outdegree) &&
            (rhs.curr == curr));
}

const std::vector<vertex_label_t>& csr_node_iterator::successor_vector() {
    buffer = std::vector<vertex_label_t>(edges, edges+outdegree);
    return buffer;
}
   
int csr_node_iterator::outdegree() {
    return outdegree;
}

} } // namespace webgraph