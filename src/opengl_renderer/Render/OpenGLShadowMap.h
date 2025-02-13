#ifndef OPENGLSHADOWMAP_H
#define OPENGLSHADOWMAP_H
#pragma once


namespace GLR {

	class ShadowMap
	{
	public:
		ShadowMap();
		~ShadowMap();

		void Init();
		void BindFBO();

		uint32_t GetDepthMap(){ return m_DepthMap; }

		uint32_t GetResolution(){ return m_Resolution; }


		void Bind(unsigned int slot);
		void Unbind(unsigned int slot);        
	private:
		uint32_t m_DepthFBO = 0;
		uint32_t m_DepthMap = 0;

		uint32_t m_Resolution = 4096;

	};

} /* end namespace GLR*/
#endif