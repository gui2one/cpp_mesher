#ifndef OPENGL_CUBE_SHADOWMAP_H
#define OPENGL_CUBE_SHADOWMAP_H
#pragma once


namespace GLR {

	class CubeShadowMap
	{
	public:
		CubeShadowMap();
		~CubeShadowMap();

		void Init();
		void BindFBO();

		uint32_t GetDepthMap(){ return m_DepthMap; }

		uint32_t GetResolution(){ return m_Resolution; }


		void Bind(unsigned int slot);
		void Unbind(unsigned int slot);        
	private:
		uint32_t m_DepthFBO = 0;
		uint32_t m_DepthMap = 0;
		uint32_t m_CubeMap = 0;

		uint32_t m_Resolution = 2048;

	};

} /* end namespace GLR*/
#endif