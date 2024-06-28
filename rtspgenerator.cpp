#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "rtspgenerator.hpp"

	RTSPGenerator::RTSPGenerator()
	{
		/* Конструктор класса по умолчанию */

			reslen = 0;
			bzero(result, ressize);
			errorString = nullptr;
	}

	void RTSPGenerator::reset()
	{
		/* Сброс данных класса */

			reslen = 0;
			bzero(result, ressize);
			errorString = nullptr;			
	}
