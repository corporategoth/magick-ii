####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = mct.jar
mct_jar_SOURCES = mct.java
EXTRA_DIST = ChanServ.java CommServ.java Config.java ConfigView.java Files.java HexFormat.java IpAddressFormat.java JMultiLineToolTip.java MemoServ.java NickServ.java NumberRangeFormat.java OctalFormat.java OperServ.java PercentageFormat.java ServMsg.java Services.java SpaceFormat.java Startup.java TabbedPane.java TimeFormat.java ToolTips.java mct.java myTableCellEditor.java

####### kdevelop will overwrite this part!!! (end)############

mostlyclean-compile:
	-rm -f *.class core *.core *.debug

%.class: %.java
	@if test -n "$(JAVAC)"; then \
		CLASSPATH="."; export CLASSPATH; \
		echo "$(JAVAC) $(JFLAGS) $(patsubst %.class,%.java,$@)"; \
		$(JAVAC) $(JFLAGS) $(patsubst %.class,%.java,$@); \
	fi

mct.jar: mct.class $(EXTRA_DIST)
	@if test -n "$(JAVAC)"; then \
		CLASSPATH="."; export CLASSPATH; \
		echo "jar cfm $@ $(patsubst %.jar,%.mf,$@) *.class *.jpg"; \
		jar cfm $@ $(patsubst %.jar,%.mf,$@) *.class *.jpg; \
	fi
