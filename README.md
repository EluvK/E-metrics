# E-metrics
my own metrics module written in C++11

## Requirment
- GCC 4.8.5 or higher
- CMake 3.8 or higher
- googletest for unit test
- [nlohmann/json](https://github.com/nlohmann/json)

## TEST

## How to USE it.

#### include headfile
`#include "metrics/metrics.h"`
#### INIT_METRICS
init metrics module at the beginning of the program using `METRICS_INIT();`

#### USE METRICS
You can take a look at file `metrics.h`, it's all defined between line 87 and line 114.
##### TIMER
- To calculate time the program used between two lines.
- Use `METRICS_TIMER_START` to start and `METRICS_TIMER_STOP` to stop.
- Or simply use `METRICS_TIME_RECORD` to metrics time automatically by constructor and destructor.
##### COUNTER
- Todo 
##### FLOW

##### PACKET_INFO

## References
- [Variant](https://github.com/kmalloc/CompilerFront/blob/master/Basic/Variant.h) 
- [Handstats](https://github.com/yandex/handystats) 

## TODO
- [ ] Finish THIS Readme.md
- [ ] Add examples
- [ ] implementation of my own Variant
- [ ] implementation of mpsc queue and make it configuable.
- [ ] JSON format listen port for website

## License
MIT License

Copyright (c) 2020 Eucalypt