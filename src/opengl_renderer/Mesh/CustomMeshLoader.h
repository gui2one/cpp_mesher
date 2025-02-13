#ifndef CUSTOMMESHLOADER_H
#define CUSTOMMESHLOADER_H
#pragma once

#include "Mesh/Mesh.h"

namespace GLR
{
	class CustomMeshLoader
{
public:
	~CustomMeshLoader();

	Mesh Load(std::filesystem::path path);
	static CustomMeshLoader* s_Instance; 
	static CustomMeshLoader* GetInstance();
private:

protected:
	CustomMeshLoader();

};
} // namespace GLR


#endif