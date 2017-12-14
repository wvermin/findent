#include "vim_plugin.h"
#include <iostream>

// generate files for usage with vim

void do_vim_help()
{
#include "vim_help.inc"
}
// output fortran.vim
void do_vim_fortran()
{
#include "vim_fortran.inc"
}

// output findent.vim
void do_vim_findent()
{
#include "vim_findent.inc"
}
