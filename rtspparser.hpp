/* rtspparser.hpp - описания общего класса RTSPParser для парсинга
						  rtsp-запросов или  rtsp-ответов и получение  
						  необходимых значений строк и полей специфичных
						  для протоколов RTSP/1.0 и RTSP/1.1             */

#ifndef RTSP_PARSER_HPP

	class RTSPParser 
	{
		public:

			enum { ressize = 1024 }; // Максимальный размер буфера для 
											 // сохранения результата парсинга
			
		protected:

			int reslen;			       // Размер полезных данных в буфере
			char result[ressize];    // Буфер для сохранения результата парсинга
			const char *errorString; // Указтель на строку с ошибкой

		public:

			/* Конструкторы и деструкторы класса */

				RTSPParser();
				virtual ~RTSPParser() {}

			/* Селекторы класса */
		
				int getReslen() const { return reslen; }
				const char *getResult() const { return result; }
				const char *getErrstr() const { return errorString; }

			/* Методы для парсинга rtsp-сообщений */

				void parseCacheControl(char *message);
				void parseConnection(char *message);
				void parseContentBase(char *message);
				void parseContentEncoding(char *message);
				void parseContentLanguage(char *message);
				 int parseContentLength(char *message);
				void parseContentLocation(char *message);

				void parseContentType(char *message);
				void parseContentTypeMediaType(char *buf, int bufsize);
				void parseContentTypeCharset(char *buf, int bufsize);

				 int parseCSeq(char *message);
				void parseDate(char *message);
				void parseExpires(char *message);
				void parseLastModified(char *message);
				void parseRange(char *message);
			 double parseScale(char *message);
				void parseServer(char *message);

				void parseSession(char *message);
				void parseSessionID(char *buf, int bufsize);
				 int parseSessionTimeout();
				
				 int parseSpeed(char *message);

				void parseTransport(char *message);
				void parseTransportProtocol(char *buf, int bufsize);
				void parseTransportProfile(char *buf, int bufsize);
				void parseTransportLower(char *buf, int bufsize);
				void parseTransportTransmission(char *buf, int bufsize);
				void parseTransportDestinationIP(char *buf, int bufsize);
				void parseTransportSourceIP(char *buf, int bufsize);
				void parseTransportClientPort(int *clientRtpPort, int *clientRtcpPort);
				void parseTransportServerPort(int *serverRtpPort, int *serverRtcpPort);
				void parseTransportMode(char *buf, int bufsize);
				void parseTransportSSRC(char *buf, int bufsize);

				void parseVia(char *message);

			/* Чекеры класса  */
		
				bool isSuccess() const;
				bool isFailed() const;

			/* Другие методы класса */
				
				void reset();
			
	}; // class RTSPParser

#endif // RTSP_PARSER_HPP
