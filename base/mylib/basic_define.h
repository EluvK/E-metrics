#pragma once
#include <mutex>
#include <type_traits>

// CLASS helper

#define DEFAULTED_DEFAULT_CONSTRUCTOR(CLASS_NAME) CLASS_NAME() = default

#define DELETED_DEFAULT_CONSTRUCTOR(CLASS_NAME) CLASS_NAME() = delete

#define DEFAULTED_COPY_SEMANTICS(CLASS_NAME)                                                                                                                                       \
    CLASS_NAME(CLASS_NAME const &) = default;                                                                                                                                      \
    CLASS_NAME & operator=(CLASS_NAME const &) = default

#define DELETED_COPY_SEMANTICS(CLASS_NAME)                                                                                                                                         \
    CLASS_NAME(CLASS_NAME const &) = delete;                                                                                                                                       \
    CLASS_NAME & operator=(CLASS_NAME const &) = delete

#define DEFAULTED_MOVE_SEMANTICS(CLASS_NAME)                                                                                                                                       \
    CLASS_NAME(CLASS_NAME &&) = default;                                                                                                                                           \
    CLASS_NAME & operator=(CLASS_NAME &&) = default

#define DELETED_MOVE_SEMANTICS(CLASS_NAME)                                                                                                                                         \
    CLASS_NAME(CLASS_NAME &&) = delete;                                                                                                                                            \
    CLASS_NAME & operator=(CLASS_NAME &&) = delete

#define DEFAULTED_COPY_AND_MOVE_SEMANTICS(CLASS_NAME)                                                                                                                              \
    DEFAULTED_COPY_SEMANTICS(CLASS_NAME);                                                                                                                                          \
    DEFAULTED_MOVE_SEMANTICS(CLASS_NAME)

#define DELETED_COPY_AND_MOVE_SEMANTICS(CLASS_NAME)                                                                                                                                \
    DELETED_COPY_SEMANTICS(CLASS_NAME);                                                                                                                                            \
    DELETED_MOVE_SEMANTICS(CLASS_NAME)

#define DELETED_COPY_DEFAULTED_MOVE_SEMANTICS(CLASS_NAME)                                                                                                                          \
    DELETED_COPY_SEMANTICS(CLASS_NAME);                                                                                                                                            \
    DEFAULTED_MOVE_SEMANTICS(CLASS_NAME)

#define DELETED_COPY_AND_MOVE_SEMANTICS(CLASS_NAME)                                                                                                                                \
    DELETED_COPY_SEMANTICS(CLASS_NAME);                                                                                                                                            \
    DELETED_MOVE_SEMANTICS(CLASS_NAME)

#define DEFAULTED_VIRTULA_DESTRUCTOR(CLASS_NAME) virtual ~CLASS_NAME() = default

#define DELETED_DESTRUCTOR(CLASS_NAME) ~CLASS_NAME() = delete

#define DEFAULTED_OVERRIDE_DESTRUCTOR(CLASS_NAME) ~CLASS_NAME() override = default

#define DEFAULTED_DESTRUCTOR(CLASS_NAME) ~CLASS_NAME() = default

// lock:

template <typename MutexT>
struct lock_guard_helper {
    std::unique_lock<MutexT> lock;
    bool b;
};

extern template struct lock_guard_helper<std::mutex>;

#define LOCK_GUARD(MUTEX)                                                                                                                                                          \
    for (::lock_guard_helper<typename ::std::remove_cv<typename ::std::remove_reference<decltype(MUTEX)>::type>::type> MUTEX##_lock{                                               \
             ::std::unique_lock<typename ::std::remove_cv<typename ::std::remove_reference<decltype(MUTEX)>::type>::type>{MUTEX}, true};                                           \
         MUTEX##_lock.b;                                                                                                                                                           \
         MUTEX##_lock.b = false)

#define LOCK(MUTEX)                                                                                                                                                                \
    std::lock_guard<typename std::remove_cv<typename std::remove_reference<decltype(MUTEX)>::type>::type> MUTEX##_lock { MUTEX }

#define SHARED_LOCK(MUTEX)                                                                                                                                                         \
    std::shared_lock<typename std::remove_cv<typename std::remove_reference<decltype(MUTEX)>::type>::type> MUTEX##_lock { MUTEX }

// namespace:

#define NS_BEG() namespace {
#define NS_END }

#define NS_BEG1(NS) namespace NS {
#define NS_END1 }

#if defined XCXX17_OR_ABOVE

#    define NS_BEG2(NS1, NS2) namespace NS1::NS2 {
#    define NS_END2 }

#    define NS_BEG3(NS1, NS2, NS3) namespace NS1::NS2::NS3 {
#    define NS_END3 }

#    define NS_BEG4(NS1, NS2, NS3, NS4) namespace NS1::NS2::NS3::NS4 {
#    define NS_END4 }

#    define NS_BEG5(NS1, NS2, NS3, NS4, NS5) namespace NS1::NS2::NS3::NS4::NS5 {
#    define NS_END5 }

#else

#    define NS_BEG2(NS1, NS2) NS_BEG1(NS1) NS_BEG1(NS2)
#    define NS_END2 NS_END1 NS_END1

#    define NS_BEG3(NS1, NS2, NS3) NS_BEG2(NS1, NS2) NS_BEG1(NS3)
#    define NS_END3 NS_END1 NS_END2

#    define NS_BEG4(NS1, NS2, NS3, NS4) NS_BEG3(NS1, NS2, NS3) NS_BEG1(NS4)
#    define NS_END4 NS_END1 NS_END3

#    define NS_BEG5(NS1, NS2, NS3, NS4, NS5) NS_BEG4(NS1, NS2, NS3, NS4) NS_BEG1(NS5)
#    define NS_END5 NS_END1 NS_END4

#endif

// log:
#define xdbg(...)                                                                                                                                                                  \
    printf("DEBUG(%d):", __FILE__, __LINE__, __VA_ARGS__);                                                                                                                         \
    printf("\n");

#define xinfo(...)                                                                                                                                                                 \
    printf("INFO line:%d :", __LINE__);                                                                                                                                            \
    printf(__VA_ARGS__);                                                                                                                                                           \
    printf("\n");

#define xwarn(...)                                                                                                                                                                 \
    printf("WARN (%s:%d): ", __FILE__, __LINE__);                                                                                                                                  \
    printf(__VA_ARGS__);                                                                                                                                                           \
    printf("\n");

#define xerror(...)                                                                                                                                                                \
    {                                                                                                                                                                              \
        fprintf(stderr, "Error(%s:%d): ", __FILE__, __LINE__);                                                                                                                     \
        fprintf(stderr, __VA_ARGS__);                                                                                                                                              \
        printf("\n");                                                                                                                                                              \
        throw(__VA_ARGS__);                                                                                                                                                        \
    }
