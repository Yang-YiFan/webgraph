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

#include "offline_graph.hpp"

#include <sstream>
#include <iostream>

namespace webgraph { namespace ascii_graph {

////////////////////////////////////////////////////////////////////////////////
offline_graph::offline_graph() : n(0LL), num_edges(0LL)
{
}

////////////////////////////////////////////////////////////////////////////////
offline_graph::~offline_graph()
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Loads the graph offline (without offsets)
 * 
 * All this method really does is set up a new SmartASCIIGraph with a stream tokenizer
 * that will read the file when an iterator is used.
 */
offline_graph offline_graph::load( const string& basename ) {
   // all this method does is a quick sanity check before setting up some state. 
   // Nothing is really read until iterators start getting created.

   offline_graph result;

   result.filename = basename + ".graph-txt";

   ifstream file( result.filename.c_str() );

   string nextline;

   getline( file, nextline );

   // read number of nodes
   istringstream nl( nextline );
        
   nl >> result.n;

   assert( result.n > 0 );

   // now read number of edges
   result.num_edges = 0LL;
   edge_iterator b, e;
   std::tie( b, e ) = result.get_edge_iterator();
   for( ; b != e; ++b ) {
      result.num_edges++;
   }

   return result;
}
        
////////////////////////////////////////////////////////////////////////////////
/**
 * 
 */
std::pair<offline_graph::node_iterator, offline_graph::node_iterator>
offline_graph::get_node_iterator ( int from ) const {
   if( from != 0 ) {
      cerr << "from is not implemented.\n";
   }

   return std::make_pair( offline_vertex_iterator( filename.c_str() ),
                          offline_vertex_iterator() );

//   return make_pair(offline_vertex_iterator( filename.c_str(), from ), 
//		    offline_vertex_iterator(filename.c_str(), n));
}

////////////////////////////////////////////////////////////////////////////////
/// Gets the beginning and end edge iterators.
/*!
 * \return A std::pair containing the beginning and end edge iterators.
 */
std::pair<offline_graph::edge_iterator, offline_graph::edge_iterator>
offline_graph::get_edge_iterator() const {
   edge_iterator begin( filename.c_str() ); 
   edge_iterator end;
   
   return std::make_pair( begin, end );
}

} }
