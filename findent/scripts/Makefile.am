bin_SCRIPTS = wfindent
man1_MANS   = wfindent.1
CLEANFILES  = wfindent

wfindent: wfindent.tmpl
	$(SED) 's|findent-location|"$(bindir)/findent"|' $< > $@
