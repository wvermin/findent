TESTS = test1.sh test2.sh test3.sh test4.sh test5.sh test6.sh \
	test7.sh test8.sh \
	test9.sh test10.sh test11.sh test12.sh \
	test13.sh test14.sh test15.sh test16.sh

EXTRA_DIST = test1.sh test2.sh test3.sh test4.sh test5.sh \
	test6.sh test7.sh test8.sh \
	test9.sh test10.sh test11.sh test12.sh \
	test13.sh test14.sh test15.sh test16.sh \
	test-compile.sh \
	progfixed-dos.f progfixed.f progfixed1.f progfixed.f \
	progfixedfree.f progfree-dos.f progfixed-dos.f \
	progfixedfree-dos.f progfree1.f progfree.f \
	progfixed-dos.f.try.f.ref progfixed.f.try.f.ref \
	progfixedfree-dos.f.try.f.ref progfixedfree.f.try.f.ref \
	progfree-dos.f.try.f.ref progfree.f.try.f.ref \
	progfree1.f.try.f.ref progfixed1.f.try.f.ref \
	help.ref manpage.ref \
        vim_help.ref gedit_help.ref vim_fortran.ref vim_findent.ref \
	gedit_external.ref gedit_plugin.ref gedit_plugin_py.ref \
	emacs_help.ref emacs_findent.ref \
	readme.ref \
	doit

export SRCDIR = $(top_srcdir)
if MAKEWIN
   export WINDOWS = yes
else
   export WINDOWS = no
endif
clean-local:
	rm -rf *.tmpdir
