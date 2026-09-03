#ifndef _CURVE_H_
#define _CURVE_H_
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace primitives
{
	template<typename T, uint32_t Degree>
	class Curve
	{
	private:
		std::vector<T> m_ControlPoints;
		std::vector<float> m_Knots;
	private:
		std::size_t findSpan(std::size_t n, float u);
	public:
		explicit Curve(std::vector<T> control_points, std::vector<float> knots);
		T evaluate(float u) const;
		T operator()(float u) const;

	};

	template<typename T, uint32_t Degree>
	inline T Curve<T, Degree>::evaluate(float u) const
	{
		const float first = m_Knots[Degree];
		const float last = m_Knots[m_ControlPoints.size() - 1];
		const float epsilon = 1e-6f;
		if (u < first - epsilon || u >= last + epsilon)
		{
			throw std::runtime_error("Parameter outside of domain !!!");
		}
		u = std::clamp(u, first, last);
		const std::size_t n = m_ControlPoints.size() - 1;
		const std::size_t k = findSpan(n, u);
		std::array<T, Degree + 1> value;
		for (std::size_t j = 0; j <= Degree; j++)
		{
			value[j] = m_ControlPoints[k - Degree + j];
		}
		for (std::size_t r = 1; r <= Degree; ++r)
		{
			for (std::size_t j = Degree + 1; j-- > r;)
			{
				std::size_t i = k - Degree + j;
				const float denom = (m_Knots[i + Degree - r + 1] - m_Knots[i]);
				float alpha = 0.0f;
				if (std::abs(denom) >= epsilon)
				{
					alpha = (u - m_Knots[i]) / denom;
					alpha = std::clamp(alpha, 0.0f, 1.0f);
				}
				value[j] = value[j - 1] * (1.0f - alpha) + value[j] * alpha;
			}
		}
		return value[Degree];
	}

	template<typename T, uint32_t Degree>
	inline T Curve<T, Degree>::operator()(float u) const
	{
		return evaluate(u);
	}

	template<typename T, uint32_t Degree>
	inline std::size_t Curve<T, Degree>::findSpan(std::size_t n, float u)
	{
		if (u >= m_Knots[n + 1])
		{
			return n;
		}
		std::size_t low = Degree;
		std::size_t high = n + 1;
		std::size_t mid = (low + high) / 2;
		while (u < m_Knots[mid] || u >= m_Knots[mid + 1])
		{
			if (u < m_Knots[mid])
			{
				high = mid;
			}
			else
			{
				low = mid + 1;
			}
			mid = (low + high) / 2;
		}
		return mid;
	}

	template<typename T, uint32_t Degree>
	inline Curve<T, Degree>::Curve(std::vector<T> control_points, std::vector<float> knots) :
		m_ControlPoints{ std::move(control_points) },
		m_Knots{ std::move{knots} },
		m_Degree{ Degree }
	{
		if (m_ControlPoints.size() < Degree + 1)
		{
			throw std::runtime_error("ControlPoints.size() < Degree + 1");
		}
		if (m_Knots.size() != m_ControlPoints.size() + Degree + 1)
		{
			throw std::runtime_error("Knots.size() != ControlPoints.size() + Degree + 1");
		}
		for (auto end = m_Knots.size() - 1; end > 1; end--)
		{
			if (m_Knots[end] < m_Knots[end - 1])
			{
				throw std::runtime_error("Knots[end] < Knots[end - 1]");
			}
		}
		if (m_Knots[Degree] > m_Knots[m_ControlPoints.size()])
		{
			throw std::runtime_error("Knots[Degree] > Knots[ControlPoints.size()]");
		}
	}
}
#endif // !_CURVE_H_