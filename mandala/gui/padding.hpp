#pragma once

#include "../types.hpp"
#include "../aabb.hpp"

namespace mandala
{
    namespace gui
    {
		namespace details
		{
			template<typename T>
			struct __padding_t__
			{
				typedef __padding_t__<T> type;
				typedef glm::detail::tvec2<T> size_type;

				T bottom = 0;
				T left = 0;
				T top = 0;
				T right = 0;

				__padding_t__()
				{
				}

				__padding_t__(T bottom, T left, T top, T right) :
					bottom(bottom),
					left(left),
					top(top),
					right(right)
				{
				}

				__padding_t__(T all) :
					__padding_t__(all, all, all, all)
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
				type operator+(__padding_t__<U>& rhs) const
				{
					auto sum = *this;
					sum += rhs;
					return sum;
				}

				template<typename U>
				type& operator+=(__padding_t__<U>& rhs)
				{
					bottom += rhs.bottom;
					left += rhs.left;
					right += rhs.right;
					top += rhs.top;

					return *this;
				}

				template<typename U>
				type operator-(__padding_t__<U>& rhs) const
				{
					auto sum = *this;
					sum -= rhs;
					return sum;
				}

				template<typename U>
				type& operator-=(__padding_t__<U>& rhs)
				{
					bottom -= rhs.bottom;
					left -= rhs.left;
					right -= rhs.right;
					top -= rhs.top;

					return *this;
				}
			};
		};

		typedef details::__padding_t__<uint8_t>		padding_u8_t;
		typedef details::__padding_t__<uint16_t>	padding_u16_t;
		typedef details::__padding_t__<uint32_t>	padding_u32_t;
		typedef details::__padding_t__<uint64_t>	padding_u64_t;
		typedef details::__padding_t__<float32_t>	padding_f32_t;
		typedef details::__padding_t__<float64_t>	padding_f64_t;
		typedef padding_f32_t padding_t;

		template<typename T, typename U>
		mandala::details::aabb2_t<T> operator-(const mandala::details::aabb2_t<T>& aabb, const details::__padding_t__<U>& padding)
		{
			return mandala::details::aabb2_t<T>(glm::detail::tvec2<T>(aabb.min.x + padding.left, aabb.min.y + padding.bottom), glm::detail::tvec2<T>(aabb.max.x - padding.right, aabb.max.y - padding.top));
		}

		template<typename T, typename U>
		void operator-=(mandala::details::aabb2_t<T>& aabb, const details::__padding_t__<U>& padding)
		{
			aabb.min.x += padding.left;
			aabb.min.y += padding.bottom;
			aabb.max.x -= padding.right;
			aabb.max.y -= padding.top;
		}

		template<typename T, typename U>
		mandala::details::aabb2_t<T> operator+(const mandala::details::aabb2_t<T>& aabb, const details::__padding_t__<U>& padding)
		{
			return mandala::details::aabb2_t<T>({ aabb.min.x - padding.left, aabb.min.y - padding.bottom }, { aabb.max.x + padding.right, aabb.max.y + padding.top });
		}

		template<typename T, typename U>
		void operator+=(mandala::details::aabb2_t<T>& aabb, const details::__padding_t__<U>& padding)
		{
			aabb.min.x -= padding.left;
			aabb.min.y -= padding.bottom;
			aabb.max.x += padding.right;
			aabb.max.y += padding.top;
		}
    };
};
