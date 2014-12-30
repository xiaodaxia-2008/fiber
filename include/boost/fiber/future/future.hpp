
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_FUTURE_HPP
#define BOOST_FIBERS_FUTURE_HPP

#include <algorithm> // std::move()
#include <chrono>
#include <exception>

#include <boost/config.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/future/detail/shared_state.hpp>
#include <boost/fiber/future/future_status.hpp>

namespace boost {
namespace fibers {

template< typename R >
class packaged_task;

template< typename R >
class promise;

template< typename R >
class shared_future;

template< typename R >
class future {
private:
    typedef typename detail::shared_state< R >::ptr_t   ptr_t;

    friend class shared_future< R >;

    ptr_t           state_;

public:
    future() noexcept :
        state_() {
        //TODO: constructs a future with no shared state
        //      after construction, valid() == false
    }


    explicit future( ptr_t const& p) noexcept :
        state_( p) {
    }
    ~future() noexcept {
        //TODO: abandon ownership if any
    }

    future( future const&) = delete;
    future & operator=( future const&) = delete;

    future( future< R > && other) noexcept :
        state_( std::move( other.state_) ) {
        //TODO: constructs a future with the shared state of other using move semantics
        //      after construction, other.valid() == false
    }

    future & operator=( future< R > && other) noexcept {
        //TODO: releases any shared state and move-assigns the contents of other to *this
        //      after the assignment, other.valid() == false and this->valid() will yield
        //      the same value as other.valid() before the assignment
        if ( this != & other) {
            state_ = std::move( other.state_);
        }
        return * this;
    }

    bool valid() const noexcept {
        //TODO: checks if the future refers to a shared state
        //      this is the case only for futures returned by
        //      promise::get_future(), packaged_task::get_future()
        //      or async() until the first time get()or share() is called
        return nullptr != state_.get();
    }

    shared_future< R > share();

    R get() {
        //TODO: the get method waits until the future has a valid result and
        //      (depending on which template is used) retrieves it
        //      it effectively calls wait() in order to wait for the result
        //      the value stored in the shared state
        //      if it satisfies the requirements of MoveAssignable, the value is moved,
        //      otherwise it is copied
        //      valid() == false after a call to this method.
        //      detect the case when valid == false before the call and throw a
        //      future_error with an error condition of future_errc::no_state
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        ptr_t tmp;
        tmp.swap( state_);
        return tmp->get();
    }

    std::exception_ptr get_exception_ptr() {
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->get_exception_ptr();
    }

    void wait() const {
        //TODO: blocks until the result becomes available
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        state_->wait();
    }

    template< class Rep, class Period >
    future_status wait_for( std::chrono::duration< Rep, Period > const& timeout_duration) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_for( timeout_duration);
    }

    future_status wait_until( std::chrono::high_resolution_clock::time_point const& timeout_time) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_until( timeout_time);
    }

    template< typename ClockType >
    future_status wait_until( typename ClockType::time_point const& timeout_time_) const {
        std::chrono::high_resolution_clock::time_point timeout_time(
            std::chrono::high_resolution_clock::now() + ( timeout_time_ - ClockType::now() ) );
        return wait_until( timeout_time);
    }
};

template< typename R >
class future< R & > {
private:
    typedef typename detail::shared_state< R & >::ptr_t   ptr_t;

    friend class shared_future< R & >;

    ptr_t           state_;

public:
    future() noexcept :
        state_() {
        //TODO: constructs a future with no shared state
        //      after construction, valid() == false
    }

    explicit future( ptr_t const& p) noexcept :
        state_( p) {
    }

    ~future() noexcept {
        //TODO: abandon ownership if any
    }

    future( future const&) = delete;
    future & operator=( future const&) = delete;

    future( future< R & > && other) noexcept :
        state_( std::move( other.state_) ) {
        //TODO: constructs a future with the shared state of other using move semantics
        //      after construction, other.valid() == false
    }

