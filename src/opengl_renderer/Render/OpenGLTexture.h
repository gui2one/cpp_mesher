#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H
#pragma once 


namespace GLR{
class Texture
{
public:

	Texture();

	virtual ~Texture();
	void Load(std::string path);

	void SetData(int width, int height, unsigned char* buffer);

	unsigned int GetID() { return m_ID; };
	int GetWidth() { return m_Width; };
	int GetHeight() { return m_Height; };
	int GetBPP() { return m_BitsPerPixel; };
	void Bind(unsigned int slot);
	void Unbind(unsigned int slot);

private:
	unsigned int m_ID;
	int m_Width, m_Height, m_BitsPerPixel;
};

}
#endif /* OPENGLTEXTURE_H */