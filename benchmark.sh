#!/bin/sh

Z=10 # 終了までに10～30分程度かかります。

 K02=`expr   2000 \* $Z / 10`
 K04=`expr   4000 \* $Z / 10`
 K10=`expr  10000 \* $Z / 10`
 K20=`expr  20000 \* $Z / 10`
 K40=`expr  40000 \* $Z / 10`

 prin=benchmark

echo '----------- benchmark.txt begin ------- Z =' $Z '----------- ' >>$prin.txt
 time ./qsort      -3  10000    8   $K10     0   0   0    0          >>$prin.txt
 time ./qs9e17     -3  10000    8   $K10   140 900   0    0          >>$prin.txt
 time ./qs10a5     -3  10000    8   $K10   140 900 400    0          >>$prin.txt
 time ./qs10a5m    -3  10000    8   $K10   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort      -3  10000   20   $K10     0   0   0    0          >>$prin.txt
 time ./qs9e17     -3  10000   20   $K10   140 900   0    0          >>$prin.txt
 time ./qs10a5     -3  10000   20   $K10   140 900 400    0          >>$prin.txt
 time ./qs10a5m    -3  10000   20   $K10   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort      -3  10000   24   $K10     0   0   0    0          >>$prin.txt
 time ./qs9e17     -3  10000   24   $K10   140 900   0    0          >>$prin.txt
 time ./qs10a5     -3  10000   24   $K10   140 900 400    0          >>$prin.txt
 time ./qs10a5m    -3  10000   24   $K10   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort      -3  10000  500   $K02     0   0   0    0          >>$prin.txt
 time ./qs9e17     -3  10000  500   $K02   140 900   0    0          >>$prin.txt
 time ./qs10a5     -3  10000  500   $K02   140 900 400    0          >>$prin.txt
 time ./qs10a5m    -3  10000  500   $K02   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort      -3  10000 1000   $K02     0   0   0    0          >>$prin.txt
 time ./qs9e17     -3  10000 1000   $K02   140 900   0    0          >>$prin.txt
 time ./qs10a5     -3  10000 1000   $K02   140 900 400    0          >>$prin.txt
 time ./qs10a5m    -3  10000 1000   $K02   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort      100 10000    8   $K20     0   0   0    0          >>$prin.txt
 time ./qs9e17     100 10000    8   $K20   140 900   0    0          >>$prin.txt
 time ./qs10a5     100 10000    8   $K20   140 900 400    0          >>$prin.txt
 time ./qs10a5m    100 10000    8   $K20   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort      100 10000   20   $K20     0   0   0    0          >>$prin.txt
 time ./qs9e17     100 10000   20   $K20   140 900   0    0          >>$prin.txt
 time ./qs10a5     100 10000   20   $K20   140 900 400    0          >>$prin.txt
 time ./qs10a5m    100 10000   20   $K20   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort      100 10000   24   $K20     0   0   0    0          >>$prin.txt
 time ./qs9e17     100 10000   24   $K20   140 900   0    0          >>$prin.txt
 time ./qs10a5     100 10000   24   $K20   140 900 400    0          >>$prin.txt
 time ./qs10a5m    100 10000   24   $K20   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort      100 10000  500   $K02     0   0   0    0          >>$prin.txt
 time ./qs9e17     100 10000  500   $K02   140 900   0    0          >>$prin.txt
 time ./qs10a5     100 10000  500   $K02   140 900 400    0          >>$prin.txt
 time ./qs10a5m    100 10000  500   $K02   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort      100 10000 1000   $K02     0   0   0    0          >>$prin.txt
 time ./qs9e17     100 10000 1000   $K02   140 900   0    0          >>$prin.txt
 time ./qs10a5     100 10000 1000   $K02   140 900 400    0          >>$prin.txt
 time ./qs10a5m    100 10000 1000   $K02   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort       2  10000    8   $K40     0   0   0    0          >>$prin.txt
 time ./qs9e17      2  10000    8   $K40   140 900   0    0          >>$prin.txt
 time ./qs10a5      2  10000    8   $K40   140 900 400    0          >>$prin.txt
 time ./qs10a5m     2  10000    8   $K40   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort       2  10000   20   $K40     0   0   0    0          >>$prin.txt
 time ./qs9e17      2  10000   20   $K40   140 900   0    0          >>$prin.txt
 time ./qs10a5      2  10000   20   $K40   140 900 400    0          >>$prin.txt
 time ./qs10a5m     2  10000   20   $K40   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort       2  10000   24   $K40     0   0   0    0          >>$prin.txt
 time ./qs9e17      2  10000   24   $K40   140 900   0    0          >>$prin.txt
 time ./qs10a5      2  10000   24   $K40   140 900 400    0          >>$prin.txt
 time ./qs10a5m     2  10000   24   $K40   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort       2  10000  500   $K04     0   0   0    0          >>$prin.txt
 time ./qs9e17      2  10000  500   $K04   140 900   0    0          >>$prin.txt
 time ./qs10a5      2  10000  500   $K04   140 900 400    0          >>$prin.txt
 time ./qs10a5m     2  10000  500   $K04   140 900 400    0          >>$prin.txt
 echo ' '                                                            >>$prin.txt
 time ./qsort       2  10000 1000   $K04     0   0   0    0          >>$prin.txt
 time ./qs9e17      2  10000 1000   $K04   140 900   0    0          >>$prin.txt
 time ./qs10a5      2  10000 1000   $K04   140 900 400    0          >>$prin.txt
 time ./qs10a5m     2  10000 1000   $K04   140 900 400    0          >>$prin.txt
echo '=================  benchmark.txt end  ==================== '   >>$prin.txt
echo '各行の最後の数値がソート１回あたりの処理時間(10μ秒単位)です ' >>$prin.txt
