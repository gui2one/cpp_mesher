#include "ShaderManager.h"


namespace GLR {

	ShaderManager* ShaderManager::s_Instance = nullptr;
	ShaderManager* ShaderManager::GetInstance()
	{
		if( s_Instance == nullptr)
		{
			s_Instance = new ShaderManager();
			s_Instance->Init();
		}

		return s_Instance;
	}


	ShaderManager::ShaderManager()
	{
	
	}

	ShaderManager::~ShaderManager()
	{

	}

	void ShaderManager::Init()
	{
		fontShader           = std::make_shared<Shader>("Resources/Shaders/font_shader.vert", "Resources/Shaders/font_shader.frag");
		msdfFontShader       = std::make_shared<Shader>("Resources/Shaders/msdf_font_shader.vert", "Resources/Shaders/msdf_font_shader.frag");
		ui_shader            = std::make_shared<Shader>("Resources/Shaders/ui_basic_shader.vert", "Resources/Shaders/ui_basic_shader.frag"); 

		basic_shader         = std::make_shared<Shader>("Resources/Shaders/basic_shader.vert", "Resources/Shaders/basic_shader.frag");
		pbr_shader           = std::make_shared<Shader>("Resources/Shaders/irradiance/2.2.2.pbr.vs", "Resources/Shaders/irradiance/2.2.2.pbr.fs");
		
		star_shader          = std::make_shared<Shader>("Resources/Shaders/star_shader.vert", "Resources/Shaders/star_shader.frag");
		cell_shader          = std::make_shared<Shader>("Resources/Shaders/cell_shader.vert", "Resources/Shaders/cell_shader.frag");
		screen_shader        = std::make_shared<Shader>("Resources/Shaders/frame_buffer_shader.vert", "Resources/Shaders/frame_buffer_shader.frag");
		line_shader          = std::make_shared<Shader>("Resources/Shaders/meshline_shader.vert", "Resources/Shaders/meshline_shader.frag");
		unlit_shader         = std::make_shared<Shader>("Resources/Shaders/unlit_shader.vert", "Resources/Shaders/unlit_shader.frag");

		depth_shader         = std::make_shared<Shader>("Resources/Shaders/simple_depth_shader.vert", "Resources/Shaders/simple_depth_shader.frag");
		cube_depth_shader    = std::make_shared<Shader>("Resources/Shaders/cube_depth_shader.vert", "Resources/Shaders/cube_depth_shader.frag", "Resources/Shaders/cube_depth_shader.geo");
		
		wire_shader          = std::make_shared<Shader>("Resources/Shaders/wire_shader.vert", "Resources/Shaders/wire_shader.frag");
		image_display_shader = std::make_shared<Shader>("Resources/Shaders/image_display_shader.vert", "Resources/Shaders/image_display_shader.frag");    

		skybox_shader        = std::make_shared<Shader>("Resources/Shaders/skybox_shader.vert", "Resources/Shaders/skybox_shader.frag");        
		
		outline_shader       = std::make_shared<Shader>("Resources/Shaders/outline_shader.vert", "Resources/Shaders/outline_shader.frag");        
	
	
		// textures

		white_texture = std::make_shared<Texture>();
		uint8_t buffer[4] = {255, 255, 255, 255};
		white_texture->SetData(1,1, buffer);

		very_light_gray_texture = std::make_shared<Texture>();
		uint8_t buffer_very_light_gray[4] = {220, 220, 220, 255};
		very_light_gray_texture->SetData(1,1, buffer_very_light_gray);

		light_gray_texture = std::make_shared<Texture>();
		uint8_t buffer_light_gray[4] = {190, 190, 190, 255};
		light_gray_texture->SetData(1,1, buffer_light_gray);


		gray_texture = std::make_shared<Texture>();
		uint8_t buffer_gray[4] = {128, 128, 128, 255};
		gray_texture->SetData(1,1, buffer_gray);



		dark_gray_texture = std::make_shared<Texture>();
		uint8_t buffer_dark_gray[4] = {50, 50, 50, 255};
		dark_gray_texture->SetData(1,1, buffer_dark_gray);


		very_dark_gray_texture = std::make_shared<Texture>();
		uint8_t buffer_very_dark_gray[4] = {10, 10, 10, 255};
		very_dark_gray_texture->SetData(1,1, buffer_very_dark_gray);


		black_texture = std::make_shared<Texture>();
		uint8_t buffer_black[4] = {0, 0, 0, 255};
		black_texture->SetData(1,1, buffer_black);

		red_texture = std::make_shared<Texture>();
		uint8_t buffer_red[4] = {255, 0, 0, 255};
		red_texture->SetData(1,1, buffer_red);

	}
} /* end namespace GLR*/