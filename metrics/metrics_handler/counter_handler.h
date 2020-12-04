#pragma once
#include "basic_handler.h"

NS_BEG2(metrics, handler)

class counter_handler : public xbasic_handler_t {
public:
    XDECLARE_DEFAULTED_DEFAULT_CONSTRUCTOR(counter_handler);
    XDECLARE_DELETED_COPY_DEFAULTED_MOVE_SEMANTICS(counter_handler);
    XDECLARE_DEFAULTED_DESTRUCTOR(counter_handler);

    metrics_variant_ptr init_new_metrics(event_message const & msg) override;
    void dump_metrics_info(metrics_variant_ptr const & metrics_ptr) override;
    void process_message_event(metrics_variant_ptr & metrics_ptr, event_message const & msg) override;
};
using counter_handler_t = counter_handler;
NS_END2