    future & operator=( future< R & > && other) noexcept {
        //TODO: releases any shared state and move-assigns the contents of other to *this
        //      after the assignment, other.valid() == false and this->valid() will yield
        //      the same value as other.valid() before the assignment
        if ( this != & other) {
            state_ = std::move( other.state_);
        }
        return * this;
    }

    bool valid() const noexcept {
        //TODO: checks if the future refers to a shared state
        //      this is the case only for futures returned by
        //      promise::get_future(), packaged_task::get_future()
        //      or async() until the first time get()or share() is called
        return nullptr != state_.get();
    }

    shared_future< R & > share();

    R & get() {
        //TODO: the get method waits until the future has a valid result and
        //      (depending on which template is used) retrieves it
        //      it effectively calls wait() in order to wait for the result
        //      the value stored in the shared state
        //      if it satisfies the requirements of MoveAssignable, the value is moved,
        //      otherwise it is copied
        //      valid() == false after a call to this method.
        //      detect the case when valid == false before the call and throw a
        //      future_error with an error condition of future_errc::no_state
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        ptr_t tmp;
        tmp.swap( state_);
        return tmp->get();
    }

    std::exception_ptr get_exception_ptr() {
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->get_exception_ptr();
    }

    void wait() const {
        //TODO: blocks until the result becomes available
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        state_->wait();
    }

    template< class Rep, class Period >
    future_status wait_for( std::chrono::duration< Rep, Period > const& timeout_duration) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_for( timeout_duration);
    }

    future_status wait_until( std::chrono::high_resolution_clock::time_point const& timeout_time) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_until( timeout_time);
    }

    template< typename ClockType >
    future_status wait_until( typename ClockType::time_point const& timeout_time_) const {
        std::chrono::high_resolution_clock::time_point timeout_time(
            std::chrono::high_resolution_clock::now() + ( timeout_time_ - ClockType::now() ) );
        return wait_until( timeout_time);
    }
};

template<>
class future< void > {
private:
    typedef detail::shared_state< void >::ptr_t   ptr_t;

    friend class shared_future< void >;

    ptr_t           state_;

public:
    future() noexcept :
        state_() {
        //TODO: constructs a future with no shared state
        //      after construction, valid() == false
    }

    explicit future( ptr_t const& p) noexcept :
        state_( p) {
    }

    ~future() noexcept {
        //TODO: abandon ownership if any
    }

    future( future const&) = delete;
    future & operator=( future const&) = delete;

    inline
    future( future< void > && other) noexcept :
        state_( std::move( other.state_) ) {
        //TODO: constructs a future with the shared state of other using move semantics
        //      after construction, other.valid() == false
    }

    inline
    future & operator=( future< void > && other) noexcept {
        //TODO: releases any shared state and move-assigns the contents of other to *this
        //      after the assignment, other.valid() == false and this->valid() will yield
        //      the same value as other.valid() before the assignment
        if ( this != & other) {
            state_ = std::move( other.state_);
        }
        return * this;
    }

    inline
    bool valid() const noexcept {
        //TODO: checks if the future refers to a shared state
        //      this is the case only for futures returned by
        //      promise::get_future(), packaged_task::get_future()
        //      or async() until the first time get()or share() is called
        return nullptr != state_.get();
    }

    shared_future< void > share();

    inline
    void get() {
        //TODO: the get method waits until the future has a valid result and
        //      (depending on which template is used) retrieves it
        //      it effectively calls wait() in order to wait for the result
        //      the value stored in the shared state
        //      if it satisfies the requirements of MoveAssignable, the value is moved,
        //      otherwise it is copied
        //      valid() == false after a call to this method.
        //      detect the case when valid == false before the call and throw a
        //      future_error with an error condition of future_errc::no_state
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        ptr_t tmp;
        tmp.swap( state_);
        tmp->get();
    }

    inline
    std::exception_ptr get_exception_ptr() {
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->get_exception_ptr();
    }

    inline
    void wait() const {
        //TODO: blocks until the result becomes available
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        state_->wait();
    }

