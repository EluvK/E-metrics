#pragma once

#include "mylib/basic_define.h"

#include <atomic>

NS_BEG1(mylib)

template <typename T>
class runnable_base {
public:
    DEFAULTED_DEFAULT_CONSTRUCTOR(runnable_base);
    DELETED_COPY_DEFAULTED_MOVE_SEMANTICS(runnable_base);
    DEFAULTED_VIRTULA_DESTRUCTOR(runnable_base);

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual bool running() const noexcept = 0;

    virtual void running(bool const value) noexcept = 0;
};

template <typename T>
using runnable_base_t = runnable_base<T>;

template <typename T>
class runnable : public runnable_base_t<T> {
    std::atomic<bool> m_running{false};

public:
    runnable(runnable const &) = default;
    runnable & operator=(runnable const &) = default;
    runnable(runnable &&) = default;
    runnable & operator=(runnable &&) = default;
    ~runnable() = default;

#if !defined XCXX14_OR_ABOVE
    runnable() noexcept {}
#else
    runnable() = default;
#endif

    bool running() const noexcept override { return m_running.load(std::memory_order_acquire); }

    void running(bool const value) noexcept override { m_running.store(value, std::memory_order_release); }
};

template <typename T>
using runnable_t = runnable<T>;

NS_END1