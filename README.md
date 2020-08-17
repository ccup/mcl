## MCL : Modern C Library

MCL is a pure header only library to support modern C development.

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

- install [cctest](https://github.com/ccup/cctest);

```sh
$ git clone git@github.com:ccup/cctest.git
$ cd cctest
$ mkdir build
$ cd build
$ cmake .. 
$ make
$ sudo make install
```

- compile and test by `build.sh`;

```sh
$ cd mcl
$ ./build.sh
```