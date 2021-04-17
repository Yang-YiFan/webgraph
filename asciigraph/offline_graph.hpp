/*               
 * Portions copyright (c) 2003-2007, Paolo Boldi and Sebastiano Vigna. Translation copyright (c) 2007, Jacob Ratkiewicz
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef ASCIIGRAPH_HPP_
#define ASCIIGRAPH_HPP_

#include <string>
#include <fstream>
#include <tuple>
#include <boost/graph/adjacency_iterator.hpp>

#include "../webgraph/webgraph_vertex.hpp"
#include "offline_edge_iterator.hpp"
#include "offline_vertex_iterator.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "base_graph.hpp"

using namespace std;

namespace webgraph { namespace ascii_graph {
/*!
 * ascii_graph::offline_graph represents an offline ASCII graph (not loaded into memory).
 * 
 * Access to members of the graph is through getting an iterator, then working with the iterator.
 */
class offline_graph : public base_graph<offline_vertex_iterator> {
   
private:
   // Only used by static factory method load.
   offline_graph();
   
protected:
   long n; // num vertices
   long num_edges;
   string filename; // file containing the graph

public:
   virtual ~offline_graph();

   static offline_graph load( const string& basename );

   typedef offline_edge_iterator edge_iterator;
   typedef offline_vertex_iterator node_iterator;

   typedef vertex_descriptor vertex_type;
   typedef edge edge_type; 

   std::pair<edge_iterator, edge_iterator> get_edge_iterator() const;
   std::pair<node_iterator, node_iterator> get_node_iterator(int from = 0) const;

   long get_num_nodes() const {
      return n;
   }

   long get_num_edges() const {
      return num_edges;
   }
};
} } // namespace webgraph

////////////////////////////////////////////////////////////////////////////////

#endif /*ASCIIGRAPH_HPP_*/
