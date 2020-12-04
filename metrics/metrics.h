#pragma once
#include "metrics_handler/counter_handler.h"
#include "metrics_handler/flow_handler.h"
#include "metrics_handler/timer_handler.h"
#include "metrics_handler/metrics_packet_info.h"
#include "base/xrunnable.h"
#include "base/xthreadsafe_queue.hpp"
#include "metrics_event.h"
#include "metrics_unit.h"

#include <chrono>
#include <map>
#include <string>
#include <thread>
NS_BEG1(metrics)

class e_metrics : public top::xbasic_runnable_t<e_metrics> {
public:
    XDECLARE_DELETED_COPY_AND_MOVE_SEMANTICS(e_metrics);
    XDECLARE_DEFAULTED_OVERRIDE_DESTRUCTOR(e_metrics);

    static e_metrics & get_instance() {
        static e_metrics instance;
        return instance;
    }

    void start() override;
    void stop() override;

private:
    XDECLARE_DEFAULTED_DEFAULT_CONSTRUCTOR(e_metrics);
    void run_process();
    void process_message_queue();
    void update_dump();

public:
    void timer_start(std::string metrics_name, time_point value);
    void timer_end(std::string metrics_name, time_point value, metrics_appendant_info info = 0, microseconds timed_out = DEFAULT_TIMED_OUT);
    void counter_increase(std::string metrics_name, int64_t value);
    void counter_decrease(std::string metrics_name, int64_t value);
    void counter_set(std::string metrics_name, int64_t value);
    void flow_count(std::string metrics_name, int64_t value, time_point timestamp);

private:
    std::thread m_process_thread;
    std::size_t m_dump_interval;
    std::chrono::microseconds m_queue_procss_behind_sleep_time;
    handler::counter_handler_t m_counter_handler;
    handler::timer_handler_t m_timer_handler;
    handler::flow_handler_t m_flow_handler;
    constexpr static std::size_t message_queue_size{100000};
    top::threading::xthreadsafe_queue<event_message, std::vector<event_message>> m_message_queue{message_queue_size};
    std::map<std::string, metrics_variant_ptr> m_metrics_hub;  // {metrics_name, metrics_vaiant_ptr}
};

class metrics_time_auto {
public:
    metrics_time_auto(std::string metrics_name, metrics_appendant_info key = std::string{"NOT SET appendant_info"}, microseconds timed_out = DEFAULT_TIMED_OUT)
      : m_metrics_name{metrics_name}, m_key{key}, m_timed_out{timed_out} {
        char c[15] = {0};
        snprintf(c, 14, "%p", this);
        m_metrics_name = m_metrics_name + "&" + c;
        e_metrics::get_instance().timer_start(m_metrics_name, std::chrono::system_clock::now());
    }
    ~metrics_time_auto() { e_metrics::get_instance().timer_end(m_metrics_name, std::chrono::system_clock::now(), m_key, m_timed_out); }

    std::string m_metrics_name;
    metrics_appendant_info m_key;
    microseconds m_timed_out;
};

// #ifdef ENABLE_METRICS
#define METRICS_INIT()                                                                                                                                                            \
    {                                                                                                                                                                              \
        auto & ins = metrics::e_metrics::get_instance();                                                                                                                      \
        ins.start();                                                                                                                                                               \
    }

#define SSTR(x) static_cast<std::ostringstream &>((std::ostringstream()  << x)).str()
#define ADD_THREAD_HASH(metrics_name) SSTR(metrics_name) + "&0x" + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()))
#define STR_CONCAT_IMPL(a, b) a##b
#define STR_CONCAT(str_a, str_b) STR_CONCAT_IMPL(str_a, str_b)

#define MICROSECOND(timeout)                                                                                                                                                       \
    std::chrono::microseconds { timeout }

#define METRICS_TIME_RECORD(metrics_name)                                                                                                                                         \
    metrics::metrics_time_auto STR_CONCAT(metrics_time_auto, __LINE__) { metrics_name }
