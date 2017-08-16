#ifndef IAITO_CONTEXT_HPP
#define IAITO_CONTEXT_HPP

#include <string>
#include <vector>

#include "iaito/utility/OpenGL.hpp"
#include "iaito/utility/Log.hpp"

namespace iaito
{
	enum ButtonCode
	{
		ButtonUnknown = -1,
		Button1,
		Button2,
		Button3,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,
	};

	enum KeyCode
	{
		KeyUnknown = -1,
		KeySpace = 32,

		KeyComma = 44,
		KeyMinus,
		KeyPeriod,
		KeySlash,

		Key0 = 48,
		Key1, Key2, Key3, Key4, Key5,
		Key6, Key7, Key8, Key9,

		KeySemicolon = 59,
		KeyEqual = 61,

		KeyA = 65,
		KeyB, KeyC, KeyD, KeyE, KeyF,
		KeyG, KeyH, KeyI, KeyJ, KeyK,
		KeyL, KeyM, KeyN, KeyO, KeyP,
		KeyQ, KeyR, KeyS, KeyT, KeyU,
		KeyV, KeyW, KeyX, KeyY, KeyZ,

		KeyLeftBracket = 91,
		KeyBackslash,
		KeyRightBracket,

		KeyEscape = 256,
		KeyEnter,
		KeyTab,
		KeyBackspace,
		KeyInsert,
		KeyDelete,
		KeyRight,
		KeyLeft,
		KeyDown,
		KeyUp,
		KeyPageUp,
		KeyPageDown,
		KeyHome,
		KeyEnd,

		KeyF1 = 284,
		KeyF2, KeyF3, KeyF4, KeyF5, KeyF6, KeyF7, 
		KeyF8, KeyF9, KeyF10, KeyF11, KeyF12,

		KeyNum0 = 320,
		KeyNum1, KeyNum2, KeyNum3, KeyNum4, KeyNum5,
		KeyNum6, KeyNum7, KeyNum8, KeyNum9,
		KeyNumDecimal,
		KeyNumDivide,
		KeyNumMultiply,
		KeyNumSubtract,
		KeyNumAdd,
		KeyNumEnter,

		KeyLeftShift = 340,
		KeyLeftControl,
		KeyLeftAlt,
		KeyRightShift = 344,
		KeyRightControl,
		KeyRightAlt
	};

	class Context
	{
		friend class Game;
		
		public:
			void Init();

			void ToggleFullscreen();
			void SetSize(int width, int height);
			void GetSize(int* width, int* height);

			void SetPosition(int x, int y);
			void GetPosition(int* x, int* y);

			void SetTitle(const std::string& title);

			bool HasFocus();
			bool IsClosed();

			double GetTimeDelta();

			// Input
			bool CheckKeyPress(KeyCode key);
			bool CheckKeyHold(KeyCode key);

			bool CheckButtonPress(ButtonCode button);
			bool CheckButtonHold(ButtonCode button);

			double GetWheelDelta();

			void SetMousePosition(int x, int y);
			void GetMousePosition(int* x, int* y);

		private:
			Context();
			~Context();

			Context(const Context& that);
			Context& operator=(const Context& that);

			void Uninit();

			void Update();

			GLFWwindow* _window;

			std::string _title;
			bool _closed;
			bool _fullscreen;
			int _width, _height;
			double _dt;

			std::vector<KeyCode> _keysOnce;
			std::vector<ButtonCode> _mouseOnce;
			double _mwd;

			inline static void error_callback(int error, const char * description)
			{
				Log(Error)<<"Context error - "<<description;
			}

			inline static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Context *context = static_cast<Context*>(glfwGetWindowUserPointer(window));

				if (action == GLFW_PRESS)
					context->_keysOnce.push_back(static_cast<KeyCode>(key));

			}

			inline static void button_callback(GLFWwindow* window, int button, int action, int mods)
			{
				Context *context = static_cast<Context*>(glfwGetWindowUserPointer(window));

				if (action == GLFW_PRESS)
					context->_mouseOnce.push_back(static_cast<ButtonCode>(button));
			}

			inline static void scroll_callback(GLFWwindow* window, double xpos, double ypos)
			{
				Context *context = static_cast<Context*>(glfwGetWindowUserPointer(window));

				context->_mwd += ypos;
			}

			inline static void close_callback(GLFWwindow* window)
			{
				Context *context = static_cast<Context*>(glfwGetWindowUserPointer(window));

				context->_closed = true;
			}
	};
}

#endif // IAITO_CONTEXT_HPP