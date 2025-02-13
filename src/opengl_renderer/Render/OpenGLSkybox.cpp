#include "OpenGLSkybox.h"
#include "stb_image.h"

namespace GLR{

	Skybox::Skybox() {

	}

	void Skybox::Init(
		std::filesystem::path right, 
		std::filesystem::path left, 
		std::filesystem::path top,
		std::filesystem::path bottom,
		std::filesystem::path front,
		std::filesystem::path back
	){
		
		
		// init shader
		m_Shader = ShaderManager::GetInstance()->skybox_shader;

		//init openGL stuff
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		std::vector<std::filesystem::path> paths = { 
			right, left, 
			top, bottom, 
			front, back
		};

		for(size_t i=0; i < paths.size(); i++){
			int width, height, nrChannels;
			unsigned char * data;
			stbi_set_flip_vertically_on_load(0);    
			data = stbi_load(paths[i].string().c_str(), &width, &height, &nrChannels, 0);

			if(data){
	
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + (GLenum)i, 
					0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				delete data;
				//stbi_image_free(data);

			}else{
				LOG_ERROR("Cubemap tex failed to load at path: {}", paths[i].string());
				
				// delete data; /* deleting here would crash I think .... no ?*/
				
			}
		}


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 	


		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		m_Vbo = 0;
		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), skyboxVertices,GL_STATIC_DRAW);

		m_Vao = 0;
		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);
		glEnableVertexAttribArray(0);


		glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3* sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	}


	void Skybox::Bind() {

		// glBindTextureUnit(0, m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}
	void Skybox::Draw(std::shared_ptr<Camera> camera){
		glDepthMask(GL_FALSE);


		glm::mat4 view = glm::mat4(1.0f);
		glm::vec3 up_vector(0.f,1.f,0.f);
		view *= glm::lookAt(
			camera->m_Position,
			camera->target_position,
			glm::normalize(up_vector)
		);

		// remove position from matrix
		view = glm::mat4(glm::mat3(view)); 

		m_Shader->UseProgram();
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetID(), "u_projection"), 1, GL_FALSE, glm::value_ptr(camera->m_Projection));
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetID(), "u_view"), 1, GL_FALSE, glm::value_ptr(view));
			
		// ... set view and projection matrix

		glBindVertexArray(m_Vao);
		glEnableVertexAttribArray(0);

		// glBindTextureUnit(0, m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


	}
}