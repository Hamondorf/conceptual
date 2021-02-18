#pragma once

#include <bit>
#include <cstddef>
#include <compare>

#include "conceptual/aliases/numeric_limits.hpp"
#include "conceptual/traits/properties.hpp"
#include "conceptual/concepts/objects.hpp"

#include "numeric.hpp"


#define HAM_CPT_TRY_REQ(...) \
    (!requires { requires ::ham::cpt::detail::bool_v<!(__VA_ARGS__)>; })

namespace ham::cpt
{
namespace detail
{

// This isn't really complete but it does provide at least a few
// arithmetic properties that should hold true
template <class T>
concept req_numeric_type = 
       req_arithmetic<T> 
    || req_regular<T> 
    && req_totally_ordered<T>
    && requires
    {
        requires (T{} == T(0));
    };

// Helper variable template for checking requirements involving operations
// on numeric types. If any of the expressions are non-constant, the program
// would be ill-formed; however, failure to do so as part of a template 
// parameter substitution will count as substitution failure, and as such
// will not render the program ill-formed.
//
// (using a variable template instead of a concept since GCC will render
// the program ill-formed on the substituion failure into the concept)
template <bool b>
inline constexpr bool bool_v = b;

static_assert(bool_v<true> && !bool_v<false>, "how");


namespace nlim_impl
{


struct NotMeaningfulFn
{
    template <class T>
    consteval bool operator()(T const& value) const noexcept
    {
        return value == T((int)0);
    }

    template <class T, class... Ts>
    consteval bool operator()(T const& value, Ts const&... values) const noexcept
    {
        if constexpr (!(req_same_as<T, Ts> && ...))
            return false;
        else
            return ((*this)(value) && ...  && (*this)(values));
    }

    constexpr bool operator==(NotMeaningfulFn const&) const noexcept = default;
};

constexpr NotMeaningfulFn not_meaningful{};

template <class T>
consteval bool check_min_max_and_lowest()
{
    constexpr auto&& min    = nlim_min_v<T>;
    constexpr auto&& max    = nlim_max_v<T>;
    constexpr auto&& lowest = nlim_lowest_v<T>;

    if constexpr (nlim_is_bounded_v<T>) {
        if constexpr (lowest == max)
            return (lowest == min) && (min == max);
        else
            return (lowest <= min) && (min <= max) && (lowest < max);
    }
    else {
        return not_meaningful(lowest) && not_meaningful(max)
            && (!nlim_is_signed<T> || not_meaningful(min));
    }
}

constexpr auto const& cmp_partial_order = std::compare_partial_order_fallback;

template <class T, template <class> class NlimNaN>
consteval std::partial_ordering NaN_ordering() noexcept
{
    return std::compare_partial_order_fallback(
        NlimNaN<T>::value, NlimNan<T>::value);
}



template <class Desired, bool is_ordered = true, class T, class... Ts>
consteval bool enforce_consistent_values_impl(T const& member, Ts const&... cv_members)
{
    constexpr std::partial_ordering unordered = std::partial_ordering::unordered;
    
    if constexpr (!(req_same_as<Desired, T> && ... && req_same_as<Desired, Ts>)) {
        return false;
    }
    else {
        if constexpr (is_ordered)
            return ((member == cv_members) && ...);
        else
            return  (cmp_partial_order(member    , member    ) == unordered)
                && ((cmp_partial_order(cv_members, cv_members) == unordered) && ...);
    }
}

template <class T>
consteval bool enforce_consistent_NaNs()
{
    static_assert(req_same_as<std::remove_cv_t<T>, T>, 
        "Implementation Bug: T should be cv-unqualified.");

    using Tc  = T const;
    using Tv  = T volatile;
    using Tcv = T const volatile;

    bool valid_qNaN = 
       /*IF*/ nlim_has_quiet_NaN_v<T> ? 
            (enforce_consistent_values_impl<T, false>)(
                nlim_quiet_NaN_v<T >, nlim_quiet_NaN_v<Tc >,
                nlim_quiet_NaN_v<Tv>, nlim_quiet_NaN_v<Tcv>)

       /*ELSE*/ : 
            not_meaningful(
                nlim_quiet_NaN_v<T >, nlim_quiet_NaN_v<Tc >,
                nlim_quiet_NaN_v<Tv>, nlim_quiet_NaN_v<Tcv>);

    bool valid_sNaN = 
        /*IF*/ nlim_has_signaling_NaN_v<T> ? 
        (enforce_consistent_values_impl<T, false>)(
            nlim_signaling_NaN_v<T >, nlim_signaling_NaN_v<Tc >,
            nlim_signaling_NaN_v<Tv>, nlim_signaling_NaN_v<Tcv>)

        /*ELSE*/ : 
        not_meaningful(
            nlim_signaling_NaN_v<T >, nlim_signaling_NaN_v<Tc >,
            nlim_signaling_NaN_v<Tv>, nlim_signaling_NaN_v<Tcv>);

    return valid_qNaN && valid_sNaN;

    
}

template <class T, class Desired, template <class> class... NlimMembers>
consteval bool enforce_consistent_values()
{
    static_assert(req_same_as<std::remove_cv_t<T>, T>, 
        "Implementation Bug: T should be cv-unqualified.");

    using Tc  = T const;
    using Tv  = T volatile;
    using Tcv = T const volatile;

    return (((enforce_consistent_values_impl<Desired>)(
                NlimMembers<T >::value, NlimMembers<Tc >::value,
                NlimMembers<Tv>::value, NlimMembers<Tcv>::value))
            && ...);
}

} // nlim_impl



template <class Type, class T = std::remove_cv_t<Type>>
concept consistent_nlim_member_values =
    bool_v<
        nlim_impl::enforce_consistent_values<
            T, bool,

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
            nlim_has_denorm_loss
        >()
        && nlim_impl::enforce_consistent_values<
            T, int,

