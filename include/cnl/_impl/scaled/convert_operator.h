
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_CONVERT_OPERATOR_H)
#define CNL_IMPL_SCALED_CONVERT_OPERATOR_H

#include "../../fraction.h"
#include "../../limits.h"
#include "../num_traits/fixed_width_scale.h"
#include "../num_traits/scale.h"
#include "../operators/native_tag.h"
#include "../power_value.h"
#include "../scaled_integer/declaration.h"
#include "is_same_tag_family.h"
#include "power.h"

/// compositional numeric library
namespace cnl {
    // integer -> floating
    template<_impl::integer Src, int SrcExponent, _impl::floating_point Dest, int DestExponent, int Radix>
    struct custom_operator<
            _impl::convert_op,
            operand<Src, power<SrcExponent, Radix>>,
            operand<Dest, power<DestExponent, Radix>>> {
        [[nodiscard]] constexpr auto operator()(Src const& from) const
        {
            return Dest(from) * _impl::power_value<Dest, SrcExponent - DestExponent, Radix>();
        }
    };

    // floating -> integer
    template<_impl::floating_point Input, int SrcExponent, _impl::integer Result, int DestExponent, int Radix>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, power<SrcExponent, Radix>>,
            operand<Result, power<DestExponent, Radix>>> {
        [[nodiscard]] constexpr auto operator()(Input const& from) const
        {
            return static_cast<Result>(
                    from * _impl::power_value<Input, SrcExponent - DestExponent, Radix>());
        }
    };

    // integer -> integer
    template<_impl::integer Input, int SrcExponent, _impl::integer Result, int DestExponent, int Radix>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, power<SrcExponent, Radix>>, operand<Result, power<DestExponent, Radix>>> {
        [[nodiscard]] constexpr auto operator()(Input const& from) const
        {
            // when converting *from* scaled_integer
            return static_cast<Result>(_impl::scale<SrcExponent - DestExponent, Radix>(
                    _impl::from_value<Result>(from)));
        }
    };

    // shims between equivalent tags
    template<typename Input, typename Result, int DestExponent, int DestRadix>
    struct custom_operator<_impl::convert_op, operand<Input>, operand<Result, power<DestExponent, DestRadix>>>
        : custom_operator<
                  _impl::convert_op,
                  operand<Input, power<0, DestRadix>>,
                  operand<Result, power<DestExponent, DestRadix>>> {
    };

    template<typename Input, int SrcExponent, int SrcRadix, typename Result>
    struct custom_operator<_impl::convert_op, operand<Input, power<SrcExponent, SrcRadix>>, operand<Result>>
        : custom_operator<_impl::convert_op, operand<Input, power<SrcExponent, SrcRadix>>, operand<Result, power<0, SrcRadix>>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // conversion from fraction

    namespace _impl {
        template<typename Number>
        [[nodiscard]] constexpr auto not_scaled_integer(Number const& number)
        {
            return number;
        }

        template<typename Rep, int Exponent, int Radix>
        [[nodiscard]] constexpr auto not_scaled_integer(
                scaled_integer<Rep, power<Exponent, Radix>> const& f)
        {
            return _impl::to_rep(f);
        }

        template<typename Number>
        struct exponent : constant<0> {
        };

        template<typename Rep, int Exponent, int Radix>
        struct exponent<scaled_integer<Rep, power<Exponent, Radix>>> : constant<Exponent> {
        };

        template<class Quotient, class Dividend, class Divisor>
        struct exponent_shift
            : std::integral_constant<
                      int, _impl::exponent<Dividend>::value - _impl::exponent<Divisor>::value
                                   - _impl::exponent<Quotient>::value> {
        };
    }

    template<
            typename SrcNumerator, typename SrcDenominator,
            typename Dest, int DestExponent, int Radix>
    struct custom_operator<
            _impl::convert_op,
            operand<cnl::fraction<SrcNumerator, SrcDenominator>, cnl::power<0, Radix>>,
            operand<Dest, cnl::power<DestExponent, Radix>>> {
        [[nodiscard]] constexpr auto operator()(
                cnl::fraction<SrcNumerator, SrcDenominator> const& from) const
        {
            static_assert(_impl::exponent<Dest>::value == 0, "TODO");

            return static_cast<Dest>(
                    _impl::fixed_width_scale<
                            _impl::exponent<SrcNumerator>::value
                                    - _impl::exponent<SrcDenominator>::value - DestExponent,
                            Radix>(static_cast<Dest>(_impl::not_scaled_integer(from.numerator)))
                    / _impl::not_scaled_integer(from.denominator));
        }
    };
}

#endif  // CNL_IMPL_SCALED_CONVERT_OPERATOR_H
