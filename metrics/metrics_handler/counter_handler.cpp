#include "counter_handler.h"

NS_BEG2(metrics, handler)

metrics_variant_ptr counter_handler_t::init_new_metrics(event_message const & msg) {
    return metrics_variant_ptr(std::make_shared<metrics_counter_unit>(msg.metrics_name, msg.metrics_value.GetConstRef<int64_t>()));
}

void counter_handler_t::dump_metrics_info(metrics_variant_ptr const & metrics_ptr) {
    bool dump_json_format{false};
    METRICS_CONFIG_GET("dump_json_format", dump_json_format);
    auto const & ptr = metrics_ptr.GetConstRef<metrics_counter_unit_ptr>();
    if (dump_json_format) {
        json res, cont;
        res["category"] = get_category(ptr->name);
        res["tag"] = get_tag(ptr->name);
        res["type"] = "counter";
        cont["value"] = ptr->inner_val;
        res["content"] = cont;
        std::stringstream ss;
        ss << res;
        dump(ss.str(), ptr->inner_val != ptr->last_dump_val);
    } else {
        std::stringstream ss;
        ss.setf(std::ios::left, std::ios::adjustfield);
        ss.fill(' ');
        ss << std::setw(calc_dump_width(ptr->name.size())) << ptr->name << ": [counter:" << ptr->inner_val << "]";
        dump(ss.str(), ptr->inner_val != ptr->last_dump_val);
    }
    ptr->last_dump_val = ptr->inner_val;
}

void counter_handler_t::process_message_event(metrics_variant_ptr & metrics_ptr, event_message const & msg) {
    auto ptr = metrics_ptr.GetRef<metrics_counter_unit_ptr>();
    assert(ptr);
    switch (msg.minor_id) {
    case e_metrics_minor_id::increase:
        ptr->inner_val += msg.metrics_value.GetConstRef<int64_t>();
        break;
    case e_metrics_minor_id::decrease:
        ptr->inner_val -= msg.metrics_value.GetConstRef<int64_t>();
        break;
    case e_metrics_minor_id::set:
        ptr->inner_val = msg.metrics_value.GetConstRef<int64_t>();
        break;
    default:
        xwarn("METRICS_BUG: name: %s %zu - %zu, type: %zu", msg.metrics_name.c_str(), msg.major_id, msg.minor_id, msg.appendant_infomation.GetType());
        // assert(0);
        break;
    }
    return;
}

NS_END2