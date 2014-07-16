#pragma once

#include "types.hpp"
#include "aabb.hpp"

namespace mandala
{
	namespace details
	{
		template<typename T>
		struct padding_t
		{
			typedef padding_t<T> type;
			typedef glm::detail::tvec2<T> size_type;

			T bottom = 0;
			T left = 0;
			T top = 0;
			T right = 0;

			padding_t()
			{
			}

			padding_t(T bottom, T left, T top, T right) :
				bottom(bottom),
				left(left),
				top(top),
				right(right)
			{
			}

			padding_t(T all) :
				padding_t(all, all, all, all)
			{
			}

			T vertical() const
			{
				return bottom + top;
			}

			T horizontal() const
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
		};
	};

	typedef details::padding_t<uint8_t>		padding_u8_t;
	typedef details::padding_t<uint16_t>	padding_u16_t;
	typedef details::padding_t<uint32_t>	padding_u32_t;
	typedef details::padding_t<uint64_t>	padding_u64_t;
	typedef details::padding_t<float32_t>	padding_f32_t;
	typedef details::padding_t<float64_t>	padding_f64_t;
	typedef padding_f32_t padding_t;

	template<typename T, typename U>
	mandala::details::aabb2_t<T> operator-(const mandala::details::aabb2_t<T>& aabb, const details::padding_t<U>& padding)
	{
		return mandala::details::aabb2_t<T>(glm::detail::tvec2<T>(aabb.min.x + padding.left, aabb.min.y + padding.bottom), glm::detail::tvec2<T>(aabb.max.x - padding.right, aabb.max.y - padding.top));
	}

	template<typename T, typename U>
	void operator-=(mandala::details::aabb2_t<T>& aabb, const details::padding_t<U>& padding)
	{
		aabb.min.x += padding.left;
		aabb.min.y += padding.bottom;
		aabb.max.x -= padding.right;
		aabb.max.y -= padding.top;
	}

	template<typename T, typename U>
	mandala::details::aabb2_t<T> operator+(const mandala::details::aabb2_t<T>& aabb, const details::padding_t<U>& padding)
	{
		return mandala::details::aabb2_t<T>({ aabb.min.x - padding.left, aabb.min.y - padding.bottom }, { aabb.max.x + padding.right, aabb.max.y + padding.top });
	}

	template<typename T, typename U>
	void operator+=(mandala::details::aabb2_t<T>& aabb, const details::padding_t<U>& padding)
	{
		aabb.min.x -= padding.left;
		aabb.min.y -= padding.bottom;
		aabb.max.x += padding.right;
		aabb.max.y += padding.top;
	}
};