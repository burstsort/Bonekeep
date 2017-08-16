#include "iaito/services/SpriteRenderer.hpp"

namespace iaito
{
	SpriteRenderer::SpriteRenderer() : _ratio(1.78f), _vao(0), _quad(0), _queued(0), _frameBuffer(0), _transformBuffer(0), _colorBuffer(0)
	{
		ResetSprite();
		Init();
	}

	SpriteRenderer::~SpriteRenderer()
	{
		Uninit();
	}

	void SpriteRenderer::Init()
	{
		Uninit();

		_spriteShader.Load("shaders/sprite");

		for (unsigned int i = 0; i < _sprMgr.Size(); ++i)
			_sprMgr.Get(i).CreateTexture();

		float quad[] =
		{
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};

		glCheck(glGenVertexArrays(1, &_vao));
		glCheck(glBindVertexArray(_vao));

		glCheck(glGenBuffers(1, &_quad));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, _quad));
		glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW));
		glCheck(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));

		// Create sprite buffers
		glCheck(glGenBuffers(1, &_frameBuffer));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, _frameBuffer));
		glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * 6 * SPRITE_BATCH, NULL, GL_STREAM_DRAW));
		glCheck(glVertexAttribPointer(1, 2, GL_UNSIGNED_INT, GL_FALSE, 6 * sizeof(unsigned int), (void*)0));
		glCheck(glVertexAttribPointer(2, 2, GL_UNSIGNED_INT, GL_FALSE, 6 * sizeof(unsigned int), (void*)(2 * sizeof(unsigned int))));
		glCheck(glVertexAttribPointer(3, 2, GL_UNSIGNED_INT, GL_FALSE, 6 * sizeof(unsigned int), (void*)(4 * sizeof(unsigned int))));

		glCheck(glGenBuffers(1, &_transformBuffer));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, _transformBuffer));
		glCheck(glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float) * SPRITE_BATCH, NULL, GL_STREAM_DRAW));
		glCheck(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
		glCheck(glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
		glCheck(glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float))));

		glCheck(glGenBuffers(1, &_colorBuffer));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer));
		glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * 4 * SPRITE_BATCH, NULL, GL_STREAM_DRAW));
		glCheck(glEnableVertexAttribArray(7));
		glCheck(glVertexAttribPointer(7, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4 * sizeof(unsigned char), (void*)0));

		// Enable Attributes
		glCheck(glEnableVertexAttribArray(0));
		glCheck(glVertexAttribDivisor(0, 0));

		for (unsigned int i = 1; i <= 7; ++i)
		{
			glCheck(glEnableVertexAttribArray(i));
			glCheck(glVertexAttribDivisor(i, 1));
		}

		// Drawing parameters
		glCheck(glEnable(GL_BLEND));
		glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		glCheck(glDepthFunc(GL_LESS));

		glCheck(glClearColor(0.1f, 0.1f, 0.2f, 1.0f));

		glCheck(glBindVertexArray(0));
	}
	
	void SpriteRenderer::Uninit()
	{
		_spriteShader.Unload();

		for (unsigned int i = 0; i < _sprMgr.Size(); ++i)
			_sprMgr.Get(i).DestroyTexture();

		// Delete sprite buffers
		if (_colorBuffer)
			glCheck(glDeleteBuffers(1, &_colorBuffer));
		if (_transformBuffer)
			glCheck(glDeleteBuffers(1, &_transformBuffer));
		if (_frameBuffer)
			glCheck(glDeleteBuffers(1, &_frameBuffer));

		if (_quad)
			glCheck(glDeleteBuffers(1, &_quad));

		if (_vao)
			glCheck(glDeleteVertexArrays(1, &_vao));
	}

	void SpriteRenderer::SetAspectRatio(float ratio)
	{
		_ratio = ratio;
	}

	spriteSheetIndex SpriteRenderer::BindSpriteSheet(const std::string& filename)
	{
		return _sprMgr.Bind(filename);
	}

	void SpriteRenderer::UnbindSpriteSheet(spriteSheetIndex index)
	{
		_sprMgr.Unbind(index);
	}
	
	void SpriteRenderer::ReloadSpriteSheets()
	{
		_sprMgr.Reload();
	}

	void SpriteRenderer::ClearSpriteSheets()
	{
		_sprMgr.Clear();
	}

	void SpriteRenderer::ResetSprite()
	{
		_sheet = -1;

		_transform[0] = 0.0f;
		_transform[1] = 0.0f;
		_transform[2] = 0.0f;
		_transform[3] = 1.0f;
		_transform[4] = 1.0f;
		_transform[5] = 0.0f;

		_color[0] = 255;
		_color[1] = 255;
		_color[2] = 255;
		_color[3] = 255;
	}

	void SpriteRenderer::SetSpriteSheet(spriteSheetIndex index)
	{
		if (_sheet != index)
			Render();

		_sheet = index;
	}

	void SpriteRenderer::SetSpritePosition(float x, float y, float z)
	{
		_transform[0] = x;
		_transform[1] = y;
		_transform[2] = z;
	}

	void SpriteRenderer::SetSpriteTransform(float scaleX, float scaleY, float angle)
	{
		_transform[3] = scaleX;
		_transform[4] = scaleY;
		_transform[5] = angle;
	}
	
	void SpriteRenderer::SetSpriteColor(unsigned char red, unsigned char green, unsigned char blue)
	{
		_color[0] = red;
		_color[1] = green;
		_color[2] = blue;
	}
	
	void SpriteRenderer::SetSpriteAlpha(unsigned char alpha)
	{
		_color[3] = alpha;
	}
	
	void SpriteRenderer::DrawSprite(unsigned int set, unsigned int frame)
	{
		if (_sheet == (unsigned int) -1)
			return;

		if (_queued >= SPRITE_BATCH)
			Render();

		_frameData.push_back(_sprMgr.Get(_sheet).GetOffsetX(set, frame));
		_frameData.push_back(_sprMgr.Get(_sheet).GetOffsetY(set, frame));
		_frameData.push_back(_sprMgr.Get(_sheet).GetPivotX(set));
		_frameData.push_back(_sprMgr.Get(_sheet).GetPivotY(set));
		_frameData.push_back(_sprMgr.Get(_sheet).GetSizeX(set));
		_frameData.push_back(_sprMgr.Get(_sheet).GetSizeY(set));

		for (unsigned int i = 0; i < 6; ++i)
			_transformData.push_back(_transform[i]);

		for (unsigned int i = 0; i < 4; ++i)
			_colorData.push_back(_color[i]);

		_queued++;
	}

	void SpriteRenderer::Render()
	{
		if (_sheet == (unsigned int) -1 || !_queued)
			return;

		glCheck(glBindVertexArray(_vao));

		glCheck(glEnable(GL_DEPTH_TEST));

		// Setup shader
		glCheck(glUseProgram(_spriteShader.GetProgram()));
		glCheck(glBindTexture(GL_TEXTURE_2D, _sprMgr.Get(_sheet).GetTexture()));
		glCheck(glUniform2f(glGetUniformLocation(_spriteShader.GetProgram(), "textureSize"), _sprMgr.Get(_sheet).GetWidth(), _sprMgr.Get(_sheet).GetHeight()));

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, _frameBuffer));
		glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(unsigned int) * _queued, _frameData.data()));

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, _transformBuffer));
		glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float) * _queued, _transformData.data()));

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer));
		glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(unsigned char) * _queued, _colorData.data()));

		// Draw a batch
		glCheck(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _queued));

		// Clear sprite data
		_frameData.clear();
		_transformData.clear();
		_colorData.clear();
		_queued = 0;
	}

	void SpriteRenderer::Clear()
	{
		glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
}