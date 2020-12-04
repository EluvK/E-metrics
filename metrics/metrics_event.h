#pragma once

#include "metrics_unit.h"
NS_BEG1(metrics)

enum class e_metrics_major_id : size_t {
    count = 1,
    timer = 2,
    flow = 3,
};

enum class e_metrics_minor_id : size_t {
    increase = 1,
    decrease,
    set,
    timestart,
    timeend,
    flow_count,
    flow_timestamp,
};

struct event_message {
    e_metrics_major_id major_id;
    e_metrics_minor_id minor_id;
    std::string metrics_name;
    Variant<int64_t, time_point> metrics_value;
    microseconds timed_out;

    metrics_appendant_info appendant_infomation;

    event_message(){};
    event_message(e_metrics_major_id _major_id, e_metrics_minor_id _minor_id, std::string _name, Variant<int64_t, time_point> _val) {
        major_id = _major_id;
        minor_id = _minor_id;
        metrics_name = _name;
        metrics_value = _val;
    };
    event_message(e_metrics_major_id _major_id,
                  e_metrics_minor_id _minor_id,
                  std::string _name,
                  Variant<int64_t, time_point> _val,
                  metrics_appendant_info _info,
                  microseconds _time_out = DEFAULT_TIMED_OUT) {
        major_id = _major_id;
        minor_id = _minor_id;
        metrics_name = _name;
        metrics_value = _val;
        appendant_infomation = _info;
        timed_out = _time_out;
    };
};

NS_END1