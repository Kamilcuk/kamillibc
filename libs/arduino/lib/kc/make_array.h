
#include <type_traits>
#include <array>

namespace kc {

template<class...> struct conjunction : std::true_type { };
template<class kc_B1> struct conjunction<kc_B1> : kc_B1 { };
template<class kc_B1, class... kc_Bn>
struct conjunction<kc_B1, kc_Bn...> 
    : std::conditional<bool(kc_B1::value), conjunction<kc_Bn...>, kc_B1>::type {};

template <bool kc_B>
using bool_constant = std::integral_constant<bool, kc_B>;

template<class kc_B>
struct negation : kc::bool_constant<!bool(kc_B::value)> { };

namespace details {
  template<class> struct is_ref_wrapper : std::false_type {};
  template<class T> struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};
 
  template<class T>
  using not_ref_wrapper = kc::negation<is_ref_wrapper<typename std::decay<T>::type>>;
 
  template <class D, class...> struct return_type_helper { using type = D; };
  template <class... Types>
  struct return_type_helper<void, Types...> : std::common_type<Types...> {
      static_assert(kc::conjunction<not_ref_wrapper<Types>...>::value,
                    "Types cannot contain reference_wrappers when D is void");
  };
 
  template <class D, class... Types>
  using return_type = std::array<typename return_type_helper<D, Types...>::type,
                                 sizeof...(Types)>;
}
 
template < class D = void, class... Types>
constexpr details::return_type<D, Types...> make_array(Types&&... t) {
  return {std::forward<Types>(t)... };
}

}

