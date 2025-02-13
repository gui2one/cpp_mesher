#ifndef OPENGLHDRTEXTURE_H
#define OPENGLHDRTEXTURE_H
#pragma once 

namespace GLR {
	class HDRTexture
	{
	public:
		HDRTexture();

		virtual ~HDRTexture();
		void Load(std::filesystem::path path);
		void SetData(std::vector<float> data, uint32_t width, uint32_t height);
		unsigned int& GetID() { return m_ID; };
		int GetWidth() { return m_Width; };
		int GetHeight() { return m_Height; };
		int GetNumComponents() { return m_NumComponents; };
		void Bind(unsigned int slot);
		void Unbind(unsigned int slot);

	private:
		unsigned int m_ID;
		int m_Width, m_Height, m_NumComponents;
	};

}
#endif /* OPENGLHDRTEXTURE_H */