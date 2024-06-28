/* parser.hpp - описания класса RTSPParser для парсинга rtsp-запросов или ответов 
					 и получение необходимых значений строк и полей специфичных для 
					 протоколов RTSP/1.0 и RTSP/1.1                                   */

#ifndef RTSP_PARSER_HPP

	class RTSPParser 
	{
		public:

			enum { ressize = 2048; }; // Максимальный размер буфера для 
											  // сохранения результата парсинга
			
		private:

			int reslen;			     // Размер полезных данных в буфере
			char result[bufsize];  // Буфер для сохранения результата парсинга
			const char *errstr;    // Указтель на строку с ошибкой

		public:

			/* Конструкторы и деструкторы класса */

				RTSPParser();
				virtual ~RTSPParser() {}

			/* Селекторы класса */
		
				int getReslen() const { return reslen; }
				const char *getResult() const { return result; }
				const char *getErrstr() const { return errstr; }

			/* Методы для парсинга rtsp-сообщения */
			
				void parseRequestLine(const char *message);
				void parseRequestMethod(const char *message);
				void parseRequestURI(const char *message);
				void parseRequestURL(const char *message);
				void parseRequestIpaddr(const char *message);
				 int parseRequestPort(const char *message);
				void parseRequestLogin(const char *message);
				void parseRequestPassword(const char *message);
				void parseRequestVersion(const char *message);
			   void parseRequestAccept(const char *message);
				void parseRequestAcceptEncoding(const char *message);
				void parseRequestAcceptLanguage(const char *message);
				void parseRequestAuthorization(const char *message);
				 int parseRequestBandwidth(const char *message);
				 int parseRequestBlocksize(const char *message);
				void parseRequestConference(const char *message);
				void parseRequestConnection(const char *message);
				 int parseRequestCSeq(const char *message);
				void parseRequestDate(const char *message);
				void parseRequestFrom(const char *message);
				void parseRequestIfModifiedSince(const char *message);
				void parseRequestroxyRequire(const char *message);
				void parseRequestProxyRequire(const char *message);
				void parseRequestRange(const char *message);
				void parseRequestRefer(const char *message);
				void parseRequestRequire(const char *message);
				void parseRequestScale(const char *message);
				void parseRequestSession(const char *message);
				void parseRequestSpeed(const char *message);
				void parseRequestTransport(const char *message);
				void parseRequestUserAgent(const char *message);
				void parseRequestVia(const char *message);

				void parseResponseStatusLine(const char *message);
				 int parseResponseStatusCode(const char *message);
				 int parseResponseReasonPhrase(const char *message);
				void parseResponseAllow(const char *message);
				void parseResponseContentBase(const char *message);
				void parseResponseContentEncoding(const char *message);
				void parseResponseContentLanguage(const char *message);
				void parseResponseContentLength(const char *message);
				void parseResponseContentLocation(const char *message);
				void parseResponseContentType(const char *message);
				void parseResponseExpires(const char *message);
				void parseResponseLocation(const char *message);
				void parseResponseProxyAuthenticate(const char *message);
				void parseResponsePublic(const char *message);
				void parseResponseRetryAfter(const char *message);
				void parseResponseRtpInfo(const char *message);
				void parseResponseServer(const char *message);
				void parseResponseVary(const char *message);
				void parseResponseWWWAuthenticate(const char *message);
			
			/* Чекеры класса  */
		
				bool isSuccess() const;
				bool isFailed() const;

				bool messageIsRequest(const char *message) const;
				bool messageIsResponse(const char *message) const;

			/* Другие методы класса */
				
				void reset();
			
	}; // class RTSPParser 

#endif // RTSP_PARSER_HPP
