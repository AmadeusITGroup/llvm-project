#ifndef POINTERS_H_
#define POINTERS_H_

#include "nullable_pointers/simulate_std.hpp"

namespace gsl {

namespace details
{

template <typename T, typename = void>
struct is_comparable_to_nullptr : std::false_type
{
};

template <typename T>
struct is_comparable_to_nullptr<
  T,
  std::enable_if_t<std::is_convertible<decltype(std::declval<T>() != nullptr), bool>::value>>
  : std::true_type
{
};

template <typename T>
struct is_int : std::false_type {};

template <>
struct is_int<int*> : std::true_type {};

template<typename T>
using value_or_reference_return_t = std::conditional_t<
  is_int<T>::value,
              const T,
              const T&>;

[[noreturn]] inline void terminate() noexcept
{
  std::terminate();
}


} // namespace details

#define GSL_LIKELY(x) __builtin_expect(!!(x), 1)
#define GSL_UNLIKELY(x) __builtin_expect(!!(x), 0)

#define GSL_CONTRACT_CHECK(type, cond)                                                             \
    (GSL_LIKELY(cond) ? static_cast<void>(0) : gsl::details::terminate())

#define Expects(cond) GSL_CONTRACT_CHECK("Precondition", cond)

template <class T>
class not_null
{
public:
  static_assert(details::is_comparable_to_nullptr<T>::value, "T cannot be compared to nullptr.");

  template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
  constexpr not_null(U&& u) noexcept : ptr_(std::forward<U>(u))
  {
    Expects(ptr_ != nullptr);
  }

  template <typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
  constexpr not_null(T u) noexcept : ptr_(std::move(u))
  {
    Expects(ptr_ != nullptr);
  }

  template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
  constexpr not_null(const not_null<U>& other) noexcept : not_null(other.get())
  {}

  not_null(const not_null& other) = default;
  not_null& operator=(const not_null& other) = default;
  constexpr details::value_or_reference_return_t<T> get() const
    noexcept
  {
    return ptr_;
  }

  constexpr operator T() const { return get(); }
  constexpr decltype(auto) operator->() const { return get(); }
  constexpr decltype(auto) operator*() const { return *get(); }

  // prevents compilation when someone attempts to assign a null pointer constant
  not_null(std::nullptr_t) = delete;
  not_null& operator=(std::nullptr_t) = delete;

  // unwanted operators...pointers only point to single objects!
  not_null& operator++() = delete;
  not_null& operator--() = delete;
  not_null operator++(int) = delete;
  not_null operator--(int) = delete;
  not_null& operator+=(std::ptrdiff_t) = delete;
  not_null& operator-=(std::ptrdiff_t) = delete;
  void operator[](std::ptrdiff_t) const = delete;

private:
  T ptr_;
};

} // end namespace gsl

#endif // POINTERS_H_
