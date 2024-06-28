// responseparser.hpp - описание класса RTSPResponseParser для парсинга rtsp-ответов

#include "rtspparser.h"

#ifndef RTSP_RESPONSE_PARSER_HPP

	class RTSPResponseParser  : public RTSPParser
	{
		public:

			/* Конструкторы и деструкторы класса */

				RTSPResponseParser() : RTSPParser() {}
				virtual ~RTSPResponseParser() {}

			/* Методы для парсинга rtsp-запроса */

				void parseStatusLine(const char *message);
				 int parseStatusCode(const char *message);
				 int parseReasonPhrase(const char *message);
				void parseAllow(const char *message);
				void parsePublic(const char *message);
				void parseRetryAfter(const char *message);
				void parseRtpInfo(const char *message);
				void parseUnsupported(const char *message);
				void parseWWWAuthenticate(const char *message);

			/* Чекеры класса  */

				bool messageIsResponse(const char *message) const;
				
	}; // RTSPResponseParser

#endif // RTSP_RESPONSE_PARSER_HPP
