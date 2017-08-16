#ifndef IAITO_SPRITE_SHEET_HPP
#define IAITO_SPRITE_SHEET_HPP

#include <string>
#include <vector>
#include <map>

#include "iaito/core/Resource.hpp"
#include "iaito/utility/OpenGL.hpp"

namespace iaito
{
	class SpriteSheet: public Resource
	{
		public:
			SpriteSheet();
			
			void Load(const std::string& filename);
			void Unload();

			void CreateTexture();
			void DestroyTexture();

			GLuint GetTexture();
			unsigned int GetWidth();
			unsigned int GetHeight();
			unsigned int GetSet(const std::string& name);

			unsigned int GetCount(unsigned int set);
			unsigned int GetSizeX(unsigned int set);
			unsigned int GetSizeY(unsigned int set);
			unsigned int GetPivotX(unsigned int set);
			unsigned int GetPivotY(unsigned int set);
			unsigned int GetOffsetX(unsigned int set, unsigned int frame);
			unsigned int GetOffsetY(unsigned int set, unsigned int frame);

		private:
			std::string _filename;
			
			GLuint _texture;

			unsigned int _width;
			unsigned int _height;

			std::map<std::string, unsigned int> _sets;

			std::vector<unsigned int> _count;

			std::vector<unsigned int> _sizesX;
			std::vector<unsigned int> _sizesY;

			std::vector<unsigned int> _pivotsX;
			std::vector<unsigned int> _pivotsY;
			
			std::map<unsigned int, std::vector<unsigned int> > _framesX;
			std::map<unsigned int, std::vector<unsigned int> > _framesY;
	};
}

#endif // IAITO_SPRITE_SHEET_HPP