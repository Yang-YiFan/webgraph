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
   
   graph::node_iterator n, n_end;
   std::tie(n, n_end) = gp->get_node_iterator( 0 );
   
   cerr << "num vertices:\n";
   cerr << gp->get_num_nodes() << endl;
   cerr << "here are vertices:\n";

   while( n != n_end ) {
       webgraph::bv_graph::graph::successor_iterator succ, succ_end;
       
       std::tie( succ, succ_end ) = n.successors();

       graph::node_iterator n_tiling, n_tiling_end;
       std::tie(n_tiling, n_tiling_end) = gp->get_node_iterator( *n );
       webgraph::bv_graph::graph::successor_iterator succ_tiling, succ_tiling_end;
       std::tie( succ_tiling, succ_tiling_end ) = n.successors();
      
       //cerr << "********************   " << *n << "\n";
       //cerr << "outdegree : " << n.outdegree()  << endl;
       assert(n.outdegree() == n_tiling.outdegree());
       assert(*n == *n_tiling);

       while( succ != succ_end ) {
          //cerr << *succ << " ";
          assert(*succ == *succ_tiling);
          ++succ;
          ++succ_tiling;
       }
      
       ++n;  
   }

   return 0;  
}
