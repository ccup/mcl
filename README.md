## MCL : Modern C Library

Library to support modern C development.

### assumptions

- Static memory management;
- Build based on CMake.
- Linux or Mac platform.

### download and install

```sh
$ git clone git@github.com:ccup/mcl.git
$ cd mcl
$ ./ccup.sh -ubi
```

### test

```sh
$ cd mcl
$ ./ccup.sh -ubt
```

Disable multi-thread tests:
- Open CMakeLists.txt, modify `option(ENABLE_THREAD       "Build tests with thread" ON)` to `option(ENABLE_THREAD       "Build tests with thread" OFF)`;
- Clean build: `rm -rf build` or `./ccup.sh -c`;
- Rebuild and test: `./ccup.sh -ubt`

### help

```sh
./ccup.sh -h
```