    template< class Rep, class Period >
    future_status wait_for( std::chrono::duration< Rep, Period > const& timeout_duration) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_for( timeout_duration);
    }

    inline
    future_status wait_until( std::chrono::high_resolution_clock::time_point const& timeout_time) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_until( timeout_time);
    }

    template< typename ClockType >
    future_status wait_until( typename ClockType::time_point const& timeout_time_) const {
        std::chrono::high_resolution_clock::time_point timeout_time(
            std::chrono::high_resolution_clock::now() + ( timeout_time_ - ClockType::now() ) );
        return wait_until( timeout_time);
    }
};


template< typename R >
class shared_future {
private:
    typedef typename detail::shared_state< R >::ptr_t   ptr_t;

    friend class future< R >;

    ptr_t           state_;

    explicit shared_future( ptr_t const& p) noexcept :
        state_( p) {
    }

public:
    shared_future() noexcept :
        state_() {
        //TODO: constructs a shared_future with no shared state
        //      after construction, valid() == false
    }

    ~shared_future() noexcept {
        //TODO: if *this is the last object referring to the shared state,
        //      destroys the shared state otherwise does nothing
    }

    shared_future( shared_future const& other) :
        state_( other.state_) {
        //TODO: constructs a shared future that refers to the same shared state,
        //      as other, if there's any
    }

    shared_future( shared_future && other) noexcept :
        state_( std::move( other.state_) ) {
        //TODO: constructs a shared_future with the shared state of other using move semantics
        //      after construction, other.valid() == false
    }

    shared_future( future< R > && other) noexcept :
        state_( std::move( other.state_) ) {
        //TODO: constructs a shared_future with the shared state of other using move semantics
        //      after construction, other.valid() == false
    }

    shared_future & operator=( shared_future const& other) noexcept {
        //TODO:
        if ( this != & other) {
            state_ = other.state_;
        }
        return * this;
    }

    shared_future & operator=( shared_future && other) noexcept {
        //TODO: releases any shared state and move-assigns the contents of other to *this
        //      after the assignment, other.valid() == false and this->valid() will yield
        //      the same value as other.valid() before the assignment
        if ( this != & other) {
            state_= std::move( other.state_);
        }
        return * this;
    }

    shared_future & operator=( future< R > && other) noexcept {
        //TODO:
        state_ = std::move( other.state_);
        return * this;
    }

    bool valid() const noexcept {
        //TODO: checks if the shared_future refers to a shared state
        //      this is the case only for shared_futures returned by
        //      promise::get_shared_future(), packaged_task::get_shared_future()
        //      or async() until the first time get()or share() is called
        return nullptr != state_.get();
    }

    R const& get() const {
        //TODO: the get method waits until the shared_future has a valid result and
        //      (depending on which template is used) retrieves it
        //      it effectively calls wait() in order to wait for the result
        //      the value stored in the shared state
        //      if it satisfies the requirements of MoveAssignable, the value is moved,
        //      otherwise it is copied
        //      valid() == false after a call to this method.
        //      detect the case when valid == false before the call and throw a
        //      future_error with an error condition of future_errc::no_state
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->get();
    }

    std::exception_ptr get_exception_ptr() {
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->get_exception_ptr();
    }

    void wait() const {
        //TODO: blocks until the result becomes available
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        state_->wait();
    }

    template< class Rep, class Period >
    future_status wait_for( std::chrono::duration< Rep, Period > const& timeout_duration) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_for( timeout_duration);
    }

    future_status wait_until( std::chrono::high_resolution_clock::time_point const& timeout_time) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_until( timeout_time);
    }

    template< typename ClockType >
    future_status wait_until( typename ClockType::time_point const& timeout_time_) const {
        std::chrono::high_resolution_clock::time_point timeout_time(
            std::chrono::high_resolution_clock::now() + ( timeout_time_ - ClockType::now() ) );
        return wait_until( timeout_time);
    }
};

template< typename R >
class shared_future< R & > {
private:
    typedef typename detail::shared_state< R & >::ptr_t   ptr_t;

    friend class future< R & >;

    ptr_t           state_;

