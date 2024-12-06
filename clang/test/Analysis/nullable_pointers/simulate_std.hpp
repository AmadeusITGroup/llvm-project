#ifndef SIMULATE_STD_H_
#define SIMULATE_STD_H_

namespace std {

template<typename _Tp, _Tp __v>
struct integral_constant
{
  static constexpr _Tp                  value = __v;
  typedef _Tp                           value_type;
  typedef integral_constant<_Tp, __v>   type;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};

using true_type =  integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template<typename _Tp, typename _Up = _Tp&&>
_Up
__declval(int);

template<typename _Tp>
_Tp
__declval(long);

template<typename _Tp>
auto declval() noexcept -> decltype(__declval<_Tp>(0))
{
  return __declval<_Tp>(0);
}

template<bool __v>
using __bool_constant = integral_constant<bool, __v>;

template<typename _From, typename _To>
struct is_convertible
  : public __bool_constant<__is_convertible(_From, _To)>
{ };

template<bool, typename _Tp = void>
struct enable_if
{ };

template<typename _Tp>
struct enable_if<true, _Tp>
{ typedef _Tp type; };

template<bool _Cond, typename _Tp = void>
using enable_if_t = typename enable_if<_Cond, _Tp>::type;

template<typename _Tp, typename _Up>
struct is_same
  : public integral_constant<bool, __is_same(_Tp, _Up)>
{ };

template<typename _Tp>
struct remove_reference
{ using type = _Tp; };

template<typename _Tp>
struct remove_reference<_Tp&>
{ using type = _Tp; };

template<typename _Tp>
struct remove_reference<_Tp&&>
{ using type = _Tp; };

template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type& __t) noexcept
{ return static_cast<_Tp&&>(__t); }

template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
{
  return static_cast<_Tp&&>(__t);
}

template<typename _Tp>
constexpr typename std::remove_reference<_Tp>::type&&
move(_Tp&& __t) noexcept
{ return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }

template<typename...> using __void_t = void;

// template<typename _Tp, typename = void>
// struct __add_lvalue_reference_helper
// { using type = _Tp; };

// template<typename _Tp>
// struct __add_lvalue_reference_helper<_Tp, __void_t<_Tp&>>
// { using type = _Tp&; };

// template<typename _Tp>
// using __add_lval_ref_t = typename __add_lvalue_reference_helper<_Tp>::type;

// template<typename _Tp, typename... _Args>
// using __is_trivially_constructible_impl
// = __bool_constant<__is_trivially_constructible(_Tp, _Args...)>;

// template<typename _Tp>
// struct is_trivially_copy_constructible
//   : public __is_trivially_constructible_impl<_Tp, __add_lval_ref_t<const _Tp>>
// {
// };

template<bool _Cond, typename _Iftrue, typename _Iffalse>
struct conditional
{ typedef _Iftrue type; };

template<typename _Iftrue, typename _Iffalse>
struct conditional<false, _Iftrue, _Iffalse>
{ typedef _Iffalse type; };

template<bool _Cond, typename _Iftrue, typename _Iffalse>
using conditional_t = typename conditional<_Cond, _Iftrue, _Iffalse>::type;

void terminate() noexcept __attribute__ ((__noreturn__));

typedef decltype(nullptr) nullptr_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

}

extern void __assert_fail (const char *__assertion, const char *__file, unsigned int __line, const char *__function) noexcept (true) __attribute__ ((__noreturn__));

#  define assert(expr) \
     (static_cast <bool> (expr) \
      ? void (0) \
      : __assert_fail (#expr, __builtin_FILE(), __builtin_LINE(), \
                       __extension__ __PRETTY_FUNCTION__))

#endif // SIMULATE_STD_H_
