#include "iaito/services/DebugRenderer.hpp"

#include <cmath>

namespace iaito
{
	DebugRenderer::DebugRenderer() : _ratio(1.78f), _r(0.5f), _g(0.5f), _b(0.5f)
	{
		Init();
	}

	DebugRenderer::~DebugRenderer()
	{
		Uninit();
	}

	void DebugRenderer::Init()
	{
		Uninit();

		_debugShader.Load("shaders/debug");

		glCheck(glGenVertexArrays(1, &_vao));
		glCheck(glBindVertexArray(_vao));

		glCheck(glGenBuffers(1, &_vbo));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
		glCheck(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
		glCheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));
		glCheck(glEnableVertexAttribArray(0));
		glCheck(glEnableVertexAttribArray(1));

		glCheck(glClearColor(0.2f, 0.1f, 0.1f, 1.0f));

		glCheck(glEnable(GL_BLEND));
		glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		glCheck(glClearColor(0.1f, 0.1f, 0.2f, 1.0f));

		glCheck(glBindVertexArray(0));
	}

	void DebugRenderer::Uninit()
	{
		_debugShader.Unload();

		if (_vbo)
			glCheck(glDeleteBuffers(1, &_vbo));

		if (_vao)
			glCheck(glDeleteVertexArrays(1, &_vao));
	}

	void DebugRenderer::SetAspectRatio(float ratio)
	{
		_ratio = ratio;
	}

	void DebugRenderer::DrawLine(float ax, float ay, float bx, float by)
	{
		_lines.push_back(ax);
		_lines.push_back(ay);
		_lines.push_back(_r);
		_lines.push_back(_g);
		_lines.push_back(_b);
		_lines.push_back(bx);
		_lines.push_back(by);
		_lines.push_back(_r);
		_lines.push_back(_g);
		_lines.push_back(_b);
	}

	void DebugRenderer::DrawCircle(float x, float y, float r)
	{
		float step = 6.2832f / CIRCLE_COMPLEXITY;
		for (unsigned int i = 0; i < CIRCLE_COMPLEXITY; ++i)
		{
			_lines.push_back(x + r * sin(step * i));
			_lines.push_back(y + r * cos(step * i) * _ratio);
			_lines.push_back(_r);
			_lines.push_back(_g);
			_lines.push_back(_b);
			_lines.push_back(x + r * sin(step * (i + 1)));
			_lines.push_back(y + r * cos(step * (i + 1)) * _ratio);
			_lines.push_back(_r);
			_lines.push_back(_g);
			_lines.push_back(_b);
		}
	}

	void DebugRenderer::DrawRectangle(float x1, float y1, float x2, float y2)
	{
		DrawLine(x1, y1, x2, y1);
		DrawLine(x2, y1, x2, y2);
		DrawLine(x2, y2, x1, y2);
		DrawLine(x1, y2, x1, y1);
	}

	void DebugRenderer::SetColor(float r, float g, float b)
	{
		_r = r;
		_g = g;
		_b = b;
	}

	void DebugRenderer::Render()
	{
		glCheck(glBindVertexArray(_vao));

		glCheck(glDisable(GL_DEPTH_TEST));

		glCheck(glUseProgram(_debugShader.GetProgram()));

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
		glCheck(glBufferData(GL_ARRAY_BUFFER, _lines.size() * sizeof(float), &_lines[0], GL_STREAM_DRAW));

		glCheck(glDrawArrays(GL_LINES, 0, _lines.size() * 0.5f));

		_lines.clear();
	}

	void DebugRenderer::Clear()
	{
		glCheck(glClear(GL_COLOR_BUFFER_BIT));
	};
}