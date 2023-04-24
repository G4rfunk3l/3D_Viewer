CC = gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -D_GNU_SOURCE
GCOV = -fprofile-arcs -ftest-coverage
OS = $(shell uname)
QMAKE = qmake
INSTALL_DIR = 3DViewer_v1.0
BUILD_DIR = 3DViewer_v1.0_build
APP_INSTALLED=$(HOME)/Desktop
ifeq ($(OS), Linux)
  APP=3DViewer
  LIBS=-lcheck -lm -lpthread -lsubunit -lrt
  OPEN=xdg-open 
  LEAKS= valgrind --leak-check=full ./gcov_report
  DESC=../README.md
else
  APP=3DViewer.app
  LIBS=-lcheck
  OPEN=open
  LEAKS=CK_FORK=no leaks --atExit -- test
  DESC=description.mht
endif

all: build

build:
	$(QMAKE) -o $(BUILD_DIR)/Makefile ./3DViewer.pro
	make -C $(BUILD_DIR)/

rebuild: clean uninstall all

install: uninstall build dvi
	cp -rf $(BUILD_DIR)/$(APP) $(APP_INSTALLED)/$(INSTALL_DIR)/
	$(OPEN) $(APP_INSTALLED)/$(INSTALL_DIR)/

uninstall:
	rm -rf $(APP_INSTALLED)/$(INSTALL_DIR)/

dvi:
	$(OPEN) $(DESC)

dist: install
	tar -cf 3DViewer_v1.0.tar $(APP_INSTALLED)/$(INSTALL_DIR)/ 3rdparty images objects description.mht *.c *.cpp *.h *.ui *.pro *.ini *.glsl *.qrc Makefile 2>/dev/null

test: clean
	$(CC) $(CFLAGS) $(GCOV) test.c 3DViewer.c $(LIBS) -o gcov_report
	./gcov_report

leaks: clean test
	$(LEAKS)

gcov_report: test
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	$(OPEN) ./report/index.html 2>/dev/null

gcov: test
	gcov 3DViewer.gcda 3DViewer.gcno
	gcovr --json 3DViewer.json
	gcovr -a 3DViewer.json --html-details coverage.html
	$(OPEN) ./coverage.html 2>/dev/null
	rm -rf *.gcno *.gcda *.json 3DViewer.c.gcov

lint:
	clang-format -i *.c *.h *.cpp
	clang-format -n *.c *.h *.cpp

check:
	cppcheck --std=c11 --enable=all --suppress=missingIncludeSystem 3DViewer.c 3DViewer.h test.c

clean:
	rm -rf ./report ./3DViewer_v1.0_build
	rm -f *.gcno *.gcda *.info gcov_report *.tar coverage.* *.pro.user*


