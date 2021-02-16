#pragma once

#include <compare>

#include "conceptual/aliases/numeric_limits.hpp"
#include "conceptual/traits/properties.hpp"
#include "conceptual/concepts/objects.hpp"

#include "numeric.hpp"


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

namespace nlim_impl
{

template <class T>
using rm_cv = std::remove_cv_t<T>;

template <class T>
using const_ = rm_cv<T> const;

template <class T>
using volatile_ = rm_cv<T> volatile;

template <class T>
using cv_ = rm_cv<T> const volatile;

constexpr auto const& cmp_partial_order = std::compare_partial_order_fallback;

template <class T, template <class> class NlimNaN>
consteval std::partial_ordering NaN_ordering() noexcept
{
    return cmp_partial_order(NlimNaN<T>::value, NlimNaN<T>::value);
}


#if __INTELLISENSE__
template <class T>
concept has_value_member = requires { typename T::value_type; };

template <class T, template <class> class NlimMember, class MemberType>
consteval bool valid_member_test()
{
    if constexpr (has_value_member<NlimMember<T>>)
        return std::same_as<MemberType, typename NlimMember<T>::value_type>;
    else
        return false;
}


template <class T, template <class> class NlimMember, class MemberType>
concept req_valid_nlim_member_for =
(valid_member_test<T, NlimMember, MemberType>)();

#else

template <class T, template <class> class NlimMember, class MemberType>
inline constexpr bool req_valid_nlim_member_for =
std::same_as<MemberType, typename NlimMember<T>::value_type>;

#endif

template <class T, template <class> class NlimMember, class MemberType>
concept req_valid_nlim_member = 
       req_valid_nlim_member_for<rm_cv<T>, NlimMember, MemberType>
    && req_valid_nlim_member_for<const_<T>, NlimMember, MemberType>
    && req_valid_nlim_member_for<volatile_<T>, NlimMember, MemberType>
    && req_valid_nlim_member_for<cv_<T>, NlimMember, MemberType>;


template <class T>
concept probably_floating_point = 
    !nlim_is_integer_v<T> && !nlim_is_exact_v<T> || nlim_is_iec559_v<T>;


template <bool has_NaN, class T, template<class> class NlimNaN>
//requires (!has_NaN) || req_valid_nlim_member<T, NlimNaN, T>
consteval bool check_NaNs_impl() noexcept
{
    static_assert(std::same_as<T, std::remove_cv_t<T>>, 
        "Implementation Error: T should be cv-unqualified.");

    if constexpr (!has_NaN) {
        return true;
    }
    else if constexpr (!req_valid_nlim_member<T, NlimNaN, T>) {
        return false;
    }
    else {
        constexpr auto ordering0 = NaN_ordering<T, NlimNaN>();
        constexpr std::partial_ordering orderings[4] = {
            (NaN_ordering<T, NlimNaN>)(),
            (NaN_ordering<T const, NlimNaN>)(),
            (NaN_ordering<T volatile, NlimNaN>)(),
            (NaN_ordering<T const volatile, NlimNaN>)()
        };

        for (auto ordering : orderings)
        {
            if (ordering != orderings[0])
                return false;
        }

        if constexpr (orderings[0] == std::partial_ordering::unordered) {
            return true;
        }
        else if constexpr (orderings[0] == std::partial_ordering::equivalent) {
            constexpr NlimNaN<T> nl_NaN{};
            constexpr NlimNaN<const_<T>> nl_NaN_c{};
            constexpr NlimNaN<volatile_<T>> nl_NaN_v{};
            constexpr NlimNaN<cv_<T>> nl_NaN_cv{};

            return (nl_NaN() == nl_NaN_c) && (nl_NaN() == nl_NaN_v) && (nl_NaN() == nl_NaN_cv);
        }
        else {
            return false;
        }
    }
}

template <class T>
consteval bool check_NaNs()
{
    return (check_NaNs_impl<nlim_has_quiet_NaN_v<T>, nlim_quiet_NaN>)()
        && (check_NaNs_impl<nlim_has_signaling_NaN_v<T>, nlim_signaling_NaN>)();
}

template <class T>
consteval bool check_min_max_and_lowest()
{
    constexpr bool valid_min    = req_valid_nlim_member_for<T, nlim_min, T>;
    constexpr bool valid_max    = req_valid_nlim_member_for<T, nlim_max, T>;
    constexpr bool valid_lowest = req_valid_nlim_member_for<T, nlim_lowest, T>;

    if constexpr (valid_min && valid_max && valid_lowest) {
        constexpr auto&& min = nlim_min_v<T>;
        constexpr auto&& max = nlim_max_v<T>;
        constexpr auto&& lowest = nlim_lowest_v<T>;

        if constexpr (nlim_is_bounded_v<T>) {

            if constexpr (lowest == max)
                return lowest == min && max == min;
            else
                return lowest <= min && min <= max && lowest < max;
        }
        else if constexpr (!nlim_is_signed_v<T>) { // bounded == false && signed == false
            return valid_min && lowest == 0 && max == 0;
        }
    }
    else // bounded == false && signed == true
        return false;
    
}


namespace WIP
{

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

struct NotMeaningfulFn
{
    template <class T>
    consteval bool operator()(T const& value) const noexcept
    {
        return value == T();
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

template <class T, template <class> class NlimNaN>
consteval std::partial_ordering NaN_ordering() noexcept
{
    return std::compare_partial_order_fallback(
        NlimNaN<T>::value, NlimNan<T>::value);
}

template <class T, template <class> class NlimNaN, bool is_meaningful>
consteval bool check_NaNs_impl()
{

}





}


}

}

}