# -*- coding: utf-8 -*-
#  Findent plugin
#  This file is part of gedit
#
# -copyright-
#-# Copyright: 2015,2016,2017,2018,2019,2020,2021 Willem Vermin wvermin@gmail.com
#-# 
#-# License: BSD-3-Clause
#-#  Redistribution and use in source and binary forms, with or without
#-#  modification, are permitted provided that the following conditions
#-#  are met:
#-#  1. Redistributions of source code must retain the above copyright
#-#     notice, this list of conditions and the following disclaimer.
#-#  2. Redistributions in binary form must reproduce the above copyright
#-#     notice, this list of conditions and the following disclaimer in the
#-#     documentation and/or other materials provided with the distribution.
#-#  3. Neither the name of the copyright holder nor the names of its
#-#     contributors may be used to endorse or promote products derived
#-#     from this software without specific prior written permission.
#-#   
#-#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#-#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#-#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#-#  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE HOLDERS OR
#-#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#-#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#-#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#-#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#-#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#-#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#-#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

from gi.repository import GObject, Gio, Gtk, Gedit
import gettext
import os.path
from subprocess import Popen,PIPE

try:
    gettext.bindtextdomain(GETTEXT_PACKAGE, GP_LOCALEDIR)
    _ = lambda s: gettext.dgettext(GETTEXT_PACKAGE, s)
except:
    _ = lambda s: s


class FindentAppActivatable(GObject.Object, Gedit.AppActivatable):
    app = GObject.Property(type=Gedit.App)

    def __init__(self):
        GObject.Object.__init__(self)
        global shortcut
        shortcut = "<Ctrl><Alt>i"

    def do_activate(self):
        self.app.add_accelerator(shortcut, "win.findent", None)

    def do_deactivate(self):
        self.app.remove_accelerator("win.findent", None)


class FindentWindowActivatable(GObject.Object, Gedit.WindowActivatable):

    window = GObject.Property(type=Gedit.Window)

    def __init__(self):
        GObject.Object.__init__(self)
        self.bottom_bar = Gtk.Box()

    def do_activate(self):
        action = Gio.SimpleAction(name="findent")
        action.connect('activate', lambda a, p: self.do_findent())
        self.window.add_action(action)
        self._insert_bottom_panel()

    def do_deactivate(self):
        self.window.remove_action("findent")
        self._remove_bottom_panel()

    def do_update_state(self):
        view = self.window.get_active_view()
        enable = view is not None and view.get_editable()
        self.window.lookup_action("findent").set_enabled(enable)
        return

    def do_findent(self):
        view = self.window.get_active_view()
        if view and hasattr(view, "findent_view_activatable"):
            view.findent_view_activatable.do_findent()

    def _insert_bottom_panel(self):
        # Add elements to panel.
        self.findent_label = Gtk.Label()
        self.findent_textbuffer = Gtk.TextBuffer()
        global mesg
        mesg = self.findent_textbuffer
        self.findent_textview = Gtk.TextView()
        self.findent_textview.set_buffer(self.findent_textbuffer)
        self.bottom_bar.add(self.findent_textview)
        # Get bottom bar (A Gtk.Stack) and add our bar.        
        panel = self.window.get_bottom_panel()
        panel.add_titled(self.bottom_bar, 'findent_panel', "Findent output")
        # Make sure everything shows up.
        panel.show()
        self.bottom_bar.show_all()
        panel.set_visible_child(self.bottom_bar)

    def _remove_bottom_panel(self):
        panel = self.window.get_bottom_panel()
        panel.remove(self.bottom_bar)

    def settext(self,text):
        self.findent_textbuffer.set_text(text)

