####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = mct.jar
mct_jar_SOURCES = mct.java
EXTRA_DIST = ChanServ.java CommServ.java Config.java ConfigView.java Files.java HexFormat.java IpAddressFormat.java JMultiLineToolTip.java MemoServ.java NickServ.java NumberRangeFormat.java OctalFormat.java OperServ.java PercentageFormat.java ServMsg.java Services.java SpaceFormat.java Startup.java TabbedPane.java TimeFormat.java ToolTips.java mct.java myTableCellEditor.java

####### kdevelop will overwrite this part!!! (end)############

mostlyclean-compile:
	-rm -f *.class core *.core *.debug

%.class: %.java
	if test -n "`which javac 2>/dev/null`"; then \
		JVER=`java -version 2>&1 | grep "^java version" | cut -f3 -d" " | sed -e "s/\"//g" -e "s/\.//g"`; \
		if test 0$$JVER -lt 140; then \
			echo "Java v1.4.0 or above is required to compile the Magick Configuration Tool."; \
		else \
			javac -g -O $(patsubst %.class,%.java,$@); \
		fi; \
	fi

mct.jar: mct.class
	if test -n "`which jar 2>/dev/null`" -a -f mct.class; then \
		jar cfm $@ mct.mf *.class *.jpg; \
	fi
