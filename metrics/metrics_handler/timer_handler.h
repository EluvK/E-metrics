#pragma once
#include "basic_handler.h"

NS_BEG2(metrics, handler)

class timer_handler : public xbasic_handler_t {
public:
    DEFAULTED_DEFAULT_CONSTRUCTOR(timer_handler);
    DELETED_COPY_DEFAULTED_MOVE_SEMANTICS(timer_handler);
    DEFAULTED_DESTRUCTOR(timer_handler);

    metrics_variant_ptr init_new_metrics(event_message const & msg) override;
    void dump_metrics_info(metrics_variant_ptr const & metrics_ptr) override;
    void process_message_event(metrics_variant_ptr & metrics_ptr, event_message const & msg) override;
};
using timer_handler_t = timer_handler;
NS_END2