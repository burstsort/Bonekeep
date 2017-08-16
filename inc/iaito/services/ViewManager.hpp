#ifndef IAITO_VIEW_MANAGER_HPP
#define IAITO_VIEW_MANAGER_HPP

#include "iaito/core/Service.hpp"
#include "iaito/utility/OpenGL.hpp"

namespace iaito
{
	class ViewManager: public Service
	{
		public:
			ViewManager();
			~ViewManager();

			void SetPosition(float x, float y);
			void GetPosition(float* x, float* y);
			void Offset(float x, float y);

			void SetSize(unsigned int w, unsigned int h);
			void GetSize(unsigned int* w, unsigned int* h);

			void SetScale(float s);
			void GetScale(float* s);
			void Zoom(float amount);

		private:
			ViewManager(const ViewManager& that);
			ViewManager& operator=(const ViewManager& that);

			float _viewX;
			float _viewY;

			unsigned int _viewW;
			unsigned int _viewH;

			float _viewScale;
	};
}

#endif // IAITO_VIEW_MANAGER_HPP