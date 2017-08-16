#ifndef IAITO_SPRITE_RENDERER_HPP
#define IAITO_SPRITE_RENDERER_HPP

#include <vector>
#include <cmath>

#include "iaito/core/Service.hpp"
#include "iaito/utility/OpenGL.hpp"

#include "iaito/resources/Shader.hpp"
#include "iaito/resources/SpriteSheet.hpp"
#include "iaito/core/ResourceManager.hpp"

namespace iaito
{
	#define SPRITE_BATCH 100

	#define spriteSheetIndex unsigned int

	class SpriteRenderer: public Service
	{
		public:
			SpriteRenderer();
			~SpriteRenderer();

			void Init();
			void Uninit();

			void SetAspectRatio(float ratio);

			spriteSheetIndex BindSpriteSheet(const std::string& filename);
			void UnbindSpriteSheet(spriteSheetIndex index);
			
			void ReloadSpriteSheets();
			void ClearSpriteSheets();

			void ResetSprite();
			void SetSpriteSheet(spriteSheetIndex index);
			void SetSpritePosition(float x, float y, float z);
			void SetSpriteTransform(float scaleX, float scaleY, float angle);
			void SetSpriteColor(unsigned char red, unsigned char green, unsigned char blue);
			void SetSpriteAlpha(unsigned char alpha);
			void DrawSprite(unsigned int set, unsigned int frame);
			void Render();

			void Clear();

		private:
			SpriteRenderer(const SpriteRenderer& that);
			SpriteRenderer& operator=(const SpriteRenderer& that);

			float _ratio;

			ResourceManager<SpriteSheet> _sprMgr;

			Shader _spriteShader;
			GLuint _vao;
			GLuint _quad;

			unsigned short _queued;

			spriteSheetIndex _sheet;
			float _transform[6];
			unsigned char _color[4];

			GLuint _frameBuffer;
			GLuint _transformBuffer;
			GLuint _colorBuffer;

			std::vector<unsigned int> _frameData;
			std::vector<float> _transformData;
			std::vector<unsigned char> _colorData;

	};
}

#endif // IAITO_SPRITE_RENDERER_HPP