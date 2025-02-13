#include "OpenGLShader.h"
namespace GLR {


	Shader::Shader()
	{

	}

	Shader::Shader(std::filesystem::path vertex_path, std::filesystem::path fragment_path, std::filesystem::path geometry_path)
	{
		LoadVertexShaderSource(vertex_path);
		LoadFragmentShaderSource(fragment_path);

		
		if( geometry_path.string().length() > 0)
		{
			LoadGeometryShaderSource(geometry_path);
		} 

		CreateShader();
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ID);
	}
	void Shader::LoadVertexShaderSource(std::filesystem::path filePath)
	{
		// vertexShaderSource = loadShaderFile(filePath);
		vertexShaderSource = Shadinclude::load(filePath.string());

	}

	void Shader::LoadFragmentShaderSource(std::filesystem::path filePath)
	{

		// fragmentShaderSource = loadShaderFile(filePath);
		fragmentShaderSource = Shadinclude::load(filePath.string());
	}

    void Shader::LoadGeometryShaderSource(std::filesystem::path filePath)
    {
		geometryShaderSource = Shadinclude::load(filePath.string());
		// LOG_INFO("\nSource : {}", geometryShaderSource);
    }

    std::string Shader::GetVertexShaderSource()
	{
		return vertexShaderSource;
	}
	
	std::string Shader::GetFragmentShaderSource()
	{
		return fragmentShaderSource;
	}
	
	std::string Shader::GetGeometryShaderSource()
	{
		return geometryShaderSource;
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, (const GLchar**)&src, nullptr);

		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Shader compile failure !!!!!\n";

			switch (type)
			{
			case GL_VERTEX_SHADER:

				std::cout << "in vertex shader" << std::endl;	
				break;
			case GL_FRAGMENT_SHADER:

				std::cout << "in fragment shader" << std::endl;	
				break;
			case GL_GEOMETRY_SHADER:

				std::cout << "in geometry shader" << std::endl;	
				break;
			
			default:
				break;
			}
			std::cout << message << "\n";
		}

		return id;
	}

	unsigned int Shader::CreateShader()
	{
		m_ID = glCreateProgram();

		bool has_geo_source = geometryShaderSource.length() > 0;
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
		unsigned int gs = 0;

		if( has_geo_source) gs = CompileShader(GL_GEOMETRY_SHADER, geometryShaderSource);
		
		if( has_geo_source) {
			glAttachShader(m_ID, gs);
		}
		glAttachShader(m_ID, vs);
		glAttachShader(m_ID, fs);

		glLinkProgram(m_ID);
		glValidateProgram(m_ID);

		if( has_geo_source){
			 glDetachShader(m_ID, gs);
		}
		glDetachShader(m_ID, vs);
		glDetachShader(m_ID, fs);

		if( has_geo_source) {
			glDeleteShader(gs);
		}
		glDeleteShader(vs);
		glDeleteShader(fs);

		return m_ID;
	}

	void Shader::UseProgram()
	{
		// glLinkProgram(m_ID);
		glUseProgram(m_ID);
	}

    std::string loadShaderFile(std::filesystem::path path)
	{
		
		std::ifstream input(path);
		if( input.fail())
		{
			LOG_ERROR("Error loading shader source {}", path.string());
		}
		std::string output = "";
		for (std::string line; std::getline(input, line);)
		{
			output += line;
			output += "\n";
		}

		return output;
	}

}
