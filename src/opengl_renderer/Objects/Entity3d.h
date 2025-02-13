#ifndef ENTITY3D_H
#define ENTITY3D_H 
#pragma once

#include "Algorithms/RNDGenerator.h"
namespace GLR {

	enum class TRANSFORM_ORDER {
		SRT,
		STR,
		TRS,
		TSR,
		RTS,
		RST
	};
class Entity3d
{

public:
	uint32_t m_ID;
	bool m_Selected = false;
	std::string m_Name;
	glm::vec3 m_Color;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_Transforms;

	/* keeping a raw pointer for parent is a bit dangerous */
	Entity3d* m_Parent = nullptr;

	glm::quat m_Orientation;
private:

public:
	Entity3d();
	Entity3d(std::string name);
	Entity3d(const Entity3d& other);
	Entity3d& operator=(const Entity3d& other);

	virtual ~Entity3d() {};
	void ApplyTransforms(TRANSFORM_ORDER order = TRANSFORM_ORDER::SRT);

	void SetScale(glm::vec3 _scale);
	void SetScale(float x, float y, float z);
	void SetScale(float scale);
	
	void SetPosition(glm::vec3 _pos);
	void SetPosition(float x, float y, float z);
	
	void SetRotation(glm::vec3 _rot);
	void SetRotation(float x, float y, float z);

	inline bool HasParent(){ return m_Parent != nullptr; }
	inline Entity3d* GetParent(){ return m_Parent; }
	inline const Entity3d* GetParent() const { return m_Parent; }

	std::vector<Entity3d*> GetParents();

	glm::mat4 ApplyParentTransforms();
	inline void SetTranforms(glm::mat4 matrix){ m_Transforms = matrix; }

	inline void SetParent(Entity3d* parent){ m_Parent = parent;  }

	inline std::string& GetName() { return m_Name; }
	inline const std::string& GetName() const { return m_Name; }

	inline void SetName(std::string name) { m_Name = name; }


	glm::vec3 GetWorldPosition();

	inline glm::vec3& GetPosition(){ return m_Position; }
	inline const glm::vec3& GetPosition() const { return m_Position; }

	inline glm::vec3& GetRotation(){ return m_Rotation; }
	inline const glm::vec3& GetRotation() const { return m_Rotation; }

	inline glm::vec3& GetScale(){ return m_Scale; }
	inline const glm::vec3& GetScale() const { return m_Scale; }

private:

};
}

#endif /* ENTITY3D_H */