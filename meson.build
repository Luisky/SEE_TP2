project('TP2', 'c')

src = [
    'src/main.c',
    'lib/ordo.h',
    'lib/ordo.c'
]

ld = ['-lrt', '-lm']

lib = include_directories('lib')

executable('ordo', src, link_args : ld, include_directories : lib)
executable('ordo_rt', src, link_args : ld, include_directories : lib, c_args : '-DRT')
executable('perturbator', 'src/perturbator.c')
