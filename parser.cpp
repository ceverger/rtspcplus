// parser.cpp - реализация конструкторов и других методов класса RTSPParser

#include <cstring>
#include "parser.hpp"

	RTSPParser::RTSPParser()
	{
		/* Конструктор класса по умолчанию */

			reslen = 0;
			bzero(result, ressize);
			errstr = nullptr;
	}

	void RTSPParser::reset()
	{
		/* Сброс данных класса */

			reslen = 0;
			bzero(result, ressize);
			errstr = 0;			
	}
