
project(
    'graphs',
    'cpp',
    version : '1.0.0',
    default_options : [
        'buildtype=debug',
        'optimization=0',
        'cpp_std=c++20',
    ],
    meson_version: '>= 0.60.0',
)

ncurses_dep = dependency('ncurses', required: host_machine.system() != 'windows')

executable('graphs',
           'main.cpp',
           'matrixgraph.cpp',
           'listgraph.cpp',
           'igraphiter.cpp',
           'igraphloader.cpp',
           'dijkstra.cpp',
           'brutForce.cpp',
           'welshPowell.cpp',
           'dsatur.cpp',
           'noorder.cpp',
            dependencies: [ncurses_dep]
)

