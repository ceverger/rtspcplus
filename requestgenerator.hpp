#include "rtspgenerator.hpp"

#ifndef RTSP_REQUEST_GENERATOR_HPP
#define RTSP_REQUEST_GENERATOR_HPP

	class RTSPRequestGenerator : public RTSPGenerator
	{
		public:

		/* Конструкторы и деструкторы класса */

			RTSPRequestGenerator() : RTSPGenerator() {}
			virtual ~RTSPRequestGenerator() {}

		/* Методы для генерации rtsp-запросов */

			void genRequest(const char *method, const char *uri, double vernum = 0);
			void setMethod(char *method);
			void setURI(char *uri);
			void setLogin(char *login);
			void setPassword(char *password);
			void setPort(int port);
			void setName(char *name);
			void setVernum(double vernum);

		/* Методы для добавления полей в текст запроса */

			void addAccept(char *accept);
			void addAcceptEncoding(char *acceptEncoding);
			void addAcceptLanguage(char *acceptLanguage);

			void addAuthorization(char *authorization);
			void addBandwidth(int bandwidth);
			void addBlocksize(int blocksize);
			void addConference(char *conference);

	}; //  RTSPRequestGenerator

#endif // RTSP_REQUEST_GENERATOR_HPP
