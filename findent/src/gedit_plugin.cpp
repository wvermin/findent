#include "gedit_plugin.h"
#include <iostream>

// generate files for usage with gedit
void do_gedit_help()
{
#include "gedit_help.inc"
}

// output findent-gedit: to be used as external command in gedit
void do_gedit_external()
{
#include "gedit_external.inc"
}

// output findent.py
void do_gedit_plugin_py()
{
#include "gedit_plugin_py.inc"
}
   
// output findent.plugin
void do_gedit_plugin()
{
#include "gedit_plugin.inc"
}
