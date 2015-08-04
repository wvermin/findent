if BUILD_JFINDENT
JAR   = jar
CHMOD = chmod
CP    = cp
jfindent jfindent.jar:  Jfindent.java
	$(JAVAC)        Jfindent.java
	$(JAR)   cfe $@ Jfindent Jfindent*.class
	$(CHMOD) +x  $@
	$(CP)        $@ jfindent.jar
bin_SCRIPTS = jfindent jfindent.jar

man1_MANS = jfindent.1

install-data-hook:
	gzip -f $(mandir)/man1/$(man1_MANS)
else
all:
endif
CLEANFILES  = jfindent jfindent.jar *.class
