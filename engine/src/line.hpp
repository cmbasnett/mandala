#pragma once

//naga
#include "types.hpp"

namespace naga
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct Line2;

        template<typename Scalar>
        struct Line2<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
            typedef Scalar ScalarType;
            typedef Line2<ScalarType> Type;
            typedef glm::tvec2<ScalarType> VectorType;
            typedef Line2<f32> RealType;

            VectorType start;
            VectorType end;

            Line2() = default;

            Line2(const VectorType& start, const VectorType& end) :
                start(start),
                end(end)
            {
            }

            RealType::VectorType direction() const
            {
                return glm::normalize((RealType::VectorType)end - (RealType::VectorType)start);
            }

            f32 length() const
            {
                return glm::length((RealType::VectorType)end - (RealType::VectorType)start);
            }

            Type operator-(const VectorType& t) const
            {
                type sum;
                sum.start = start - t;
                sum.end = end - t;
                return sum;
            }

			Type& operator-=(const VectorType& t)
            {
                *this = *this - t;
                return *this;
            }

			Type operator+(const VectorType& t) const
            {
				Type sum;
                sum.start = start + t;
                sum.end = end + t;
                return sum;
            }

			Type& operator+=(const VectorType& t)
            {
                *this = *this + t;
                return *this;
            }
        };

        template<typename Scalar, typename Real = void, typename Enable = void>
        struct Line3;

        template<typename Scalar>
        struct Line3<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
            typedef Scalar ScalarType;
            typedef Line3<ScalarType> Type;
            typedef glm::tvec3<ScalarType> VectorType;
            typedef Line3<f32> RealType;

            VectorType start;
            VectorType end;

            Line3() = default;

            Line3(const VectorType& start, const VectorType& end) :
                start(start),
                end(end)
            {
            }

            RealType::VectorType direction() const
            {
                return glm::normalize((RealType::VectorType)end - (RealType::VectorType)start);
            }

            f32 length() const
            {
                return glm::length((RealType::VectorType)end - (RealType::VectorType)start);
            }

			Type operator-(const VectorType& t) const
            {
				Type sum;
                sum.start = start - t;
                sum.end = end - t;
                return sum;
            }

			Type& operator-=(const VectorType& t)
            {
                *this = *this - t;
                return *this;
            }

			Type operator+(const VectorType& t) const
            {
				Type sum;
                sum.start = start + t;
                sum.end = end + t;
                return sum;
            }

            Type& operator+=(const VectorType& t)
            {
                *this = *this + t;
                return *this;
            }
        };
    }

	typedef details::Line2<f32> Line2;
    typedef details::Line3<f32> Line3;
}
