#pragma once

#include "conceptual/aliases/numeric_limits.hpp"
#include "conceptual/traits/properties.hpp"


// TODO: Fix this god awful mess

namespace ham::cpt
{
namespace detail
{

template <class ExpectedMemberType, class... Members>
concept check_nlim_splzn_member_types = 
    (req_same_as<ExpectedMemberType, decltype(Members::value)> && ...);

// The extra equality check is there for the case of NaNs and such
template <template <class> class NlimMember, class ExpectedMemberType, class T, class... Ts>
concept check_nlim_splzn_member_values =
    ((
           (NlimMember<T>::value == NlimMember<T>::value) 
        == (NlimMember<T>::value == NlimMember<Ts>::value)) && ...)
    && check_nlim_splzn_member_types<NlimMember<T>, NlimMember<Ts...>>;

template <class Type, class ExpectedMemberType, template <class> class... Members>
consteval bool check_nlim_members()
{
    using T   = std::remove_cv_t<Type>;
    using Tc  = T const;
    using Tv  = T volatile;
    using Tcv = T const volatile;
    return (check_nlim_spclzn_member_values<
        Members, ExpectedMemberType, T, Tc, Tv, Tcv> && ...);
}


template <class Type, class ExpectedMemberType, template <class> class... Members>
consteval bool check_nlim_member_types()
{
    using T   = std::remove_cv_t<Type>;
    using Tc  = T const;
    using Tv  = T volatile;
    using Tcv = T const volatile;
    return (check_nlim_spclzn_member_types<
        ExpectedMemberType, Members<T>, Members<Tc>, Members<Tv>, Members<Tcv>> && ...);
}



template <class T>
concept nlim_specialized_impl =
       (check_nlim_members<T, bool, nlim_is_specialized>)()
    && (check_nlim_members<T, bool, 
            nlim_is_specialized,
            nlim_is_signed,
            nlim_is_integer,
            nlim_is_exact,
            nlim_is_iec559,
            nlim_is_bounded,
            nlim_is_modulo,
        
            nlim_has_infinity,
            nlim_has_quiet_NaN,
            nlim_has_signaling_NaN,
            nlim_has_denorm_loss,
        
            nlim_traps,
            nlim_tinyness_before
        >)()
    && (check_nlim_members<T, int, 
            nlim_digits,
            nlim_digits10,
            nlim_max_digits10,

            nlim_radix,
            
            nlim_min_exponent,
            nlim_min_exponent10,
            nlim_max_exponent,
            nlim_max_exponent10
        >)()
    && (check_nlim_members<T, std::float_denorm_style, nlim_has_denorm>)()
    && (check_nlim_members<T, std::float_round_style, nlim_round_style>)()
    && (check_nlim_members<T, T, 
            nlim_min,
            nlim_max,
            nlim_lowest,
            
            nlim_epsilon,
            nlim_round_error,
            nlim_infinity,
            nlim_quiet_NaN,
            nlim_signaling_NaN,
            nlim_denorm_min
        >)()
    && (  !nlim_is_iec559_v<T> 
        || nlim_has_signaling_NaN_v<T> && nlim_has_quiet_NaN_v<T>);

}

template <class T>
concept nlim_specialized = req_arithmetic<T> || detail::nlim_specialized_impl<T>;


}