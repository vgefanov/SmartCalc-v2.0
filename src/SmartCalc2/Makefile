.PHONY: all clean tests gcov_report dvi install uninstall dist
CC = gcc
CFLAGS =-Wall -Wextra -Werror -lstdc++ -std=c++17
CFLAGS_TEST = -lgtest
CFLAGS_GCOV =--coverage -fkeep-inline-functions
CPPCHECK_FLAGS =--enable=all --suppress=missingIncludeSystem
OUTFILE = model
OUTFILE_TEST = test
SOURCES = $(OUTFILE).cpp
SOURCES_TEST = $(OUTFILE_TEST).cpp
CHECK_FILES = main.cpp controller.cpp controller.h model.cpp model.h view.cpp view.h test.cpp

all: prepare
	make -f qMakefile

install: all
	@test -d bin || mkdir -p bin
	rm -f -r bin/SmartCalc2.app
	/usr/local/bin/qmake -install qinstall SmartCalc2.app bin/SmartCalc2.app
	-strip bin/SmartCalc2.app/Contents/MacOS/SmartCalc2

uninstall: prepare
	-rm -f -r bin/SmartCalc2.app
	-rmdir bin/

clean: prepare
	@-rm *.gcov *.gcda *.gcno *.info *.cfg *.xml *.gz 2>/dev/null
	@-rm test 2>/dev/null
	@-rm -rf lcov_report bin report 2>/dev/null
	@make -f qMakefile clean
	@-rm -rf SmartCalc2.app
	@-rm qMakefile

dvi:
	# brew install doxygen
	cd doc && doxygen doxygen.conf
	open doc/html/index.html

dist:
	tar -czpf SmartCalc2.tar.gz *.cpp *.h *.ui Makefile SmartCalc2.pro SmartCalc2.pro.user

tests:
	$(CC) $(CFLAGS) $(CFLAGS_TEST) $(SOURCES) $(SOURCES_TEST) -o $(OUTFILE_TEST)
	./$(OUTFILE_TEST)

gcov_report:
	@$(CC) $(CFLAGS) $(CFLAGS_TEST) $(CFLAGS_GCOV) $(SOURCES) $(SOURCES_TEST) -o $(OUTFILE_TEST)
	./$(OUTFILE_TEST)
	gcov $(OUTFILE)
	gcov -f $(OUTFILE).gcda
	lcov --no-external --capture --directory . --output-file main_coverage.info
	genhtml main_coverage.info --output-directory lcov_report
	open lcov_report/index.html

prepare:
	qmake -o qMakefile

checkall: style check leaks

style:
	cp ../../materials/linters/.clang-format ./
	clang-format -i main.cpp controller.cpp controller.h model.cpp model.h view.cpp view.h test.cpp 
	clang-format -n main.cpp controller.cpp controller.h model.cpp model.h view.cpp view.h test.cpp

check:
	cppcheck --std=c++17 --language=c++ $(CPPCHECK_FLAGS) $(CHECK_FILES)

leaks: tests
	CK_FORK=no leaks --atExit -- ./$(OUTFILE_TEST)

brew:
    # cd ~/goinfre
    # git clone https://github.com/Homebrew/brew homebrew
    # eval "$(~/goinfre/homebrew/bin/brew shellenv)"
    # brew install lcov
