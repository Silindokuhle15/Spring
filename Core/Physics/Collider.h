#pragma once
#include "Mesh.h"
#include <algorithm>
#include "PhysicsState.h"
template <typename T>
class Collider
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

struct GenericCollider
{
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
class Collider<GenericCollider> : public GenericCollider, public Mesh
{
public:

	physics::PhysicsState m_PhysicsState;
	template<typename T>
	bool Intersect(Collider<T>& other);
	Collider(Mesh& source):
		::Mesh{source}
	{
		m_Positions = source.m_Positions;
		double a = m_Transform[3][0];
		double b = m_Transform[3][1];
		double c = m_Transform[3][2];
		//m_Pos = glm::vec3(a, b, c);
		//m_PhysicsState.position = 
		//m_Transform[15];

	}
	Collider(physics::PhysicsState& physics_state, Mesh& source) :
		::Mesh{ source },
		m_PhysicsState{ physics_state }
	{
		m_Positions = source.m_Positions;
	}
	Collider(physics::PhysicsState& physics_state , std::vector<glm::vec3>& vec):
		m_PhysicsState{physics_state}
		//m_Positions{}
	{
		//m_Pos = pos;
		m_Positions = vec;
	}

	void FindMinMax()
	{
		m_xAxis.clear();
		m_yAxis.clear();
		m_zAxis.clear();

		for (auto& pos : m_Positions)
		{
			// PROJECT THE POINT ALONG THE AXIS
			glm::vec3 pos_new = pos + m_PhysicsState.position;
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
	// SEPARATE THE AXIS
	FindMinMax();
	other.FindMinMax();
	// COMPARE THE VALUES
	double proj_sum_x = (proj_x + other.proj_x); 
	double proj_sum_y = (proj_y + other.proj_y); 
	double proj_sum_z = (proj_z + other.proj_z); 

	double max_sum_x = (max_x - other.min_x) >= (other.max_x - min_x) ? (max_x - other.min_x) : (other.max_x - min_x);
	double max_sum_y = (max_y - other.min_y) >= (other.max_y - min_y) ? (max_y - other.min_y) : (other.max_y - min_y);
	double max_sum_z = (max_z - other.min_z) >= (other.max_z - min_z) ? (max_z - other.min_z) : (other.max_z - min_z);

	bool intersect_x = (proj_sum_x >= max_sum_x ) ? true : false;
	bool intersect_y = (proj_sum_y >= max_sum_y ) ? true : false;
	bool intersect_z = (proj_sum_z >= max_sum_z ) ? true : false;

	return (intersect_x && intersect_y && intersect_z);
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