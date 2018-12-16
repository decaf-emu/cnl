
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_CTORS_H)
#define CNL_IMPL_DUPLEX_INTEGER_CTORS_H 1

#include "type.h"
#include "../type_traits/enable_if.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::calculate_lower
        template<typename Lower, typename Integer>
        constexpr auto calculate_lower(Integer const& input)
        -> enable_if_t<digits<Lower>::value>=digits<Integer>::value, Lower>
        {
            return Lower(input) & numeric_limits<Lower>::max();
        }

        template<typename Lower, typename Integer>
        constexpr auto calculate_lower(Integer const& input)
        -> enable_if_t<digits<Lower>::value<digits<Integer>::value, Lower>
        {
            return Lower(input & Integer{numeric_limits<Lower>::max()});
        }

        // cnl::_impl::calculate upper
        template<typename Upper, typename Lower, typename Integer>
        constexpr auto calculate_upper(Integer const& input)
        -> enable_if_t<digits<Lower>::value>=digits<Integer>::value, Upper>
        {
            // sign-friendly flush
            return static_cast<Upper>((input >> (digits<Integer>::value-1)) >> 1);
        }

        template<typename Upper, typename Lower, typename Integer>
        constexpr auto calculate_upper(Integer const& input)
        -> enable_if_t<digits<Lower>::value<digits<Integer>::value, Upper>
        {
            return sensible_right_shift<Upper>(input, digits<Lower>::value);
        }

        // cnl::_impl::duplex_integer::duplex_integer
        template<typename Upper, typename Lower>
        constexpr int duplex_integer<Upper, Lower>::lower_width;

        template<typename Upper, typename Lower>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(upper_type const& u, lower_type const& l)
                :_upper(u), _lower(l) { }

        template<typename Upper, typename Lower>
        template<typename Integer, _impl::enable_if_t<(numeric_limits<Integer>::is_integer), int> Dummy>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(Integer const& i)
                : _upper(calculate_upper<Upper, Lower>(i)),
                  _lower(calculate_lower<Lower>(i))
        {
        }
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_CTORS_H