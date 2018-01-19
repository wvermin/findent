#include "emacs_plugin.h"
#include <iostream>

// generate files for usage with emacs
void do_emacs_help()
{
#include "emacs_help.inc"
}

// output findent.el: to be used in emacs init file
void do_emacs_findent()
{
#include "emacs_findent.inc"
}
