#include "Font2dObject.h"
#include <locale>
#include "Algorithms/RNDGenerator.h"
namespace GLR
{
Font2dObject::Font2dObject():Entity3d()
{
	
}

Font2dObject::Font2dObject(std::string name)
	:Entity3d(name)
{
}

Font2dObject::Font2dObject(const Font2dObject& other)
	:Entity3d(other)
{
	m_BoundingBox = other.m_BoundingBox;
	m_Position    = other.m_Position;
	m_Rotation    = other.m_Rotation;
	m_Scale       = other.m_Scale;
	m_Transforms  = other.m_Transforms;
	m_ItemUUID  = other.m_ItemUUID;
	m_MeshBuffer = other.m_MeshBuffer;
}

Font2dObject& Font2dObject::operator=(const Font2dObject &other)
{
	m_BoundingBox = other.m_BoundingBox;
	m_Position    = other.m_Position;
	m_Rotation    = other.m_Rotation;
	m_Scale       = other.m_Scale;
	m_Transforms  = other.m_Transforms;
	m_ItemUUID  = other.m_ItemUUID;
	m_MeshBuffer = other.m_MeshBuffer;

	return *this;	
}

void Font2dObject::InitRenderData()
{
	m_MeshBuffer.InitRenderData();
}

void Font2dObject::BindRenderData()
{
	m_MeshBuffer.BindRenderData();
}

void Font2dObject::Render()
{
	m_MeshBuffer.Render();
}


void Font2dObject::GenerateQuads()
{

	tiny_utf8::utf8_string utf_string = m_Text;
	Mesh mesh;
	float offset_x = 0.0f;
	float offset_y = 0.0f;
	glm::vec2 uv_scale = glm::vec2(1.0f);
	glm::vec2 uv_pos = glm::vec2(0.0f);

	auto atlas_texture = m_Atlas->GetTexture();


	auto code = static_cast<unsigned int>(utf_string[0]);
	
	// auto metrics = m_Atlas->GetGlyphMetrics(code);

	float SPACING = 1.0f;
	

	const auto& metrics = m_Atlas->m_FontGeo.getMetrics();
	

	double fScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
	double x = 0.0;
	double y = 0.0;

	float texelWidth = 1.0f / atlas_texture->GetWidth(); 
	float texelHeight = 1.0f / atlas_texture->GetHeight(); 
	
	for(size_t i=0; i<utf_string.size(); ++i)
	{
		unsigned int text_char = static_cast<unsigned int>(utf_string[i]);

		if( utf_string[i] == static_cast<unsigned int>('\n')){
			x = 0.0f;
			y -= 1.0f;
		}

		auto* glyph = m_Atlas->m_FontGeo.getGlyph(text_char);
		
		if( glyph != nullptr)
		{
			double a_l, a_b, a_r, a_t;
			glyph->getQuadAtlasBounds(a_l, a_b, a_r, a_t);
			glm::vec2 uvMin = glm::vec2((float) a_l, (float)a_b); 
			glm::vec2 uvMax = glm::vec2((float) a_r, (float)a_t); 

			uvMin *= glm::vec2(texelWidth, texelHeight);
			uvMax *= glm::vec2(texelWidth, texelHeight);
			
			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin = glm::vec2((float) pl, (float)pb); 
			glm::vec2 quadMax = glm::vec2((float) pr, (float)pt); 
				
			quadMin *= fScale;
			quadMax *= fScale;
			quadMin += glm::vec2((float)x, (float)y);
			quadMax += glm::vec2((float)x, (float)y);

			if( i < utf_string.size()-1){

				double advance = glyph->getAdvance();
				unsigned int next_char = static_cast<unsigned int>(utf_string[i+1]);

				m_Atlas->m_FontGeo.getAdvance(advance, text_char, next_char);

				float kerningOffset = -0.02f;
				x += fScale * advance + kerningOffset;
				
			}


			auto quad = MeshUtils::MakeQuad();

			float rand_z = RNDGenerator::GetInstance()->Float(-1.0f, 1.0f) * 0.001f; // used to avoid z-ifghting

			quad.GetPoints()[0].position = glm::vec3(quadMin, rand_z);
			quad.GetPoints()[1].position = glm::vec3(quadMax.x, quadMin.y, rand_z);
			quad.GetPoints()[2].position = glm::vec3(quadMin.x, quadMax.y, rand_z);
			quad.GetPoints()[3].position = glm::vec3(quadMax, rand_z);

		
			
			MeshUtils::UVScale(quad, uvMax - uvMin);
			MeshUtils::UVTranslate(quad, uvMin);

			int atlas_width = atlas_texture->GetWidth();
			float scale_x = 1.0f;
			float scale_y = 1.0f;

			float m_x = 1.0f;
			float m_y = 0.0f;

			mesh = MeshUtils::Merge(mesh, quad);
		}
	}

	mesh.ComputeAABB();
	m_BoundingBox = mesh.m_BoundingBox;
	m_MeshBuffer.SetMesh(std::make_shared<Mesh>(mesh));

	m_MeshBuffer.InitRenderData();
}
}
