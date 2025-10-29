///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_INVOKE_INCLUDED
#define ETL_INVOKE_INCLUDED

#include "platform.h"
#include "functional.h"
#include "function_traits.h"
#include "type_traits.h"
#include "utility.h"

/**
 * Negation wrapper for a type trait value.
 * @tparam T Trait type that exposes a static `value` member convertible to bool.
 * @note Defines `value` as `!bool(T::value)`.
 */

/**
 * Determines whether a type `T` is a function type.
 * @tparam T Type to inspect.
 * @note Specializations for `T&` and `T&&` yield false.
 */

/**
 * Helper to detect member pointer types.
 * @tparam T Type to inspect (unspecialized helper yields false).
 */

/**
 * Detects whether `T` is a member pointer (e.g., `MemberType Class::*`).
 * @tparam T Type to inspect.
 */

/**
 * Helper to detect member function pointer types.
 * @tparam T Type to inspect (unspecialized helper yields false).
 * @note Specialized helper yields `true` only when `T` is a function type member pointer.
 */

/**
 * Detects whether `T` is a pointer to a member function.
 * @tparam T Type to inspect.
 */

/**
 * Helper to detect member object pointer types (member data pointers).
 * @tparam T Type to inspect (unspecialized helper yields false).
 * @note Specialized helper yields true when `T` is a member pointer and not a function type.
 */

/**
 * Detects whether `T` is a pointer to a non-function member object (data member).
 * @tparam T Type to inspect.
 */

/**
 * Invoke a callable that is not a member pointer with the given arguments.
 * @tparam F Callable type.
 * @tparam TArgs Argument types forwarded to the callable.
 * @param f Callable to invoke.
 * @param args Arguments forwarded to `f`.
 * @returns Result of calling `f(args...)`.
 */

/**
 * Invoke a pointer-to-member-function on an object (object is not a pointer).
 * @tparam F Member function pointer type.
 * @tparam T Type of the object instance (not a pointer).
 * @tparam TArgs Argument types forwarded to the member function.
 * @param f Pointer to member function.
 * @param t Object instance on which to invoke the member function.
 * @param args Arguments forwarded to the member function.
 * @returns Result of invoking `(t.*f)(args...)`.
 */

/**
 * Invoke a pointer-to-member-function on an object pointer.
 * @tparam F Member function pointer type.
 * @tparam T Type of the object pointer.
 * @tparam TArgs Argument types forwarded to the member function.
 * @param f Pointer to member function.
 * @param t Pointer to the object on which to invoke the member function.
 * @param args Arguments forwarded to the member function.
 * @returns Result of invoking `((*t).*f)(args...)`.
 */

/**
 * Access a member object (data member) through an object instance (non-pointer).
 * @tparam F Pointer-to-member-object type.
 * @tparam T Type of the object instance (not a pointer).
 * @param f Pointer to member object.
 * @param t Object instance from which to access the member.
 * @returns The selected member `t.*f`.
 */

/**
 * Access a member object (data member) through an object pointer.
 * @tparam F Pointer-to-member-object type.
 * @tparam T Type of the object pointer.
 * @param f Pointer to member object.
 * @param t Pointer to the object from which to access the member.
 * @returns The selected member `((*t).*f)`.
 */

/**
 * Primary forward declaration for invoke result detection helper.
 * @tparam F Callable type.
 * @tparam (unused) SFINAE helper parameter.
 * @tparam Us Argument types.
 */

/**
 * SFINAE-enabled specialization that deduces the result type of invoking `F` with `Us...`.
 * @tparam F Callable type.
 * @tparam Us Argument types.
 * @note Provides member alias `type` equal to the decltype of `etl::invoke(declval<F>(), declval<Us>()...)`.
 */

/**
 * Alias template mapping to `invoke_result_impl` for deducing the result type of an invocation.
 * @tparam F Callable type.
 * @tparam Us Argument types.
 */

/**
 * Alias for the deduced result type of invoking `F` with `Us...`.
 * @tparam F Callable type.
 * @tparam Us Argument types.
 */

/**
 * Deprecated alias name retained for compatibility; equivalent to `invoke_result_t<F, Us...>`.
 * @tparam F Callable type.
 * @tparam Us Argument types.
 */
namespace etl {
  template <typename T> struct __not_ : etl::integral_constant<bool, !bool(T::value)> {};

