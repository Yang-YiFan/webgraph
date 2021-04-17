#ifndef CSRNODEITERATOR_HPP_
#define CSRNODEITERATOR_HPP_

#include <assert.h>
#include <vector>
#include <boost/iterator/iterator_facade.hpp>
#include "base_node_iterator.hpp"

namespace webgraph { namespace csrgraph {

class csr_node_iterator : public boost::iterator_facade<
        csr_node_iterator,
        vertex_label_t,
        boost::forward_traversal_tag,
        vertex_label_t>, public base_node_iterator {

    private:
        long numVertices;
        // the offsets of current vertex
        vertex_label_t* offsets;
        // the edges of current vertex
        vertex_label_t* edges;
        int outdegree;
        /** The index of the node just before the next one. */
        int curr;
        /** wether this is meant to stand as an end marker */
        bool end_marker;

        // aux data structure helps to pass successor vector
        // multithread unsafe!!!
        std::vector<vertex_label_t> buffer;

    public:
        csr_node_iterator() {
            numVertices = 0LL;
            offsets = nullptr;
            edges = nullptr;
            outdegree = 0;
            end_marker = true;
            buffer.clear();
        }

        csr_node_iterator(long _numVertices, vertex_label_t* _offsets, vertex_label_t* _edges, int _curr) :
            numVertices(_numVertices), offsets(_offsets), edges(_edges), curr(_curr) {
            buffer.clear();
            assert(curr <= numVertices);
            outdegree = offsets[1] - offsets[0];
            if(curr == numVertices)
                end_marker = true;
            else
                end_marker = false;
        }

        csr_node_iterator(const csr_node_iterator& rhs) {
            copy(rhs);
        }

        ~csr_node_iterator() { }

        csr_node_iterator& operator = (const csr_node_iterator& rhs) {
            copy(rhs);
            return *this;
        }

    private:
        void copy( const csr_node_iterator& other );
    
    // Backing methods for iterator_facade
    private:
        friend class boost::iterator_core_access;

        void increment();

        bool equal( const csr_node_iterator& rhs ) const;

        vertex_label_t dereference() const {
            return curr;
        }

    public:
        const std::vector<vertex_label_t>& successor_vector();
        int outdegree();

};

} } // namespace webgraph

#endif /*CSRNODEITERATOR_HPP_*/