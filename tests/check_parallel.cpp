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

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <omp.h>

#include "webgraph/webgraph.hpp"

int main( int argc, char** argv ) {
   using namespace std;
   using namespace webgraph::bv_graph;

   if( argc < 2 ) {
      cerr << "You have to provide name of graph file.\n";
      return 1;
   }
   
//#ifndef CONFIG_FAST
//   logs::register_logger( "print_webgraph", logs::LEVEL_MAX );
//#endif

   string name = argv[1];

   typedef std::shared_ptr<graph> graph_ptr;
   graph_ptr gp = graph::load( name );
   
   // get CSR
   graph::node_iterator n, n_end;
   std::tie(n, n_end) = gp->get_node_iterator( 0 );
   
   cerr << "num vertices:\n";
   cerr << gp->get_num_nodes() << endl;
   cerr << "here are vertices:\n";

   // have a CSR representation
   int* offsets = new int[gp->get_num_nodes()+1];
   int* edges = new int[gp->get_num_edges()];
   offsets[0] = 0;

   while( n != n_end ) {
       webgraph::bv_graph::graph::successor_iterator succ, succ_end;
       
       std::tie( succ, succ_end ) = n.successors();
      
       //cerr << "outdegree : " << n.outdegree() << endl;
       //cerr << "********************   " << *n << "\n";

       offsets[*n+1] = offsets[*n] + n.outdegree();

       int count = 0;
       while( succ != succ_end ) {
          //cerr << *succ << " ";
          edges[count+offsets[*n]] = *succ;
          ++succ;
          count++;
       }
       assert(count == n.outdegree());

       //cerr << endl;
       ++n;  
   }

   omp_set_num_threads(16);
   cerr << "num threads: " << omp_get_max_threads() << endl;

   const uint32_t CHUNK_SIZE = 1024;
   const uint32_t numChunks = (gp->get_num_nodes() - 1) / CHUNK_SIZE + 1;
#pragma omp parallel for schedule(dynamic, 1)
   for(uint32_t chunkId = 0; chunkId < numChunks; chunkId++) {
       uint32_t vid = chunkId * CHUNK_SIZE;

       graph::node_iterator n_tiling, n_tiling_end;
       std::tie(n_tiling, n_tiling_end) = gp->get_node_iterator( vid );

       assert(n_tiling.outdegree() == (offsets[vid+1] - offsets[vid]));
   }


   return 0;  
}
