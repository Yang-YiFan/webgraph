/*
 * Run it to see usage. Defaults should work for most parameters.
 * 
 * Sample usage: given a graph called some-graph.txt, the following will
 * compress it to three files - compressed_graph.graph, 
 * compressed_graph.offsets, and compressed_graph.properties.
 *
 *      ./compress_webgraph --source=some_graph --dest=compressed_graph
 */

#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <tuple>
#include <assert.h>

#include "webgraph/webgraph.hpp"

/** Reads an immutable graph (ascii graph) and stores it as a {@link BVGraph}.
 *  Instead of using ascii graph as frontend, the input ascii graph is first
 *  converted into CSR format, and then passed to webgraph to test the CSR frontend. 
 */
int main( int argc, char** argv ) {
   namespace po = boost::program_options;
   namespace bvg = webgraph::bv_graph;
   namespace ag = webgraph::ascii_graph;
   namespace csrg = webgraph::csrgraph;
   using namespace std;

   std::string src, dest;
   
   int window_size = -1, 
      max_ref_count = -1, 
      min_interval_length = -1, 
      zeta_k = 5, 
      flags = 0,
      quantum = 10000;

   bool offline = false, write_offsets = false;

   ostringstream help_message_oss;

   /// TODO - change this so that source and dest are positional arguments
   help_message_oss 
      << "Usage: " << argv[0] << "[OPTIONS] --source=SOURCE --dest=[DEST]\n"
      << "Compresses a graph differentially. The SOURCE and DEST parameters are\n"
      << "basenames from which suitable file names will be stemmed.\n"
      << "\n"
      << "If DEST is omitted, no recompression is performed. This is useful\n"
      << "in conjunction with --offsets.\n"
      << "\n"
      << "Optional arguments";

   po::options_description desc( help_message_oss.str() );
   desc.add_options()
      ("help,h", "Print help message")
      
      ("comp,c", 
       po::value<int>(), 
       "A compression flag (may be specified several times)")
      
      ("window-size,w", 
       po::value<int>(&window_size)->default_value( bvg::graph::DEFAULT_WINDOW_SIZE ),
       "Reference window size")
      
      ("max-ref-count,m", 
       po::value<int>(&max_ref_count)->default_value(bvg::graph::DEFAULT_MAX_REF_COUNT),
       "Maximum number of backward references")
      
      ("graph-class,g", 
       po::value<string>(),
       "Set graph class")

      ("min-interval-length", 
       po::value<int>(&min_interval_length)->
                          default_value(bvg::graph::DEFAULT_MIN_INTERVAL_LENGTH),
       "Set minimum interval length")
      
      ("zeta-k,k", 
       po::value<int>(&zeta_k)->default_value(bvg::graph::DEFAULT_ZETA_K),
       "the k parameter for zeta-k codes")
      
      ("offline,o", "Use the offline load method to reduce memory consumption")
      
      ("offsets,O", "Generate offsets for the source graph")
      
      ("quantum,q",
       po::value<int>(&quantum)->default_value( quantum ),
       "Set value for progress meter quantum")

      ("source,s",
       po::value<string>(&src),
       "Set source graph file")

      ("dest,d",
       po::value<string>(&dest),
       "Set destination file")
      ;

   po::variables_map vm;
   po::store( po::parse_command_line( argc, argv, desc), vm );
   po::notify( vm );

   if( vm.count( "help" ) ) {
      cout << desc << endl;
      return 1;
   }

   if( vm.count("comp") ) {
      cerr << "Not implemented yet." << endl;
      //         flags |= BVGraph.class.getField( g.getOptarg() ).getInt( BVGraph.class );
      return 1;
   }

   if( vm.count("graph-class") ) {
      if( vm["graph-class"].as<string>() != "AsciiGraph" ) {
         cerr << "The only allowable parameter for graph-class is AsciiGraph right now.\n";

         return 1;
      }
   }

   if( vm.count( "offline" ) ) {
      offline = true;
   }

   if( vm.count( "offsets" ) ) {
      write_offsets = true;
   }
   
   if( !vm.count( "source" ) || !vm.count( "dest") ) {
      cerr << "For now you must specify either a source or a dest." << "\n";
      
      cerr << desc;  
      
      return 0;
   }

   ////////////////////////////////////////////////////////////////////////////////
   // All options are now collected. Start doing the actual loading.
   ////////////////////////////////////////////////////////////////////////////////

   //timing::time_t start = timimg::timer();

   ag::offline_graph graph = ag::offline_graph::load( src );
   ag::offline_graph::node_iterator ag_n, ag_n_end;
   std::tie(ag_n, ag_n_end) = graph.get_node_iterator(0);

   // now we convert the ascii graph to csr graph
   int* offsets = new int[graph.get_num_nodes()+1];
   int* edges = new int[graph.get_num_edges()];

   offsets[0] = 0;
   while(ag_n != ag_n_end) {
      std::vector<int> ag_successors;
      ag_successors = ag_n.successor_vector();

      offsets[*ag_n+1] = offsets[*ag_n] + ag_n.outdegree();
      for(uint32_t i=0; i<ag_successors.size(); i++) {
         edges[i+offsets[*ag_n]] = ag_successors.at(i);
      }
      ++ag_n;
   }
   assert(offsets[graph.get_num_nodes()] == graph.get_num_edges());

   csrg::csr_graph csr_g(graph.get_num_nodes(), graph.get_num_edges(), offsets, edges);
   csrg::csr_graph::node_iterator csrg_n, csrg_n_end;
   std::tie(csrg_n, csrg_n_end) = csr_g.get_node_iterator(0);

   std::tie(ag_n, ag_n_end) = graph.get_node_iterator(0);
   // check csr graph is identical to ascii graph
   assert(csr_g.get_num_nodes() == graph.get_num_nodes());
   assert(csr_g.get_num_edges() == graph.get_num_edges());

   while( csrg_n != csrg_n_end ) {
      std::vector<int> csrg_successors;
      csrg_successors = csrg_n.successor_vector();

      std::vector<int> ag_successors;
      ag_successors = ag_n.successor_vector();

      assert(*csrg_n == *ag_n);
      assert(csrg_n.outdegree() == ag_n.outdegree());
      assert(csrg_successors.size() == ag_successors.size());
      assert((uint32_t)csrg_n.outdegree() == csrg_successors.size());

      //std::cerr<<"vertex: "<<*csrg_n<<std::endl;
      for(uint32_t i=0; i<csrg_successors.size(); i++) {
         assert(csrg_successors.at(i) == ag_successors.at(i));
         //std::cerr<<csrg_successors.at(i)<<", ";
      }
      //std::cerr<<std::endl;

      ++csrg_n;
      ++ag_n;
   }
   assert(ag_n == ag_n_end);
   std::cerr<<"CSR conversion OK!"<<std::endl;

   // pass the csr graph to compression framework

   ostream* log = &cerr;

   if( dest != "" ) {
      cerr << "About to call store CSR graph...\n";
      bvg::graph::store_offline_graph<csrg::csr_graph>( csr_g, dest, window_size, max_ref_count,
                                                        min_interval_length,
                                                        zeta_k, flags, log );
   }
   else {
      if ( write_offsets ) {
         cerr << "Not implemented yet\n";
         return 1;
//       if ( graph instanceof BVGraph ) {
//          final OutputBitStream offsets = new OutputBitStream( graph.basename() + ".offsets", 64 * 1024 );
//          pm.expectedUpdates( graph.numNodes() );
//          pm.start( "Writing offsets..." );
//          ((BVGraph)graph).writeOffsets( offsets, pm );
//          offsets.close();
//          pm.done();
//       }
//       else System.err.println( "The sources graph is not a BVGraphn" );
      }
   }

   //timimg::time_t stop = timing::timer();

   //cout << timing::calculate_elapsed( start, stop );

   return 0;
}
