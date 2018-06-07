DIFF = diff
RM = rm -f
CXXFLAGS += -ggdb -Wall -I. -DCOMMAND_HANDLER_DEBUG

default: test_CommandHandler

.PHONY : CommandHandler.h
CommandHandler.h : ../../src/CommandHandler.h
	echo '' > $@
	echo '#include "arduino_compat.h"' >> $@
	echo '#include <iostream>' >> $@
	echo 'using namespace ::std;' >> $@
	cat $< >> $@

CommandHandler.cpp : ../../src/CommandHandler.cpp
	cp $< $@

CommandHandler.o : CommandHandler.cpp CommandHandler.h
	$(CXX) -DMICRONMEA_DEBUG $(CXXFLAGS) -c $< -o $@

test_CommandHandler.o : test_CommandHandler.cpp CommandHandler.h

test_CommandHandler : test_CommandHandler.o CommandHandler.o
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

.PHONY : run
run : test_CommandHandler 
	./test_CommandHandler < nmea_sentences.txt
# Or run with electric fence to catch segmentation faults
# 	LD_PRELOAD=libefence.so ./test_CommandHandler

# Regression testing. Run as "make regressiontest", should display
# "TEST PASSED" if everything ok.
.PHONY : regressiontest
regressiontest :
	$(MAKE) realclean
	$(MAKE) test_CommandHandler
	./test_CommandHandler < nmea_sentences.txt > test_CommandHandler.regressiontest.tmp
	echo TEST RUN
	$(DIFF) -s -u test_CommandHandler.regressiontest test_CommandHandler.regressiontest.tmp
	-$(RM) test_CommandHandler.regressiontest.tmp
	@echo
	@echo TEST PASSED

.PHONY : clean
clean : 
	-$(RM) *.o CommandHandler.h CommandHandler.cpp test_CommandHandler.regressiontest.tmp

.PHONY : realclean
realclean : clean
	-$(RM) -f test_CommandHandler 
