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

#include "node_iterator.hpp"
#include "iterator_wrappers.hpp"

#include <utility>
#include <algorithm>
#include <cassert>

#include "../webgraph.hpp"

using namespace std;
#ifndef CONFIG_FAST
using namespace logs;
#endif
   
namespace webgraph { namespace bv_graph {

using namespace utility_iterators;

node_iterator::node_iterator( const graph* owner, std::shared_ptr<ibitstream> is, 
                              int from, int window_size ) :
      ibs(is) {
      this->from = from;
      this->cyclic_buffer_size = window_size + 1;
      this->owner = owner;
      this->n = owner->get_num_nodes();
      this->end_marker = false;
      this->avail = 0;
      this->window.resize( cyclic_buffer_size );
      for( vector< vector<vertex_label_t> >::iterator itor = window.begin();
           itor != window.end();
           itor++ ) {
        itor->resize( graph::INITIAL_SUCCESSOR_LIST_LENGTH );
      }
         
      this->outd.resize( cyclic_buffer_size );
//    this->block_outdegrees = NULL; // so long as offset step = 1 TODO change this

      if ( from != 0 ) {
        owner->position_no_read( *ibs, from );
      }
      curr = from - 1;
      increment(); // grab the first node.
}

////////////////////////////////////////////////////////////////////////////////

void node_iterator::copy( const node_iterator& other ) {
   this->n = other.n;
   this->ibs = other.ibs;
   this->cyclic_buffer_size = other.cyclic_buffer_size;
   this->window = other.window;
   this->outd = other.outd;
   this->block_outdegrees = other.block_outdegrees;
   this->from = other.from;
   this->curr = other.curr;
   this->owner = other.owner;
   this->end_marker = other.end_marker;
   this->avail = other.avail;
}


////////////////////////////////////////////////////////////////////////////////
void node_iterator::increment() {
   if( curr == n - 1 ) {
      // then this increment is about to place curr == n. Rather than trying to load
      // any successor list, just invalidate this iterator and make it an end marker.
      end_marker = true;
   } else {
      assert( curr < n - 1 );
      int cur_index = ++curr % cyclic_buffer_size;

      graph::internal_succ_itor_ptr itor;

      itor = owner->get_successors_internal( curr, ibs, window, outd, block_outdegrees, avail );

      if( window[cur_index].size() < (unsigned)outd[cur_index] )
         window[cur_index ].resize( outd[cur_index] );

      if(avail < cyclic_buffer_size) avail++;

      vector<vertex_label_t>::iterator i = window[cur_index].begin();

      while( itor->has_next() ) {
         *i = itor->next(); // this actually populates successor list
#ifndef CONFIG_FAST
         logs::logger( "iterators" ) << logs::LEVEL_EVERYTHING
                                     << "node iterator - succ list so far: " << *i << "\n";
#endif

         ++i;
      }
    
//      logger( "iterators" ) << LEVEL_EVERYTHING
//                            << "itor: " << itor->as_str() << "\n";
      
//      iterator_wrappers::java_to_cpp<int> w_itor( itor ), w_end;
            
//      std::copy( w_itor, w_end, window[cur_index].begin() );      
      
//       logger( "iterators" ) << LEVEL_EVERYTHING
//                             << "Done copying...\n";
   }
}

////////////////////////////////////////////////////////////////////////////////
bool node_iterator::equal(const node_iterator& rhs ) const {
   // figure out of this is supposed to be an end comparison
   if( (rhs.end_marker && n == curr) ||
       (rhs.n == rhs.curr && end_marker) ||
       (rhs.end_marker && end_marker) ||
       (n == curr && rhs.n == rhs.curr) )
      return true;

   // otherwise, do a real comparison
   
   return this->n == rhs.n &&
      ibs == rhs.ibs &&
      cyclic_buffer_size == rhs.cyclic_buffer_size &&
      window == rhs.window &&
      outd == rhs.outd &&
      block_outdegrees == rhs.block_outdegrees && 
      from == rhs.from &&
      curr == rhs.curr &&
      owner == rhs.owner &&
      avail == rhs.avail;
}

graph::succ_itor_pair node_iterator::successors() {
   assert( curr != from - 1 );

   int cur_index = curr % cyclic_buffer_size;
   
   typedef iterator_wrappers::itor_capture_wrapper<vector<vertex_label_t>::iterator, vertex_label_t> itor_base;
   
   itor_base ib( window[cur_index].begin(), 0, outd[cur_index] );
   
   // now wrap the iterator and return
   return std::make_pair( iterator_wrappers::java_to_cpp<vertex_label_t>( ib ),
                          iterator_wrappers::java_to_cpp<vertex_label_t>() );

//   return make_pair( successor_iterator_wrapper(base_pair.first),
//                     successor_iterator_wrapper(base_pair.second) );
}


const std::vector<vertex_label_t>& node_iterator::successor_vector() {
   assert( curr != from - 1 );
   
   vector<vertex_label_t>& retval = window[ curr % cyclic_buffer_size ];

   retval.resize( outd[ curr % cyclic_buffer_size ] );

   return retval;
}
   
int node_iterator::outdegree() {
   assert( curr != from - 1 );

   return outd[ curr % cyclic_buffer_size ];
}

} }
