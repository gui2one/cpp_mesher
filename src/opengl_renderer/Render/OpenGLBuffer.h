#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H
#pragma once

#include <iosfwd>
#include <vector>
#include <initializer_list>
#include "spdlog/fmt/ostr.h" /*must be included for spdlog output*/

namespace GLR 
{

enum class ShaderDataType
{
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Int,
	Int2,
	Int3,
	Int4,
	Mat3,
	Mat4,
	Bool

};



struct BufferElement
{

	std::string Name;
	ShaderDataType Type;
	uint32_t Offset;
	uint32_t Size;

	BufferElement(ShaderDataType type, std::string name)
		: Name(name), Type(type), Offset(0), Size(ShaderDataTypeSize(type))
	{
	}

	inline uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:

			return 0;
		case ShaderDataType::Float:
			return sizeof(float) * 1;
		case ShaderDataType::Float2:
			return sizeof(float) * 2;
		case ShaderDataType::Float3:
			return sizeof(float) * 3;
		case ShaderDataType::Float4:
			return sizeof(float) * 4;

		case ShaderDataType::Int:
			return sizeof(int) * 1;
		case ShaderDataType::Int2:
			return sizeof(int) * 2;
		case ShaderDataType::Int3:
			return sizeof(int) * 3;
		case ShaderDataType::Int4:
			return sizeof(int) * 4;

		case ShaderDataType::Mat3:
			return sizeof(float) * 3 * 3;
		case ShaderDataType::Mat4:
			return sizeof(float) * 4 * 4;

		case ShaderDataType::Bool:
			return sizeof(bool);
		}

		return 0;
	}

	inline uint32_t GetNumComponents() const
	{
		switch (Type)
		{
		case ShaderDataType::None:
			return 0;
		case ShaderDataType::Float:
			return 1;
		case ShaderDataType::Float2:
			return 2;
		case ShaderDataType::Float3:
			return 3;
		case ShaderDataType::Float4:
			return 4;
		case ShaderDataType::Int:
			return 1;
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Int3:
			return 3;
		case ShaderDataType::Int4:
			return 4;
		case ShaderDataType::Mat3:
			return 3;
		case ShaderDataType::Mat4:
			return 4;
		case ShaderDataType::Bool:
			return 1;
		default :
			return 0;
		}
		

		return 0;
	}
};

class BufferLayout
{

public:
	BufferLayout(){};
	BufferLayout(std::initializer_list<BufferElement> elements)
		: m_Elements(elements), m_Stride(0)
	{

		ComputeOffsetAndStride();
	}

	inline void ComputeOffsetAndStride()
	{
		m_Stride = 0;
		uint32_t offset = 0;
		for (auto &element : m_Elements)
		{
			element.Offset = offset;         
			offset += element.Size;
			m_Stride += element.Size;
		}
	   
	}

	std::vector<BufferElement> GetElements() { return m_Elements; }
	const std::vector<BufferElement> GetElements() const { return m_Elements; }
	uint_fast16_t GetStride() { return m_Stride; }

	//// iterators
	//std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	//std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

	//std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	//std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	friend std::ostream& operator<<(std::ostream& os, const BufferLayout& layout);

private:
	std::vector<BufferElement> m_Elements;

	uint32_t m_Offset;
	uint32_t m_Stride = 0;
};


inline std::ostream& operator<<(std::ostream& os, const BufferLayout& layout) {

	os << "\t---Layout--- \n" << "Num Elements : " << layout.GetElements().size() << std::endl;
	for (auto& element : layout.GetElements()) {
		os << "element name :" << element.Name << " -- Size :" << element.Size <<std::endl;
	}
	return os;
}

class VertexBuffer
{
public:
	VertexBuffer(float* vertices, uint32_t size);
	~VertexBuffer();

	void bind();
	void unbind();
	void setLayout(const BufferLayout& layout) { m_Layout = layout; }
	BufferLayout& getLayout() { return m_Layout; }
	uint32_t getID() { return m_ID; }

private:
	uint32_t m_ID;
	BufferLayout m_Layout;
};

class IndexBuffer
{
public:
	IndexBuffer(uint32_t* indices, uint32_t size);
	~IndexBuffer();

	void Bind();
	void unbind();

	inline uint32_t getID() { return m_ID; }

private:
	uint32_t m_ID;
};

}
#endif /*OPENGLBUFFER_H*/