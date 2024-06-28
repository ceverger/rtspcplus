#ifndef RTSP_HPP

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
		
				       int  getReslen() const { return reslen;      }
				const char *getResult() const { return result;      }
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


	class RTSPRequestParser : public RTSPParser 
	{
		public:

			/* Конструкторы и деструкторы класса */

				RTSPRequestParser() : RTSPParser() {}
				virtual ~RTSPRequestParser() {}

			/* Методы для парсинга rtsp-запроса */

				void parseMethod(const char *message);

				void parseURI(const char *message);
				void parseLogin(char *buf, int bufsize);
				void parsePassword(char *buf, int bufsize);
				void parseHost(char *buf, int bufsize);
				 int parsePort();
				void parseURL(char *buf, int bufsize);
				void parseName(char *buf, int bufsize);

				void parseVersion(const char *message);
				double parseVernum();

				void parseAccept(char *message);
				void parseAcceptEncoding(char *message);
				void parseAcceptLanguage(char *message);

				void parseAuthorization(char *message);
				 int parseBandwidth(char *message);
				 int parseBlocksize(char *message);
				void parseConference(char *message);
				void parseFrom(char *message);
				void parseIfModifiedSince(char *message);
				void parseProxyRequire(char *message);
				void parseReferer(char *message);
				void parseReuire(char *message);
				void parseUserAgent(char *message);

			/* Чекеры класса  */

				bool messageIsRequest(const char *message) const;
				
	}; // RTSPRequestParser


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

	class RTSPGenerator 
	{

		public:

			enum { ressize = 1024 }; // Максимальный размер буфера для 
											 // сохранения результата генерации
			
		protected:

			int reslen;			       // Размер полезных данных в буфере
			char result[ressize];    // Буфер для сохранения результата генерации
			const char *errorString; // Указатель на строку с ошибкой

		public:
			
			/* Конструкторы и деструкторы класса */

				RTSPGenerator();
				virtual ~RTSPGenerator() {}

			/* Селекторы класса */
		
				int getReslen() const { return reslen; }
				const char *getResult() const { return result; }
				const char *getErrstr() const { return errorString; }

			/* Методы для добавления полей в rtsp-сообщения */

				 int addCSeq(int cseq);
				void addDate(const char *date);

			/* Другие методы класса */
				
				void reset();

	}; // RTSPGenerator


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


	class RTSPResponseGenerator : public RTSPGenerator
	{

		private:

			

		public:

			

	} //  RTSPResponseGenerator

#endif // RTSP_HPP
