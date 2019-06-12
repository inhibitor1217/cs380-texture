#include <Texture.hpp>

#include <SOIL/SOIL.h>
#include <iostream>
#include <vector>

namespace Engine
{
	TextureLoader::TextureLoader(int unit, const char* path)
	{
		_unit = unit;
		glGenTextures(1, &_id);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, _id);

		std::cout << "Loading " << path << "..." << std::endl;

		int width, height, channel;
		unsigned char* image = SOIL_load_image(path, &width, &height, &channel, SOIL_LOAD_AUTO);
		if (image == 0)
		{
			std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
			return;
		}
		
		int format;
		switch (channel)
		{
		case(3):
			format = GL_RGB;
			break;
		case(4):
			format = GL_RGBA;
			break;
		default:
			format = GL_RGB;
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		//Define in App to change parameters freely
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	TextureLoader::TextureLoader(int unit, std::string path_px, std::string path_nx, std::string path_py, std::string path_ny, std::string path_pz, std::string path_nz)
	{
		std::vector<const char*> paths;
		paths.push_back(path_px.c_str());
		paths.push_back(path_nx.c_str());
		paths.push_back(path_py.c_str());
		paths.push_back(path_ny.c_str());
		paths.push_back(path_pz.c_str());
		paths.push_back(path_nz.c_str());

		_unit = unit;
		glGenTextures(1, &_id);
		glActiveTexture(GL_TEXTURE0+ _unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

		for (int i = 0; i < 6; i++)
		{
			std::cout << "Loading " << paths[i] << "..." << std::endl;

			int width, height, channel;
			unsigned char* image = SOIL_load_image(paths[i], &width, &height, &channel, SOIL_LOAD_AUTO);
			if (image == 0)
			{
				std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
				return;
			}

			int format;
			switch (channel)
			{
			case(3):
				format = GL_RGB;
				break;
			case(4):
				format = GL_RGBA;
				break;
			default:
				format = GL_RGB;
				break;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}

		//Define in App to change parameters freely
		/*glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/

	}

	GLuint TextureLoader::GetID()
	{
		return _id;
	}
	
	int TextureLoader::GetUnit()
	{
		return _unit;
	}
}