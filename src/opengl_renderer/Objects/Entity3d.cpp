
#include "Entity3d.h"

namespace GLR {

	Entity3d::Entity3d() : 
		m_Name("default name"),
		m_Color(glm::vec3(1.0f, 1.0f, 1.0f)),
		m_Position(glm::vec3(0.f, 0.f, 0.f)),
		m_Rotation(glm::vec3(0.f, 0.f, 0.f)),
		m_Scale(glm::vec3(1.f, 1.f, 1.f)),
		m_Transforms(glm::mat4(1.f)),
		m_Parent(nullptr),
		m_Orientation(glm::quat(0.0f, 0.0f, 0.0f, 0.0f))
	{
		m_ID = RNDGenerator::GetInstance()->GenerateEntityID();
	}

	Entity3d::Entity3d(std::string name) : 
		m_Name(name),
		m_Color(glm::vec3(1.0f, 1.0f, 1.0f)),
		m_Position(glm::vec3(0.f, 0.f, 0.f)),
		m_Rotation(glm::vec3(0.f, 0.f, 0.f)),
		m_Scale(glm::vec3(1.f, 1.f, 1.f)),
		m_Transforms(glm::mat4(1.f)),
		m_Parent(nullptr),
		m_Orientation(glm::quat(0.0f, 0.0f, 0.0f, 0.0f))
	{
		m_ID = RNDGenerator::GetInstance()->GenerateEntityID();
	}

	Entity3d::Entity3d(const Entity3d &other)
	{

		m_ID = RNDGenerator::GetInstance()->GenerateEntityID();
		m_Name = other.m_Name;
		m_Color = other.m_Color;
		m_Position = other.m_Position;
		m_Rotation = other.m_Rotation;
		m_Scale = other.m_Scale;
		m_Transforms = other.m_Transforms;
		m_Parent = other.m_Parent;
		m_Orientation = other.m_Orientation;

	}

	Entity3d& Entity3d::operator=(const Entity3d &other)
	{
		m_ID = RNDGenerator::GetInstance()->GenerateEntityID();
		m_Name = other.m_Name;
		m_Color = other.m_Color;
		m_Position = other.m_Position;
		m_Rotation = other.m_Rotation;
		m_Scale = other.m_Scale;
		m_Transforms = other.m_Transforms;
		m_Parent = other.m_Parent;
		m_Orientation = other.m_Orientation;

		return *this;
	}

	void Entity3d::ApplyTransforms(TRANSFORM_ORDER order)
	{
		glm::mat4 matrix = glm::mat4(1.0f);

		switch (order)
		{
		case GLR::TRANSFORM_ORDER::SRT:

			// translate
			matrix = glm::translate(matrix, m_Position);

			// rotate
			matrix = glm::rotate(matrix, glm::radians(m_Rotation.z), glm::vec3(0.f, 0.f, 1.f));
			matrix = glm::rotate(matrix, glm::radians(m_Rotation.y), glm::vec3(0.f, 1.f, 0.f));
			matrix = glm::rotate(matrix, glm::radians(m_Rotation.x), glm::vec3(1.f, 0.f, 0.f));

			
			// scale
			matrix = glm::scale(matrix, m_Scale);

			break;
		case GLR::TRANSFORM_ORDER::STR:
			break;
		case GLR::TRANSFORM_ORDER::TRS:
			break;
		case GLR::TRANSFORM_ORDER::TSR:
			break;
		case GLR::TRANSFORM_ORDER::RTS:
			break;
		case GLR::TRANSFORM_ORDER::RST:
			break;
		default:
			break;
		}

		m_Transforms = matrix;
	}

	void Entity3d::SetScale(glm::vec3 _scale)
	{
		m_Scale = _scale;
		ApplyTransforms();
	}

	void Entity3d::SetScale(float x, float y, float z)
	{
		SetScale(glm::vec3(x, y, z));
	}

    void Entity3d::SetScale(float scale)
    {
		SetScale(glm::vec3(scale));
    }

    void Entity3d::SetPosition(float x, float y, float z)
	{
		SetPosition(glm::vec3(x, y, z));
	}
	
	void Entity3d::SetPosition(glm::vec3 _pos)
	{
		m_Position = _pos;
		ApplyTransforms();
	}

	void Entity3d::SetRotation(glm::vec3 _rot)
	{
		m_Rotation = _rot;
		ApplyTransforms();
	}

	void Entity3d::SetRotation(float x, float y, float z)
	{
		SetRotation(glm::vec3(x, y, z));
	}

	std::vector<Entity3d *> Entity3d::GetParents()
	{
		std::vector<Entity3d*> parents;
		if(HasParent())
		{
			Entity3d * cur = static_cast<Entity3d *>(this);
			while(cur != nullptr)
			{
				Entity3d * p = cur->m_Parent;

				if( p == nullptr)
				{
					return parents;
				}else{
					parents.push_back(p);
					cur = p;
				}
			}
		}        
		return parents;
	}

	glm::mat4 Entity3d::ApplyParentTransforms()
	{
		ApplyTransforms();
		
		auto parents = GetParents();
		auto matrix = m_Transforms;
		for (const auto& entity : parents)
		{
			matrix = entity->m_Transforms * matrix;
		}
		return matrix;
	}

	glm::vec3 Entity3d::GetWorldPosition()
	{
		auto matrix = ApplyParentTransforms();
		auto pos = glm::vec3(matrix * glm::vec4(m_Position, 1.0f));

		return pos;      
	}
}