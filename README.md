## MCL : Modern C Library

MCL is a pure header only library to support modern C development.

### assumptions

- Static memory management;
- Build based on CMake.
- Linux or Mac platform.

### download and install

```sh
$ git clone git@github.com:ccup/mcl.git
$ cd mcl
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

### test

```sh
$ cd mcl
$ ./ccup.sh -ubt
```
