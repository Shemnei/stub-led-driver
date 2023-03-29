# stub-led-driver

A simple led driver for a imaginary microcontroller written in `C`.

## Tools

- [`gcc`](https://gcc.gnu.org/) (v12.2.1 20230201)
- [`make`](https://www.gnu.org/software/make/) (v4.4.1)
- [`ar`](https://en.wikipedia.org/wiki/Ar_(Unix)) (part of `binutils`; for static library builds)
- [`valgrind`](https://valgrind.org/) (v3.20.0; for memory leak checks)
- [`doxygen`]() (v; for document generation)

## Build

Most build files produces by `make` will be located in the `out` directory.

### Static library

To build a library (`libled.a`) for static linking, run:

```
make build-lib-static
```

### Dynamic library

To build a library (`libled.so`) for dynamic linking, run:

```
make build-lib-dynamic
```

### Tests

Some basic "tests" are located in [`src/main.c`](src/main.c).
If a test fails, the expected and current result are printed to standard out.

```
make test
```

### Documentation

The documentation will be located under the `doc/` directory.
To build it run:

```sh
make doc
```

## Other commands

### Check

Does code checking without building anything.

```
make check
```

### Valgrind

Checks the test suit for any memory leaks.

```
make valgrind
```
