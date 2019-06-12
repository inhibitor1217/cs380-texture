#pragma once

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <string>

namespace Engine
{
	class TextureLoader
	{
	private:
		int _unit;
		GLuint _id;
	public:
		TextureLoader(int unit, const char* path);
		TextureLoader(int unit, std::string path_px, std::string path_nx, std::string path_py, std::string path_ny, std::string path_pz, std::string path_nz);

		GLuint GetID();
		int GetUnit();
	};
}
