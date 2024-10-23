#!/bin/sh

rm -rf build
rm -rf /home/renaud/applications/mine/renaudg/rolisteam/src/libraries/diceparser/src/tests/fuzzer/sync_dir/fuzzer01
mkdir build
cd build

CC=afl-gcc CXX=afl-g++ cmake ../ -DCMAKE_PREFIX_PATH:PATH=/home/renaud/applications/other/.qt/6.7.2/gcc_64 -DQT_QMAKE_EXECUTABLE:FILEPATH=/home/renaud/applications/other/.qt/6.7.2/gcc_64/bin/qmake -DSTATIC_BUILD=ON && make

fuzzRoot=/home/renaud/applications/mine/renaudg/rolisteam/src/libraries/diceparser/src/tests/fuzzer
fuzzTestCase=/home/renaud/applications/mine/renaudg/rolisteam/src/libraries/diceparser/build/src/tests/fuzzer/fuzzTestCase
#./fuzzTestCase ../testcase_dir/gooddata.txt
# afl-fuzz -m 2G -i ../testcase_dir -o ../findings_dir ./fuzzTestCase @@
afl-fuzz -m 2G -i $fuzzRoot/testcase_dir -o $fuzzRoot/sync_dir -M fuzzer01 $fuzzTestCase @@ &
screen -S fuzz1  -d -m touch afl-fuzz  -m 2G -i $fuzzRoot/testcase_dir -o $fuzzRoot/sync_dir -S fuzzer02 $fuzzTestCase @@ &
screen -S fuzz2 -d -m touch afl-fuzz -m 2G -i $fuzzRoot/testcase_dir -o $fuzzRoot/sync_dir -S fuzzer03 $fuzzTestCase @@ &
screen -S fuzz3 -d -m touch afl-fuzz -m 2G -i $fuzzRoot/testcase_dir -o $fuzzRoot/sync_dir -S fuzzer04 $fuzzTestCase @@ &
screen -S fuzz4 -d -m touch afl-fuzz -m 2G -i $fuzzRoot/testcase_dir -o $fuzzRoot/sync_dir -S fuzzer05 $fuzzTestCase @@ &
screen -S fuzz5 -d -m touch afl-fuzz -m 2G -i $fuzzRoot/testcase_dir -o $fuzzRoot/sync_dir -S fuzzer06 $fuzzTestCase @@



# as root

# echo core >/proc/sys/kernel/core_pattern

# cd /sys/devices/system/cpu
# echo performance | tee cpu*/cpufreq/scaling_governor
# echo  ondemand | tee cpu*/cpufreq/scaling_governor