#define METRICS_TIME_RECORD_KEY(metrics_name, key)                                                                                                                                \
    metrics::metrics_time_auto STR_CONCAT(metrics_time_auto, __LINE__) { metrics_name, key }
#define METRICS_TIME_RECORD_KEY_WITH_TIMEOUT(metrics_name, key, timeout)                                                                                                             \
    metrics::metrics_time_auto STR_CONCAT(metrics_time_auto, __LINE__) { metrics_name, key, MICROSECOND(timeout) }

#define METRICS_TIMER_START(metrics_name) metrics::e_metrics::get_instance().timer_start(ADD_THREAD_HASH(metrics_name), std::chrono::system_clock::now());

#define METRICS_TIMER_STOP(metrics_name) metrics::e_metrics::get_instance().timer_end(ADD_THREAD_HASH(metrics_name), std::chrono::system_clock::now());

#define METRICS_TIMER_STOP_KEY(metrics_name, key) metrics::e_metrics::get_instance().timer_end(ADD_THREAD_HASH(metrics_name), std::chrono::system_clock::now(), key);

#define METRICS_TIMER_STOP_KEY_WITH_TIMEOUT(metrics_name, key, timeout)                                                                                                              \
    metrics::e_metrics::get_instance().timer_end(ADD_THREAD_HASH(metrics_name), std::chrono::system_clock::now(), key, MICROSECOND(timeout));

#define METRICS_COUNTER_INCREMENT(metrics_name, value) metrics::e_metrics::get_instance().counter_increase(metrics_name, value)
#define METRICS_COUNTER_DECREMENT(metrics_name, value) metrics::e_metrics::get_instance().counter_decrease(metrics_name, value)
#define METRICS_COUNTER_SET(metrics_name, value) metrics::e_metrics::get_instance().counter_set(metrics_name, value)

#define METRICS_FLOW_COUNT(metrics_name, value) metrics::e_metrics::get_instance().flow_count(metrics_name, value, std::chrono::system_clock::now());

#define METRICS_PACKET_INFO(metrics_name, ...)                                                                                                                                    \
    metrics::handler::metrics_pack_unit STR_CONCAT(packet_info_auto_, __LINE__){metrics_name};                                                                                                       \
    metrics::handler::metrics_packet_impl(STR_CONCAT(packet_info_auto_, __LINE__), __VA_ARGS__);

#define METRICS_XBASE_DATA_CATEGORY_NEW(key) METRICS_COUNTER_INCREMENT("dataobject_cur_xbase_type" + std::to_string(key), 1);
#define METRICS_XBASE_DATA_CATEGORY_DELETE(key) METRICS_COUNTER_INCREMENT("dataobject_cur_xbase_type" + std::to_string(key), -1);

// #else
// #define METRICS_INIT()
// #define METRICS_TIME_RECORD(metrics_name)
// #define METRICS_TIME_RECORD_KEY(metrics_name, key)
// #define METRICS_TIME_RECORD_KEY_WITH_TIMEOUT(metrics_name, key, timeout)
// #define METRICS_TIMER_START(metrics_name)
// #define METRICS_TIMER_STOP(metrics_name)
// #define METRICS_TIMER_STOP_KEY(metrics_name, key)
// #define METRICS_TIMER_STOP_KEY_WITH_TIMEOUT(metrics_name, key, timeout)
// #define METRICS_COUNTER_INCREMENT(metrics_name, value)
// #define METRICS_COUNTER_DECREMENT(metrics_name, value)
// #define METRICS_COUNTER_SET(metrics_name, value)
// #define METRICS_FLOW_COUNT(metrics_name, value)
// #define METRICS_PACKET_INFO(metrics_name, ...)
// #define METRICS_XBASE_DATA_CATEGORY_NEW(key)
// #define METRICS_XBASE_DATA_CATEGORY_DELETE(key)
// #endif

NS_END1
