mkdir cbuild
cd cbuild
# rm -rf *
cmake ..
make metrics_test
./test/metrics_test