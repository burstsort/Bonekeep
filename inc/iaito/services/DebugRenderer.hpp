#ifndef IAITO_DEBUG_RENDERER_HPP
#define IAITO_DEBUG_RENDERER_HPP

#include <vector>

#include "iaito/core/Service.hpp"
#include "iaito/utility/OpenGL.hpp"

#include "iaito/resources/Shader.hpp"


#define CIRCLE_COMPLEXITY 16

namespace iaito
{
 	class DebugRenderer: public Service
	{
		public:
			DebugRenderer();
			~DebugRenderer();

			void Init();
			void Uninit();

			void SetAspectRatio(float ratio);
			
			void DrawLine(float xa, float ya, float xb, float yb);
			void DrawCircle(float x, float y, float r);
			void DrawRectangle(float x1, float y1, float x2, float y2);

			void SetColor(float r, float g, float b);

			void Render();
			void Clear();

		private:
			DebugRenderer(const DebugRenderer& that);
			DebugRenderer& operator=(const DebugRenderer& that);

			float _ratio;

			Shader _debugShader;
			GLuint _vao;
			GLuint _vbo;

			float _r, _g, _b;
			std::vector<float> _lines;
	};
}

#endif // IAITO_DEBUG_RENDERER_HPP