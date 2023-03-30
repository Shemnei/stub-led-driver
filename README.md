[![CI](https://github.com/Shemnei/stub-led-driver/actions/workflows/ci.yaml/badge.svg?branch=main)](https://github.com/Shemnei/stub-led-driver/actions/workflows/ci.yaml)
[![Documentation](https://github.com/Shemnei/stub-led-driver/actions/workflows/doxygen.yaml/badge.svg)](https://github.com/Shemnei/stub-led-driver/actions/workflows/doxygen.yaml)

# stub-led-driver

A simple led driver for a imaginary microcontroller written in `C`.

## Tools

- [`gcc`](https://gcc.gnu.org/) (v12.2.1 20230201)
- [`make`](https://www.gnu.org/software/make/) (v4.4.1)
- [`ar`](https://en.wikipedia.org/wiki/Ar_(Unix)) (part of `binutils`; for static library builds)
- [`valgrind`](https://valgrind.org/) (v3.20.0; for memory leak checks)
- [`doxygen`](https://www.doxygen.nl/) (v1.9.6; for document generation)
- [`watchexec`](https://github.com/watchexec/watchexec) (v1.22.0; for continuously building the documentation on change)

## Build

Most build files produces by `make` will be located in the `out` directory.

### Static library

To build a library (`libled.a`) for static linking, run:

```sh
make build-lib-static
```

### Dynamic library

To build a library (`libled.so`) for dynamic linking, run:

```sh
make build-lib-dynamic
```

### Tests

Some basic "tests" are located in [`src/tests.c`](src/tests.c).
If a test fails, the expected and current result are printed to standard out.

```sh
make test
```

### Documentation

The documentation will be located under the `doc/` directory.
To build it run:

```sh
make doc
```

## Other commands

### Watch

Watches for changes and re-builds the documentation on change.

```sh
make watch
```

### Check

Does code checking without building anything.

```sh
make check
```

### Valgrind

Checks the test suit for any memory leaks.

```sh
make valgrind
```
