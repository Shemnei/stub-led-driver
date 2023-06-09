.PHONY: default all build build-lib build-lib-dynamic build-lib-static build-tests
.PHONY: clean run lint valgrind doc set-target

# Use expanded (:=) instead of recursive (=) variable definitions to only have to
# "build" them once.
BUILD_DIR := out
CC        := gcc
CFLAGS    = -Werror -Wall -Wextra -std=c11 -pedantic -D'$(TARGET_MC)'

default: all

all: build

build: build-lib build-tests

build-lib: build-lib-dynamic build-lib-static

build-lib-dynamic: ${BUILD_DIR}/libled.so
build-lib-static: ${BUILD_DIR}/libled.a

build-tests: ${BUILD_DIR}/tests_s ${BUILD_DIR}/tests_d

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

clean:
	rm -r ${BUILD_DIR}

test: ${BUILD_DIR}/tests_s
	./${BUILD_DIR}/tests_s

check: set-target
	$(CC) $(CFLAGS) -fsyntax-only src/*.c

valgrind: ${BUILD_DIR}/tests_s
	valgrind \
		--tool=memcheck \
		--leak-check=yes \
		--show-reachable=yes \
		--num-callers=20 \
		--track-fds=yes \
		${BUILD_DIR}/tests_s

doc:
	doxygen .doxygen

watch:
	watchexec -w src -w .doxygen make doc

set-target:
	# Only redefine variable if not already defined
	$(if $(TARGET_MC), , $(eval TARGET_MC=$(shell ./get-target-board-controller.sh)))

# BUILD Shared Library

# PIC = Position idependent code
${BUILD_DIR}/registers.pic.o: set-target ${BUILD_DIR} src/registers.c
	$(CC) $(CFLAGS) -fPIC -c -o ${BUILD_DIR}/registers.pic.o src/registers.c

${BUILD_DIR}/led.pic.o: set-target ${BUILD_DIR} src/led.c
	$(CC) $(CFLAGS) -fPIC -c -o ${BUILD_DIR}/led.pic.o src/led.c

${BUILD_DIR}/msg.pic.o: set-target ${BUILD_DIR} src/msg.c
	$(CC) $(CFLAGS) -fPIC -c -o ${BUILD_DIR}/msg.pic.o src/msg.c

${BUILD_DIR}/libled.so: set-target ${BUILD_DIR} ${BUILD_DIR}/registers.pic.o ${BUILD_DIR}/led.pic.o ${BUILD_DIR}/msg.pic.o
	$(CC) $(CFLAGS) -shared -o ${BUILD_DIR}/libled.so ${BUILD_DIR}/registers.pic.o ${BUILD_DIR}/led.pic.o ${BUILD_DIR}/msg.pic.o

# BUILD Static Library
${BUILD_DIR}/registers.stat.o: set-target ${BUILD_DIR} src/registers.c
	$(CC) $(CFLAGS) -c -o ${BUILD_DIR}/registers.stat.o src/registers.c

${BUILD_DIR}/led.stat.o: set-target ${BUILD_DIR} src/led.c
	$(CC) $(CFLAGS) -c -o ${BUILD_DIR}/led.stat.o src/led.c

${BUILD_DIR}/msg.stat.o: set-target ${BUILD_DIR} src/msg.c
	$(CC) $(CFLAGS) -c -o ${BUILD_DIR}/msg.stat.o src/msg.c

${BUILD_DIR}/libled.a: set-target ${BUILD_DIR} ${BUILD_DIR}/registers.stat.o ${BUILD_DIR}/led.stat.o ${BUILD_DIR}/msg.stat.o
	ar -rcs ${BUILD_DIR}/libled.a ${BUILD_DIR}/registers.stat.o ${BUILD_DIR}/led.stat.o ${BUILD_DIR}/msg.stat.o

# BUILD tests with static lib
${BUILD_DIR}/tests_d: set-target ${BUILD_DIR} ${BUILD_DIR}/libled.so src/tests.c
	$(CC) $(CFLAGS) -Wl,-rpath,. -o ${BUILD_DIR}/tests_d src/tests.c -L${BUILD_DIR} -lled

${BUILD_DIR}/tests_s: set-target ${BUILD_DIR} ${BUILD_DIR}/libled.a src/tests.c
	$(CC) $(CFLAGS) -o ${BUILD_DIR}/tests_s src/tests.c -static -L${BUILD_DIR} -lled
