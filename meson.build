project('TP2', 'c')

src = [
    'src/main.c',
    'lib/ordo.h',
    'lib/ordo.c'
]

ld = ['-lrt', '-lm']

executable('ordo', src, link_args : ld)
executable('ordo_rt', src, link_args : ld, c_args : '-DRT')
executable('perturbator', 'src/perturbator.c')