  /// is T a function -- a function cannot be const qualified like a variable
  template<typename T> struct is_function : public etl::integral_constant<bool, !etl::is_const<const T>::value> { };
  template<typename T> struct is_function<T&> : public etl::false_type { };
  template<typename T> struct is_function<T&&> : public etl::false_type { };

  /// is T a member pointer
  template<typename  T> struct is_member_pointer_helper : etl::false_type {};
  template<typename T, typename C> struct is_member_pointer_helper<T C::*> : etl::true_type {};
  template<typename T> struct is_member_pointer : is_member_pointer_helper<etl::remove_cv_t<T>> {};

  /// is T a member function pointer
  template <typename> struct is_member_function_pointer_helper : etl::false_type {};
  template <typename T, typename C> struct is_member_function_pointer_helper<T C::*> : public etl::is_function<T>::type {};
  template <typename T> struct is_member_function_pointer : public is_member_function_pointer_helper<etl::remove_cv_t<T>>::type {};

  /// is T a member object pointer
  template<typename> struct is_member_object_pointer_helper : public etl::false_type { };
  template<typename T, typename C> struct is_member_object_pointer_helper<T C::*> : public __not_<etl::is_function<T>>::type { };
  template<typename T> struct is_member_object_pointer : public is_member_object_pointer_helper<etl::remove_cv_t<T>>::type {};

  template <
    typename F, 
    typename ... TArgs, 
    typename = typename etl::enable_if<
      !etl::is_member_pointer<etl::decay_t<F>>::value>::type
    >
  ETL_CONSTEXPR auto invoke(F&& f, TArgs&& ... args) 
    -> decltype(etl::forward<F>(f)(etl::forward<TArgs>(args)...)) {
    return etl::forward<F>(f)(etl::forward<TArgs>(args)...);
  }

  template <
    typename F, 
    typename T, 
    typename... TArgs,
    typename DFn = etl::decay_t<F>,
    typename = typename etl::enable_if<
      etl::is_member_function_pointer<DFn>::value && 
      !etl::is_pointer<etl::decay_t<T>>::value
    >::type
  >
  ETL_CONSTEXPR auto invoke(F&& f, T&& t, TArgs&&... args)
      -> decltype((etl::forward<T>(t).*f)(etl::forward<TArgs>(args)...))
  {
      return (etl::forward<T>(t).*f)(etl::forward<TArgs>(args)...);
  }

    template <
    typename F, 
    typename T, 
    typename ... TArgs,
    typename DFn = etl::decay_t<F>,
    typename = typename etl::enable_if<
      etl::is_member_function_pointer<DFn>::value && 
      etl::is_pointer<etl::decay_t<T>>::value
    >::type
  >
  ETL_CONSTEXPR auto invoke(F&& f, T&& t, TArgs&&... args)
      -> decltype(((*etl::forward<T>(t)).*f)(etl::forward<TArgs>(args)...))
  {
      return ((*etl::forward<T>(t)).*f)(etl::forward<TArgs>(args)...);
  }

  template <
    typename F, 
    typename T,
    typename DFn = etl::decay_t<F>,
    typename = typename etl::enable_if<
      etl::is_member_object_pointer<DFn>::value && 
      !etl::is_pointer<etl::decay_t<T>>::value
    >::type
  >
  ETL_CONSTEXPR auto invoke(F&& f, T&& t)
      -> decltype(etl::forward<T>(t).*f)
  {
      return etl::forward<T>(t).*f;
  }

  template <
    typename F,
    typename T,
    typename DFn = etl::decay_t<F>,
    typename = typename etl::enable_if<
      etl::is_member_object_pointer<DFn>::value &&
      etl::is_pointer<etl::decay_t<T>>::value
    >::type
  >
  ETL_CONSTEXPR auto invoke(F&& f, T&& t)
      -> decltype(((*etl::forward<T>(t)).*f))
  {
      return ((*etl::forward<T>(t)).*f);
  }

  template <class F, class, class ... Us> struct invoke_result_impl;

  template <typename F, typename... Us>
  struct invoke_result_impl<
      F,
      etl::void_t<decltype(etl::invoke(etl::declval<F>(), etl::declval<Us>()...))>,
      Us...
    > {
    using type = decltype(etl::invoke(etl::declval<F>(), etl::declval<Us>()...));
  };

  template <typename F, typename... Us>
  using invoke_result = invoke_result_impl<F, void, Us...>;

  template <typename F, typename... Us>
  using invoke_result_t = typename invoke_result<F, Us...>::type;

  template <typename F, typename ... Us>
  using deduced_result_type = typename invoke_result<F, Us...>::type;
}

#endif