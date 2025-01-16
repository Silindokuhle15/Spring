#pragma once
#include "Mesh.h"
#include <algorithm>
#include "PhysicsState.h"
template <typename T>
class Collider : public Component
{
public:
	//glm::vec3 m_Position;
	virtual bool Intersect(const Collider<T>& other);

	glm::vec3 m_Pos;
	Collider() = default;
	~Collider() = default;
};

struct SphereCollider
{
	std::vector<double> m_xAxis;
	std::vector<double> m_yAxis;
	std::vector<double> m_zAxis;

	double m_Radius;
	glm::vec3 m_Pos;

	//void OnInit() override {}
	//void OnUpdate(TimeStep ts) override {}
};

struct GenericCollider : public Mesh
{
public:

	std::vector<double> m_xAxis;
	std::vector<double> m_yAxis;
	std::vector<double> m_zAxis;

	//physics::PhysicsState m_PhysicsState;

	double min_x;
	double min_y;
	double min_z;

	double max_x;
	double max_y;
	double max_z;

	double proj_x;
	double proj_y;
	double proj_z;

	GenericCollider(Mesh& source):
		::Mesh(source)
	{
		if (m_SubMeshes.size() > 0)
		{
			for (auto& sub_mesh : m_SubMeshes)
			{
				for (auto& v : sub_mesh.m_V)
				{
					glm::vec3 pos_new = v.pos;
					m_xAxis.push_back(glm::dot(pos_new, glm::vec3(1.0, 0.0, 0.0)));
					m_yAxis.push_back(glm::dot(pos_new, glm::vec3(0.0, 1.0, 0.0)));
					m_zAxis.push_back(glm::dot(pos_new, glm::vec3(0.0, 0.0, 1.0)));
				}
			}
		}
		for (auto& v : m_V)
		{
			// PROJECT THE POINT ALONG THE AXIS
			glm::vec3 pos_new = v.pos;
			m_xAxis.push_back(glm::dot(pos_new, glm::vec3(1.0, 0.0, 0.0)));
			m_yAxis.push_back(glm::dot(pos_new, glm::vec3(0.0, 1.0, 0.0)));
			m_zAxis.push_back(glm::dot(pos_new, glm::vec3(0.0, 0.0, 1.0)));
		}

		std::sort(m_xAxis.begin(), m_xAxis.end());
		std::sort(m_yAxis.begin(), m_yAxis.end());
		std::sort(m_zAxis.begin(), m_zAxis.end());

		min_x = m_xAxis.at(1);
		min_y = m_yAxis.at(1);
		min_z = m_zAxis.at(1);

		max_x = m_xAxis.at(m_xAxis.size() - 1);
		max_y = m_yAxis.at(m_yAxis.size() - 1);
		max_z = m_zAxis.at(m_zAxis.size() - 1);

		proj_x = max_x - min_x;
		proj_y = max_y - min_y;
		proj_z = max_z - min_z;
	}

	//void OnInit() override {}
	//void OnUpdate(TimeStep ts) override {}
	//
	//GenericCollider() {
	//
	//}
	//GenericCollider(Mesh& source)
	//	//:
	//	//m_Pos{ glm::vec3(0) }
	//	//Mesh::Mesh(source)
	//{
	//}
	
};

template<>
class Collider<SphereCollider> : SphereCollider
{
public:
	template<typename T>
	bool Intersect(const Collider<T>& other);
	Collider(float a, float b, float c, float d)
	{
		m_Radius = a;
		m_Pos = glm::vec3(b, c, d);
	}
};

template<>
class Collider<GenericCollider> : public GenericCollider
{
public:
	template<typename T>
	bool Intersect(Collider<T>& other);
	Collider(Mesh& source):
		::GenericCollider{source}
	{
		m_Positions = source.m_Positions;
	}

	void FindMinMax(physics::PhysicsState& state)
	{
		double a = state.position.x;
		double b = state.position.y;
		double c = state.position.z;

		auto min_vec = state.position + glm::vec3(m_xAxis.at(1), m_yAxis.at(1), m_zAxis.at(1));
		auto max_vec = state.position + glm::vec3(m_xAxis.at(m_xAxis.size() - 1), m_yAxis.at(m_yAxis.size() - 1), m_zAxis.at(m_zAxis.size() - 1));

		min_x = glm::dot(min_vec, glm::vec3(1.0f, 0.0f, 0.0f));
		min_y = glm::dot(min_vec, glm::vec3(0.0f, 1.0f, 0.0f));
		min_z = glm::dot(min_vec, glm::vec3(0.0f, 0.0f, 1.0f));

		//min_x = m_xAxis.at(1) + a;
		//min_y = m_yAxis.at(1) + b;
		//min_z = m_zAxis.at(1) + c;


		max_x = glm::dot(max_vec, glm::vec3(1.0f, 0.0f, 0.0f));
		max_y = glm::dot(max_vec, glm::vec3(0.0f, 1.0f, 0.0f));
		max_z = glm::dot(max_vec, glm::vec3(0.0f, 0.0f, 1.0f));

		//max_x = m_xAxis.at(m_xAxis.size() - 1) + a;
		//max_y = m_yAxis.at(m_yAxis.size() - 1) + b;
		//max_z = m_zAxis.at(m_zAxis.size() - 1) + c;

		proj_x = max_x - min_x;
		proj_y = max_y - min_y;
		proj_z = max_z - min_z;
		
	}
};

/*
template <typename T>
inline bool Collider<SphereCollider>::Intersect(const Collider<T>& other)
{
	return false;
}
*/

template<>
inline bool Collider<GenericCollider>::Intersect(Collider<GenericCollider>& other)
{
	return false;
 }

template<> 
inline bool Collider<GenericCollider>::Intersect(Collider<SphereCollider>& other)
{
}

template<>
inline bool Collider<SphereCollider>::Intersect(const Collider<SphereCollider>& other)
{
	double distance = glm::dot(m_Pos, other.m_Pos);
	double sumOfRadii = m_Radius + other.m_Radius;
	return distance <= sumOfRadii ? true : false;
}