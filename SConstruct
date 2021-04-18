# -*- mode:python -*-

# Base

import sys, os, re
from os.path import isdir, isfile, join as joinpath
import SCons

# The absolute path to the current directory (where this file lives).
root = Dir('.').abspath
build_root = joinpath(root, 'build')

# Make sure build_root exists (might not if this is the first build there)
if not isdir(build_root):
    os.mkdir(build_root)

###################################################
#
# Set up the default build environment.  This environment is copied
# and modified according to each selected configuration.
#
###################################################

env = Environment(ENV = os.environ,  # inherit user's environment vars
                  ROOT = root)
Export('env')
env.Decider('MD5-timestamp')
env.SConsignFile(joinpath(build_root,"sconsign"))

# Default duplicate option is to use hard links, but this messes up
# when you use emacs to edit a file in the target dir, as emacs moves
# file to file~ then copies to file, breaking the link.  Symbolic
# (soft) links work better.
env.SetOption('duplicate', 'soft-copy')

# Set up common flags
env = Environment(ENV = os.environ)

env['CC'] = 'gcc'
env['CPP'] = 'g++'
env['CXX'] = env['CPP']

env['DEBUG_FLAGS'] = ' '
env['CFLAGS'] = ' -g -O3 -DCONFIG_FAST -funroll-loops -mfpmath=sse -mavx -Wall -fmax-errors=1 -Wno-parentheses '
env['CPATH'] =  [root, joinpath(root, 'include')]
env['CPPFLAGS'] = env['CFLAGS']
env['CPPPATH'] = env['CPATH']
env['LIBS'] = []
env['LIBPATH'] = [root, joinpath(root, 'include')]
env['LINKFLAGS'] = " -g -Wl,-O3"

# build static library
sources = ['asciigraph/offline_graph.cpp', 'asciigraph/vertex.cpp', 'asciigraph/offline_edge_iterator.cpp', 'asciigraph/offline_vertex_iterator.cpp',\
    'bitstreams/input_bitstream.cpp', 'bitstreams/output_bitstream.cpp', 'csrgraph/csr_node_iterator.cpp', 'properties/properties.cpp', 'utils/fast.cpp',\
    'webgraph/compression_flags.cpp', 'webgraph/webgraph.cpp', 'webgraph/iterators/node_iterator.cpp']
env.StaticLibrary('libwebgraph', sources)

# build example apps
env.SConscript(joinpath('tests', 'SConscript'), variant_dir=joinpath(build_root, 'tests'), exports={'env' : env.Clone()})
#env.SConscript(joinpath('benchmarks', 'SConscript'), variant_dir=joinpath(build_root, 'benchmarks'), exports={'env' : env.Clone()})
env.SConscript(joinpath('utils/tools', 'SConscript'), variant_dir=joinpath(build_root, 'utils'), exports={'env' : env.Clone()})
