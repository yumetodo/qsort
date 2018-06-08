# qsort

@t-kawa 氏作成のqsortをベンチマークする

元のコードは

[qsort(qs9 qs10)](http://ww51.tiki.ne.jp/~srr-cake/qsort/qs10/index.html)

## Todo

- unixの`time`コマンドに依存しない計測方法

## How to build and run benchmark

Require CMake to build.

### Linux

```bash
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=../bin ..
$ make install
$ cd ../bin
$ ./benchmark.sh
```

### Windows

#### Visual Studio

Some unix-like shell(ex. git-bash, msys2 bash, etc...) is required to benchmark.

1. Start `cmake-gui.exe`
2. Specify source code path and build dir path
3. Configure
4. Set `CMAKE_INSTALL_PREFIX` to `<some directory path>`
5. Configure
6. Build
7. Open Project
8. In Solution Explorer, right-click `INSTALL` and select build
9. start some unix-like shell
10. `cd <some directory path>`
11. `./benchmark.sh`

### msys2

```bash
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=../bin -G "MSYS Makefiles" ..
$ make install
$ cd ../bin
$ ./benchmark.sh
```

## 原作者情報

河村　知行  
〒745-0845 山口県周南市河東町3-2 （株）曽呂利  
t-kawa@crux.ocn.ne.jp