    explicit shared_future( ptr_t const& p) noexcept :
        state_( p) {
    }

public:
    shared_future() noexcept :
        state_() {
        //TODO: constructs a shared_future with no shared state
        //      after construction, valid() == false
    }

    ~shared_future() noexcept {
        //TODO: if *this is the last object referring to the shared state,
        //      destroys the shared state otherwise does nothing
    }

    shared_future( shared_future const& other) :
        state_( other.state_) {
        //TODO: constructs a shared future that refers to the same shared state,
        //      as other, if there's any
    }

    shared_future( shared_future && other) noexcept :
        state_( std::move( other.state_) ) {
        //TODO: constructs a shared_future with the shared state of other using move semantics
        //      after construction, other.valid() == false
    }

    shared_future( future< R & > && other) noexcept :
        state_( std::move( other.state_) ) {
        //TODO: constructs a shared_future with the shared state of other using move semantics
        //      after construction, other.valid() == false
    }

    shared_future & operator=( shared_future const& other) noexcept {
        //TODO:
        if ( this != & other) {
            state_ = other.state_;
        }
        return * this;
    }

    shared_future & operator=( shared_future && other) noexcept {
        //TODO: releases any shared state and move-assigns the contents of other to *this
        //      after the assignment, other.valid() == false and this->valid() will yield
        //      the same value as other.valid() before the assignment
        if ( this != & other) {
            state_ = std::move( other.state_);
        }
        return * this;
    }

    shared_future & operator=( future< R & > && other) noexcept {
        //TODO:
        state_ = std::move( other.state_);
        return * this;
    }

    bool valid() const noexcept {
        //TODO: checks if the shared_future refers to a shared state
        //      this is the case only for shared_futures returned by
        //      promise::get_shared_future(), packaged_task::get_shared_future()
        //      or async() until the first time get()or share() is called
        return nullptr != state_.get();
    }

    R & get() const {
        //TODO: the get method waits until the shared_future has a valid result and
        //      (depending on which template is used) retrieves it
        //      it effectively calls wait() in order to wait for the result
        //      the value stored in the shared state
        //      if it satisfies the requirements of MoveAssignable, the value is moved,
        //      otherwise it is copied
        //      valid() == false after a call to this method.
        //      detect the case when valid == false before the call and throw a
        //      future_error with an error condition of future_errc::no_state
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->get();
    }

    std::exception_ptr get_exception_ptr() {
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->get_exception_ptr();
    }

    void wait() const {
        //TODO: blocks until the result becomes available
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        state_->wait();
    }

    template< class Rep, class Period >
    future_status wait_for( std::chrono::duration< Rep, Period > const& timeout_duration) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_for( timeout_duration);
    }

    future_status wait_until( std::chrono::high_resolution_clock::time_point const& timeout_time) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_until( timeout_time);
    }

    template< typename ClockType >
    future_status wait_until( typename ClockType::time_point const& timeout_time_) const {
        std::chrono::high_resolution_clock::time_point timeout_time(
            std::chrono::high_resolution_clock::now() + ( timeout_time_ - ClockType::now() ) );
        return wait_until( timeout_time);
    }
};

template<>
class shared_future< void > {
private:
    typedef detail::shared_state< void >::ptr_t   ptr_t;

    friend class future< void >;

    ptr_t           state_;

    shared_future( ptr_t const& p) noexcept :
        state_( p) {
    }

public:
    shared_future() noexcept :
        state_() {
        //TODO: constructs a shared_future with no shared state
        //      after construction, valid() == false
    }

    ~shared_future() noexcept {
        //TODO: if *this is the last object referring to the shared state,
        //      destroys the shared state otherwise does nothing
    }

    inline
    shared_future( shared_future const& other) :
        state_( other.state_) {
        //TODO: constructs a shared future that refers to the same shared state,
        //      as other, if there's any
    }

    inline
    shared_future( shared_future && other) noexcept :
        state_( std::move( other.state_) ) {
        //TODO: constructs a shared_future with the shared state of other using move semantics
        //      after construction, other.valid() == false
    }

