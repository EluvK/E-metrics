#define private public
#include "metrics/metrics.h"

#include <gtest/gtest.h>

#define TEST_CASE_SIZE 7
#define SLEEP_SECOND(t) std::this_thread::sleep_for(std::chrono::seconds(t))
#define SLEEP_MILLSECOND(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
#define SLEEP_NANOSECOND(t) std::this_thread::sleep_for(std::chrono::nanoseconds(t))

class metrics_test : public testing::Test {
public:
    void SetUp() {
        METRICS_INIT();
        metrics::e_metrics::get_instance().m_metrics_hub.clear();
        metrics::e_metrics::get_instance().start();
        std::size_t test_dump_interval{2};
        METRICS_CONFIG_SET("dump_interval", test_dump_interval);
    }
    void TearDown() {
        SLEEP_SECOND(3);
        metrics::e_metrics::get_instance().stop();
    }
};
// #if 0
TEST(test_metrics, config) {
    auto & config_center = metrics::e_metrics_config::get_instance();
    // config_center.set("string",value);
    std::size_t get_dump_interval{};
    config_center.get("dump_interval", get_dump_interval);
    ASSERT_EQ(get_dump_interval, 180);
    get_dump_interval = 360;
    config_center.set("dump_interval", get_dump_interval);
    std::size_t new_dump_interval;
    METRICS_CONFIG_GET("dump_interval", new_dump_interval);
    ASSERT_EQ(new_dump_interval, 360);

    bool dump_full_unit{};
    METRICS_CONFIG_GET("dump_full_unit", dump_full_unit);
    ASSERT_EQ(dump_full_unit, true);
    dump_full_unit = false;
    METRICS_CONFIG_SET("dump_full_unit", dump_full_unit);
    bool new_dump_opt;
    METRICS_CONFIG_GET("dump_full_unit", new_dump_opt);
    ASSERT_EQ(new_dump_opt, false);
}

TEST_F(metrics_test, basic_function) {
    std::size_t test_basic_size = 1000;
    for (std::size_t index = 0; index < test_basic_size; ++index) {
        METRICS_COUNTER_INCREMENT("test_count_metrics1", static_cast<uint64_t>((100 + rand()) % 10000));
        METRICS_COUNTER_INCREMENT("test_count_metrics2", static_cast<uint64_t>((100 + rand()) % 10000));
        SLEEP_NANOSECOND(rand() % 10);
        // }
        // for (std::size_t index = 0; index < test_basic_size; ++index) {
        METRICS_TIME_RECORD("test_time_metrics");
        SLEEP_NANOSECOND(rand() % 10);
        // }
        // for (std::size_t index = 0; index < test_basic_size; ++index) {
        METRICS_FLOW_COUNT("test_flow_metrics1", static_cast<uint64_t>(100));
        METRICS_FLOW_COUNT("test_flow_metrics2", static_cast<uint64_t>((100 + rand()) % 10000));
        SLEEP_NANOSECOND(rand() % 10);
    }
}

TEST_F(metrics_test, test_timed_out) {
    std::string test_name{"TEST_TIME_OUT_DEFAULT"};
    for (auto index = 0; index < TEST_CASE_SIZE; ++index) {
        METRICS_TIME_RECORD(test_name);
        SLEEP_SECOND(index);
    }
    for (auto index = 0; index < TEST_CASE_SIZE; ++index) {
        METRICS_TIMER_START(test_name);
        SLEEP_SECOND(index);
        METRICS_TIMER_STOP(test_name);
    }

    test_name = "TEST_TIME_OUT_KEY";
    for (auto index = 0; index < TEST_CASE_SIZE; ++index) {
        METRICS_TIME_RECORD_KEY(test_name, "string:time_out_info:xxx");
        SLEEP_SECOND(index);
    }
    for (auto index = 0; index < TEST_CASE_SIZE; ++index) {
        METRICS_TIMER_START(test_name);
        SLEEP_SECOND(index);
        METRICS_TIMER_STOP_KEY(test_name, 1234567);
    }

    test_name = "TEST_TIME_OUT_KEY_TIME";
    for (auto index = 0; index < TEST_CASE_SIZE; ++index) {
        METRICS_TIME_RECORD_KEY_WITH_TIMEOUT(test_name, "string:time_out_info:xxx", 2000);
        SLEEP_MILLSECOND(index);
    }
    for (auto index = 0; index < TEST_CASE_SIZE; ++index) {
        METRICS_TIME_RECORD_KEY_WITH_TIMEOUT(test_name, 1234567, std::chrono::seconds(2));
        SLEEP_SECOND(index);
    }

    SLEEP_SECOND(2);
}

void test_metrics_time_thread_start_end(std::string metrics_name, int sleep_time_millsecond) {
    METRICS_TIMER_START(metrics_name);
    SLEEP_MILLSECOND(sleep_time_millsecond);
    METRICS_TIMER_STOP(metrics_name);
}

void test_metrics_time_thread_record(std::string metrics_name, int sleep_time_millsecond) {
    METRICS_TIME_RECORD(metrics_name);
    SLEEP_MILLSECOND(sleep_time_millsecond);
}

TEST_F(metrics_test, test_multithread) {
    std::thread t1 = std::thread(&test_metrics_time_thread_start_end, "test_start_end", 1000);
    t1.detach();
    SLEEP_MILLSECOND(300);
    std::thread t2 = std::thread(&test_metrics_time_thread_start_end, "test_start_end", 100);
    t2.detach();
    SLEEP_MILLSECOND(600);
    std::thread t3 = std::thread(&test_metrics_time_thread_record, "test_record", 1000);
    t3.detach();
    SLEEP_MILLSECOND(300);
    std::thread t4 = std::thread(&test_metrics_time_thread_record, "test_record", 100);
    t4.detach();
    SLEEP_MILLSECOND(600);
    SLEEP_SECOND(2);
    METRICS_COUNTER_INCREMENT("drives", 1);
    SLEEP_SECOND(3);
}
// #endif
#if 0
// to test metrics performance
// v0.3 result : tps over six billion message_event per second
// so the message_process can theoretically handle over three billion metrics per second.
TEST_F(metrics_test, performance) {
    std::size_t test_performance = 40000;
    std::size_t count = 20;
    for (std::size_t i = 0; i < count; ++i) {
        metrics::e_metrics::get_instance().stop();
        for (std::size_t index = 0; index < test_performance; ++index) {
            METRICS_FLOW_COUNT("test_benchmark", static_cast<uint64_t>(rand() % 10000));
        }
        METRICS_TIMER_START("TIME_benchmark_time");
        metrics::e_metrics::get_instance().start();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        METRICS_TIMER_STOP("TIME_benchmark_time");
        SLEEP_SECOND(3);
    }
}
#endif