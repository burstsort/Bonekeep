#ifndef IAITO_SERVICE_HPP
#define IAITO_SERVICE_HPP

namespace iaito
{
	class Service
	{
		public:
			virtual ~Service(){};

			virtual void Update(){};
	};
}

#endif // IAITO_SERVICE_HPP