    inline
    shared_future( future< void > && other) noexcept :
        state_( std::move( other.state_) ) {
        //TODO: constructs a shared_future with the shared state of other using move semantics
        //      after construction, other.valid() == false
    }

    inline
    shared_future & operator=( shared_future const& other) noexcept
    {
        //TODO:
        if ( this != & other) {
            state_ = other.state_;
        }
        return * this;
    }

    inline
    shared_future & operator=( shared_future && other) noexcept {
        //TODO: releases any shared state and move-assigns the contents of other to *this
        //      after the assignment, other.valid() == false and this->valid() will yield
        //      the same value as other.valid() before the assignment
        if ( this != & other) {
            state_ = std::move( other.state_);
        }
        return * this;
    }

    inline
    shared_future & operator=( future< void > && other) noexcept {
        //TODO:
        state_ = std::move( other.state_);
        return * this;
    }

    inline
    bool valid() const noexcept {
        //TODO: checks if the shared_future refers to a shared state
        //      this is the case only for shared_futures returned by
        //      promise::get_shared_future(), packaged_task::get_shared_future()
        //      or async() until the first time get()or share() is called
        return nullptr != state_.get();
    }

    inline
    void get() const {
        //TODO: the get method waits until the shared_future has a valid result and
        //      (depending on which template is used) retrieves it
        //      it effectively calls wait() in order to wait for the result
        //      the value stored in the shared state
        //      if it satisfies the requirements of MoveAssignable, the value is moved,
        //      otherwise it is copied
        //      valid() == false after a call to this method.
        //      detect the case when valid == false before the call and throw a
        //      future_error with an error condition of future_errc::no_state
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        state_->get();
    }

    inline
    std::exception_ptr get_exception_ptr() {
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->get_exception_ptr();
    }

    inline
    void wait() const {
        //TODO: blocks until the result becomes available
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        state_->wait();
    }

    template< class Rep, class Period >
    future_status wait_for( std::chrono::duration< Rep, Period > const& timeout_duration) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_for( timeout_duration);
    }

    inline
    future_status wait_until( std::chrono::high_resolution_clock::time_point const& timeout_time) const {
        //TODO: blocks until the result becomes available or timeout
        //      valid() == true after the call
        if ( ! valid() ) {
            throw future_uninitialized();
        }
        return state_->wait_until( timeout_time);
    }

    template< typename ClockType >
    future_status wait_until( typename ClockType::time_point const& timeout_time_) const {
        std::chrono::high_resolution_clock::time_point timeout_time(
            std::chrono::high_resolution_clock::now() + ( timeout_time_ - ClockType::now() ) );
        return wait_until( timeout_time);
    }
};


template< typename R >
shared_future< R >
future< R >::share() {
    //TODO: transfer the shared state of *this to a shared_future object
    //      multiple shared_future objects may reference the same shared state,
    //      which is not possible with future
    //      after calling share on a future, valid() == false
    //      detect the case when valid == false before the call and throw a
    //      future_error with an error condition of future_errc::no_state
    if ( ! valid() ) {
        throw future_uninitialized();
    }
    return shared_future< R >( std::move( * this) );
}

template< typename R >
shared_future< R & >
future< R & >::share() {
    //TODO: transfer the shared state of *this to a shared_future object
    //      multiple shared_future objects may reference the same shared state,
    //      which is not possible with future
    //      after calling share on a future, valid() == false
    //      detect the case when valid == false before the call and throw a
    //      future_error with an error condition of future_errc::no_state
    if ( ! valid() ) {
        throw future_uninitialized();
    }
    return shared_future< R & >( std::move( * this) );
}

inline
shared_future< void >
future< void >::share() {
    //TODO: transfer the shared state of *this to a shared_future object
    //      multiple shared_future objects may reference the same shared state,
    //      which is not possible with future
    //      after calling share on a future, valid() == false
    //      detect the case when valid == false before the call and throw a
    //      future_error with an error condition of future_errc::no_state
    if ( ! valid() ) {
        throw future_uninitialized();
    }
    return shared_future< void >( std::move( * this) );
}

}}

#endif
