#pragma once

//std

//mandala
#include "types.hpp"
#include "aabb.hpp"

namespace mandala
{
	namespace details
	{
		template<typename Scalar, typename Enable = void>
		struct padding_t;

		template<typename Scalar>
		struct padding_t<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
		{
			typedef Scalar scalar_type;
			typedef padding_t<scalar_type> type;
			typedef glm::detail::tvec2<scalar_type> size_type;

			scalar_type bottom = 0;
			scalar_type left = 0;
			scalar_type top = 0;
			scalar_type right = 0;

            padding_t() = default;
			padding_t(scalar_type bottom, scalar_type left, scalar_type top, scalar_type right) :
				bottom(bottom),
				left(left),
				top(top),
				right(right)
			{
			}

			padding_t(scalar_type all) :
				padding_t(all, all, all, all)
			{
			}

			scalar_type vertical() const
			{
				return bottom + top;
			}

			scalar_type horizontal() const
			{
				return left + right;
			}

			size_type size() const
			{
				return size_type(horizontal(), vertical());
			}

			template<typename U>
			type operator+(padding_t<U>& rhs) const
			{
				auto sum = *this;
				sum += rhs;
				return sum;
			}

			template<typename U>
			type& operator+=(padding_t<U>& rhs)
			{
				bottom += rhs.bottom;
				left += rhs.left;
				right += rhs.right;
				top += rhs.top;

				return *this;
			}

			template<typename U>
			type operator-(padding_t<U>& rhs) const
			{
				auto sum = *this;
				sum -= rhs;
				return sum;
			}

			template<typename U>
			type& operator-=(padding_t<U>& rhs)
			{
				bottom -= rhs.bottom;
				left -= rhs.left;
				right -= rhs.right;
				top -= rhs.top;

				return *this;
			}

			template<typename U>
			bool operator==(const padding_t<U>& rhs) const
			{
				return bottom == rhs.bottom && left == rhs.left && right == rhs.right && top == rhs.top;
			}

			template<typename U>
			bool operator!=(const padding_t<U>& rhs) const
			{
				return bottom != rhs.bottom || left != rhs.left || right != rhs.right || top != rhs.top;
			}
		};
	}

	typedef details::padding_t<uint8_t>		padding_u8_t;
	typedef details::padding_t<uint16_t>	padding_u16_t;
	typedef details::padding_t<uint32_t>	padding_u32_t;
	typedef details::padding_t<uint64_t>	padding_u64_t;
	typedef details::padding_t<float32_t>	padding_f32_t;
	typedef details::padding_t<float64_t>	padding_f64_t;
	typedef padding_f32_t padding_t;

	template<typename T, typename U>
	details::aabb2_t<T> operator-(const details::aabb2_t<T>& aabb, const details::padding_t<U>& padding)
	{
		return details::aabb2_t<T>(glm::detail::tvec2<T>(aabb.min.x + padding.left, aabb.min.y + padding.bottom), glm::detail::tvec2<T>(aabb.max.x - padding.right, aabb.max.y - padding.top));
	}

	template<typename T, typename U>
	void operator-=(details::aabb2_t<T>& aabb, const details::padding_t<U>& padding)
	{
		aabb.min.x += padding.left;
		aabb.min.y += padding.bottom;
		aabb.max.x -= padding.right;
		aabb.max.y -= padding.top;
	}

	template<typename T, typename U>
	details::aabb2_t<T> operator+(const details::aabb2_t<T>& aabb, const details::padding_t<U>& padding)
	{
		return details::aabb2_t<T>({ aabb.min.x - padding.left, aabb.min.y - padding.bottom }, { aabb.max.x + padding.right, aabb.max.y + padding.top });
	}

	template<typename T, typename U>
	void operator+=(details::aabb2_t<T>& aabb, const details::padding_t<U>& padding)
	{
		aabb.min.x -= padding.left;
		aabb.min.y -= padding.bottom;
		aabb.max.x += padding.right;
		aabb.max.y += padding.top;
	}
}