            nlim_digits,
            nlim_digits10,
            nlim_max_digits10,
            
            nlim_radix,
            
            nlim_min_exponent,
            nlim_min_exponent10,
            nlim_max_exponent,
            nlim_max_exponent10
        >()
        && nlim_impl::enforce_consistent_values<
            T, std::float_round_style,
            
            nlim_round_style
        >()
        && nlim_impl::enforce_consistent_values<
            T, std::float_denorm_style,
            
            nlim_has_denorm
        >()
    >;

//  Certain members are only "meaningful" for certain types and in accordance
//  with the standard, 
//      "Any value that is not 'meaningful' shall be set to 0 or false." 
//      - https://eel.is/c++draft/numeric.special
template <class T>
concept valid_nlim_member_values =
    bool_v<        
           (  !nlim_is_iec559_v<T> // 1
            || 
               nlim_has_infinity_v<T> 
            && nlim_has_quiet_NaN_v<T> 
            && nlim_has_signaling_NaN_v<T>
            // && !nlim_is_exact_v<T> 
            // ^^^ not really sure if this holds true for every IEEE-754 floating-point
            && !nlim_is_integer_v<T>)

        && (  !nlim_is_integer_v<T> // 2
            || 
                nlim_is_exact_v<T> 
            && !nlim_is_iec559_v<T>
            && !nlim_has_infinity_v<T>
            && !nlim_has_quiet_NaN_v<T> 
            && !nlim_has_signaling_NaN_v<T>
            && !nlim_tinyness_before_v<T>

            && nlim_has_denorm_v<T> == std::denorm_absent
            && nlim_round_style_v<T> == std::round_toward_zero

            && nlim_impl::not_meaningful(
                nlim_epsilon_v<T>,
                nlim_denorm_min_v<T>)

            && nlim_impl::not_meaningful(
                nlim_min_exponent_v<T>,
                nlim_max_exponent_v<T>,
                nlim_min_exponent10_v<T>,
                nlim_max_exponent10_v<T>,
                nlim_max_digits10_v<T>
            ))
        
        && (  !nlim_has_infinity_v<T> 
            || nlim_impl::not_meaningful(nlim_infinity_v<T>))
        
        && (   nlim_is_bounded_v<T>
            || nlim_impl::not_meaningful(nlim_digits10_v<T>))

        && (nlim_impl::enforce_consistent_NaNs<T>())
        && (nlim_impl::check_min_max_and_lowest<T>())
        
        && (nlim_radix_v<T> > 1)
    >;

//  1: constraints imposed upon types that define 'is_iec559' as true
//     (thus identifying as conformant to the IEEE-754 standard)
// 
//      - has_infinty, has_quiet_NaN, has_signaling_NaN, must all be true
//      - is_integer should be false

//  2: constaints imposed upon types imposed upon types that define 
//     'is_integer' as true. 
// 
//      - is_exact should be true https://eel.is/c++draft/numeric.limits#members-20
// 
//      - the following specializations are only meaningful for specializations
//        involving floating point types (and thus should be false in the case
//        an integer specialization):
//          - is_iec559, has_infinty, has_quiet_NaN, has_signaling_NaN, 
//            tinyness_before
// 
//      - has_denorm == denorm_absent 
//      - round_style == round_towards_zero
//
//

//  TODO: Finish explaining the mess that is all of the preconditions for a type
//        being a valid numeric limits specialization. And maybe add toggleable 
//        static_asserts for various preconditions.
//


template <class T>
concept nlim_specialized_for_impl =
       bool_v<T((int)0) == T()>
    && consistent_nlim_member_values<T>
    && valid_nlim_member_values<T>;

}

}