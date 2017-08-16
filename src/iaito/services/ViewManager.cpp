#include "iaito/services/ViewManager.hpp"

namespace iaito
{
	ViewManager::ViewManager() : _viewX(0), _viewY(0), _viewW(800), _viewH(600), _viewScale(1){}
	ViewManager::~ViewManager(){}

	void ViewManager::SetPosition(float x, float y)
	{
		_viewX = x;
		_viewY = y;
	}

	void ViewManager::GetPosition(float* x, float* y)
	{
		*x = _viewX;
		*y = -_viewY;
	}

	void ViewManager::Offset(float x, float y)
	{
		_viewX += x;
		_viewY -= y;
	}

	void ViewManager::SetSize(unsigned int w, unsigned int h)
	{
		_viewW = w;
		_viewH = h;

		glCheck(glViewport(0, 0, _viewW, _viewH));
	}

	void ViewManager::GetSize(unsigned int* w, unsigned int* h)
	{
		*w = _viewW;
		*h = _viewH;
	}

	void ViewManager::SetScale(float s)
	{
		_viewScale = s;
	}

	void ViewManager::GetScale(float* s)
	{
		*s = _viewScale;
	}

	void ViewManager::Zoom(float amount)
	{
		_viewScale += amount;
	}
}