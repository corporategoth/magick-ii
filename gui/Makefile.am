####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = mct.jar
mct_jar_SOURCES = mct.class

####### kdevelop will overwrite this part!!! (end)############

mostlyclean-compile:
	-rm -f *.class core *.core *.debug

SUFFIXES = .java .class .jar

.java.class:
	@if test -n "$(JAVAC)"; then \
		CLASSPATH="."; export CLASSPATH; \
		echo "$(JAVAC) $(JFLAGS) $(patsubst %.class,%.java,$@)"; \
		$(JAVAC) $(JFLAGS) $(patsubst %.class,%.java,$@); \
	fi

mct.jar: $(mct_jar_SOURCES)
	@if test -n "$(JAR)"; then \
		CLASSPATH="."; export CLASSPATH; \
		echo "$(JAR) cfm $@ $(patsubst %.jar,%.mf,$@) *.class *.jpg"; \
		$(JAR) cfm $@ $(patsubst %.jar,%.mf,$@) *.class *.jpg; \
	fi
