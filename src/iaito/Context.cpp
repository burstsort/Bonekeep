#include "iaito/Context.hpp"

namespace iaito
{
	Context::Context() : _window(NULL), _title("IAITO"), _closed(true), _fullscreen(false), _width(800), _height(600), _dt(0)
	{
		glfwSetErrorCallback(error_callback);

		if (!glfwInit())
		{
			Log(Error)<<"Error initialising context";
			return;
		}

		Log(Debug)<<"Context initialised - "<<glfwGetVersionString();

		Init();
	}

	Context::~Context()
	{
		Uninit();
		glfwTerminate();

		Log(Debug)<<"Context uninitialised";
	}

	void Context::Init()
	{
		Uninit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		if (_fullscreen)
			_window = glfwCreateWindow(_width, _height, _title.c_str(), glfwGetPrimaryMonitor(), NULL);
		else
			_window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

		if (!_window)
		{
			Log(Error)<<"Error opening window";
			return;
		}

		glfwMakeContextCurrent(_window);

		if (gl3wInit())
		{
			Log(Error)<<"Error loading OpenGL profile";
			return;
		}

		glfwSwapInterval(0);

		glfwSetWindowUserPointer(_window, this);

		glfwSetKeyCallback(_window, key_callback);
		glfwSetMouseButtonCallback(_window, button_callback);
		glfwSetScrollCallback(_window, scroll_callback);
		glfwSetWindowCloseCallback(_window, close_callback);

		glfwSetInputMode(_window, GLFW_CURSOR, _fullscreen ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);

		_closed = false;
	}

	void Context::Uninit()
	{
		_closed = true;

		if (_window)
			glfwDestroyWindow(_window);
	}
	
	void Context::ToggleFullscreen()
	{
		_fullscreen = !_fullscreen;
	}

	void Context::SetSize(int width, int height)
	{
		_width = width;
		_height = height;
		glfwSetWindowSize(_window, width, height);
		glViewport(0, 0, width, height);
	}

	void Context::GetSize(int* width, int* height)
	{
		glfwGetFramebufferSize(_window, width, height);
	}

	void Context::SetPosition(int x, int y)
	{
		glfwSetWindowPos(_window, x, y);
	}

	void Context::GetPosition(int* x, int* y)
	{
		glfwGetWindowPos(_window, x, y);
	}

	void Context::SetTitle(const std::string& title)
	{
		_title = title;
		glfwSetWindowTitle(_window, title.c_str());
	}

	bool Context::HasFocus()
	{
		return glfwGetWindowAttrib(_window, GLFW_FOCUSED);
	}

	bool Context::IsClosed()
	{
		return _closed;
	}

	void Context::Update()
	{
		_keysOnce.clear();
		_mouseOnce.clear();
		_mwd = 0;

		glfwSwapBuffers(_window);
		glfwPollEvents();

		// Timestep
		_dt = glfwGetTime();
		glfwSetTime(0);

		if (_dt > 0.2)
			_dt = 0.2;
	}

	double Context::GetTimeDelta()
	{
		return _dt;
	}

	bool Context::CheckKeyPress(KeyCode key)
	{
		for (unsigned int i = 0; i < _keysOnce.size(); ++i)
			if (_keysOnce[i] == key)
				return true;
		return false;
	}

	bool Context::CheckKeyHold(KeyCode key)
	{
		return (glfwGetKey(_window, key) == GLFW_PRESS);
	}

	bool Context::CheckButtonPress(ButtonCode button)
	{
		for (unsigned int i = 0; i < _mouseOnce.size(); ++i)
			if (_mouseOnce[i] == button)
				return true;
		return false;
	}

	bool Context::CheckButtonHold(ButtonCode button)
	{
		return (glfwGetMouseButton(_window, button) == GLFW_PRESS);
	}

	double Context::GetWheelDelta()
	{
		return _mwd;
	}

	void Context::SetMousePosition(int x, int y)
	{
		glfwSetCursorPos(_window, x, y);
	}

	void Context::GetMousePosition(int* x, int* y)
	{
		double mx, my;
		glfwGetCursorPos(_window, &mx, &my);
		*x = mx;
		*y = my;
	}
}