class FindentViewActivatable(GObject.Object, Gedit.ViewActivatable):

    view = GObject.Property(type=Gedit.View)

    def __init__(self):
        GObject.Object.__init__(self)
        self.default_flags = "-Ia -i3"
        self.executable = "findent"
        self.userflags = os.getenv("GEDIT_FINDENT_FLAGS")
        if self.userflags == None:
            self.userflags = ""

    def do_activate(self):
        self.view.findent_view_activatable = self
        self.view.connect('populate-popup', self.populate_popup)
        mesg.set_text("Findent ready for use."+ \
            " Shortcut: "+shortcut+ \
            " and in popup menu." + \
            " Default flags: '" + self.default_flags + "'" + \
            "\nflags from environment variable GEDIT_FINDENT_FLAGS: '" + 
            self.userflags+"'")

    def do_deactivate(self):
        delattr(self.view, "findent_view_activatable")

    def populate_popup(self, view, popup):
        if not isinstance(popup, Gtk.MenuShell):
            return

        item = Gtk.SeparatorMenuItem()
        item.show()
        popup.append(item)

        item = Gtk.MenuItem.new_with_mnemonic(_('_Findent'))
        item.set_sensitive(self.view.get_editable())
        item.show()
        item.connect('activate', lambda i: self.do_findent())
        popup.append(item)


    def do_findent(self):
        doc = self.view.get_buffer()
        if doc is None:
            return

        ctype = doc.get_content_type()
        if ctype != "text/x-fortran":
            mesg.set_text("Content type is '"+ctype+"'" + \
                    ". Findent handles only 'text/x-fortran'")
            return

        # run a simple test to see if findent is doing what we expect

        srcin = "program test\ncontinue\nend program test"
        expected = "program test\n   continue\nend program test\n"

        try:
            with Popen([self.executable,"-i3"], stdout=PIPE, stdin=PIPE) as proc:
                srcout = proc.communicate(str.encode(srcin))[0].decode()
        except OSError:
            mesg.set_text("Cannot find findent." + \
                    "\nTake care that the location of findent is in PATH" +\
                    "\nUsing '"+self.executable+"'")
            return

        if srcout != expected:
            mesg.set_text("Findent is not functioning as expected." +\
                    "\nTake care that the location of findent is in PATH" +\
                    "\nUsing '"+self.executable+"'")
            return

        xy = self.view.window_to_buffer_coords(Gtk.TextWindowType.TEXT,0,0)
        z = self.view.get_line_at_y(xy[1])[0]
        line  = z.get_line()
        cline = doc.get_iter_at_mark(doc.get_insert()).get_line()

        srcin  = doc.get_text(doc.get_start_iter(),doc.get_end_iter(),True)

        name   = doc.get_short_name_for_display()
        suffix = os.path.splitext(name)[1].lower()

        if suffix in {".f",".for",".fpp",".ftn",".fortran",".f77"}:
            ff = "-ifixed"
        elif suffix in {".f90",".f95",".f03",".f08"}:
            ff = "-ifree"
        else:
            ff = "-iauto"

        flags = self.default_flags.split() + [ff] + self.userflags.split()

        try:
            with Popen([self.executable]+flags, stdout=PIPE, stdin=PIPE) as proc:
                srcout = proc.communicate(str.encode(srcin))[0].decode()
        except OSError:
            mesg.set_text("Cannot find findent." + \
                    "\nTake care that the location of findent is in PATH" +\
                    "\nUsing '"+self.executable+"'")
            return

        orig_lines = doc.get_line_count()
        doc.set_text(srcout)
        while Gtk.events_pending():
            Gtk.main_iteration_do(False)

        hop = doc.get_iter_at_line(line)
        self.view.scroll_to_iter(hop,0,True,0,0)
        doc.goto_line(cline)
        lines = doc.get_line_count()
        s = ""
        for i in flags:
            s = s+" "+i

        mesg.set_text("flags used: "+s+" \n"+ \
                str(lines)+" lines indented")
        if lines == orig_lines+1:
            mesg.insert_at_cursor(", empty line added")

        if lines != orig_lines and lines != orig_lines+1:
            mesg.insert_at_cursor("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")
            mesg.insert_at_cursor("\nXX MALFUNCTION DETECTED, UNDO TWICE! XX")
            mesg.insert_at_cursor("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")

# ex:ts=4:et:
