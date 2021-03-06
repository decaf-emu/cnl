
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H

#include "../operators/native_tag.h"
#include "is_rounding_tag.h"
#include "is_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief tag to match the rounding behavior of fundamental arithmetic types
    ///
    /// Arithmetic operations using this tag should behave the same as equivalent operators.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::nearest_rounding_tag
    struct native_rounding_tag
        : _impl::homogeneous_deduction_tag_base
        , _impl::homogeneous_operator_tag_base {
    };

    namespace _impl {
        template<>
        struct is_rounding_tag<native_rounding_tag> : std::true_type {
        };
    }

    template<typename Source, typename Destination>
    struct custom_operator<_impl::convert_op, operand<Source>, operand<Destination, native_rounding_tag>>
        : custom_operator<_impl::convert_op, operand<Source>, operand<Destination>> {
    };

    template<_impl::unary_arithmetic_op Operator, typename Operand>
    struct custom_operator<Operator, operand<Operand, native_rounding_tag>>
        : custom_operator<Operator, operand<Operand, _impl::native_tag>> {
    };

    template<_impl::binary_arithmetic_op Operator, typename Lhs, typename Rhs>
    struct custom_operator<
            Operator,
            operand<Lhs, native_rounding_tag>,
            operand<Rhs, native_rounding_tag>>
        : custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
    };

    template<_impl::shift_op Operator, tag RhsTag, typename Lhs, typename Rhs>
    struct custom_operator<
            Operator,
            operand<Lhs, native_rounding_tag>, operand<Rhs, RhsTag>>
        : custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
    };

    template<_impl::prefix_op Operator, typename Rhs>
    struct custom_operator<Operator, native_rounding_tag, Rhs>
        : custom_operator<Operator, operand<Rhs, _impl::native_tag>> {
    };

    template<_impl::postfix_op Operator, typename Rhs>
    struct custom_operator<Operator, native_rounding_tag, Rhs>
        : custom_operator<Operator, operand<Rhs, _impl::native_tag>> {
    };
}

#endif  // CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H
