// requestparser.cpp - реализация методов класса RTSPRequestParser

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "requestparser.hpp"

	void RTSPRequestParser::parseMethod(const char *message)
	{
		/* Получение названия метода  */

		if(message == NULL) return;

		int i = 0;

		reset();

		while(i < ressize && message[i] != ' ')
		{
			result[i] = message[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);
	}

	void RTSPRequestParser::parseURI(const char *message)
	{
		/* Получение URL */

		if(message == NULL) return;
		
		int i = 0;
		const char *pos = message;

		reset();

		while(*pos++ != ' ');

		if(*pos == '*') 
		{
			result[0] = *pos;
			result[1] = '\0';
			reslen = 1;
			return;
		}

		pos = strstr(pos, "rtsp://");
		if(pos == NULL) return;

		while(i < ressize && pos[i] != ' ')
		{
			result[i] = pos[i];
			i++;			
		}

		result[i] = '\0';
		reslen = strlen(result);
	}

	void RTSPRequestParser::parseLogin(char *buf, int bufsize)
	{
		/* Получение логина пользователя  */

		if(buf == NULL || reslen <= 1) return;

		int i = 0;
		char *pos, *cur, *end;

		bzero(buf, bufsize);

 		pos = strstr(result, "rtsp://");
		if(pos == NULL) return;
		pos += strlen("rtsp://");

		cur = pos;
		while(*cur != '/' && *cur != '\0') cur++;
		end = cur;
		
		cur = pos;
		while(*cur != '@' && cur != end) cur++;
		if(cur == end) return;
		end = cur;

		cur = pos;
		while(*cur != ':' && cur != end) cur++;
		if(cur == end) return;
		end = cur;
	
		while(i < bufsize && pos != end)
		{
			*buf++ = *pos++;
			++i;
		}

		*buf = '\0';	
	}

	void RTSPRequestParser::parsePassword(char *buf, int bufsize)
	{
		/* Получение пароля пользователя  */

		if(buf == NULL || reslen <= 1) return;

		int i = 0;
		char *pos, *cur, *end;

		bzero(buf, bufsize);

 		pos = strstr(result, "rtsp://");
		if(pos == NULL) return;
		pos += strlen("rtsp://");

		cur = pos;
		while(*cur != '/' && *cur != '\0') cur++;
		end = cur;
		
		cur = pos;
		while(*cur != '@' && cur != end) cur++;
		if(cur == end) return;
		end = cur;
		
		cur = pos;
		while(*cur != ':' && cur != end) cur++;
		if(cur == end) return;
		pos = cur + 1;

		while(i < bufsize && pos != end)
		{
			*buf++ = *pos++;
			++i;
		}

		*buf = '\0';
	}

	void RTSPRequestParser::parseHost(char *buf, int bufsize)
	{
		/* Получение хоста  */

		if(buf == NULL || reslen <= 1) return;

		int i = 0;
		char *pos, *cur, *end;

		bzero(buf, bufsize);

 		pos = strstr(result, "rtsp://");
		if(pos == NULL) return;
		pos += strlen("rtsp://");

		cur = pos;
		while(*cur != '/' && *cur != '\0') cur++;
		end = cur;

		cur = pos;
		while(*cur != '@' && cur != end) cur++;
		if(*cur == '@') pos = cur + 1;

		cur = pos;
		while(*cur != ':' && cur != end) cur++;
		if(*cur == ':') end = cur;


		while(i < bufsize && pos != end)
		{
			*buf++ = *pos++;
			++i;
		}

		*buf = '\0';
	}

	int RTSPRequestParser::parsePort()
	{
		/* Получение порта */
		
		if(reslen <= 1) return -1;

		int i = 0;
		int bufsize = 8;
		char buf[bufsize];
		char *buf_pos = buf;
		char *pos, *cur, *end;

		bzero(buf, bufsize);

 		pos = strstr(result, "rtsp://");
		if(pos == NULL) return -1;
		pos += strlen("rtsp://");

		cur = pos;
		while(*cur != '/' && *cur != '\0') cur++;
		end = cur;

		cur = pos;
		while(*cur != '@' && cur != end) cur++;
		if(*cur == '@') pos = cur + 1;

		cur = pos;
		while(*cur != ':' && cur != end) cur++;
		if(cur == end) return 0;
		
		pos = cur + 1;
		while(i < bufsize && pos != end)
		{
			*buf_pos++ = *pos++;
			i++;
		}

		*buf_pos = '\0';		

		return strtol(buf, NULL, 10);;
	}

	void RTSPRequestParser::parseURL(char *buf, int bufsize)
	{
		if(buf == NULL || reslen <= 1) return;

		int i = 0, j = 0;
		char *pos, *cur, *end, *temp;

		bzero(buf, bufsize);

 		pos = strstr(result, "rtsp://");
		if(pos == NULL) return;
		end = pos + strlen("rtsp://");
		
		while(i < bufsize && pos != end)
		{
			*buf++ = *pos++;
			i++;
		}

		*buf = '\0';

		pos = end;
		cur = pos;
		while(*cur != '/' && *cur != '\0') cur++;
		end = cur;		

		cur = pos;
		while(*cur != '@' && cur != end) cur++;
		if(*cur == '@') pos = cur + 1;

		cur = pos;
		while(*cur != ':' && cur != end) cur++;

		if(*cur == ':')
		{
			temp = end;
			end = cur;

			while(i < bufsize && pos != end)
			{
				*buf++ = *pos++;
				i++;
			}

			*buf = '\0';

			pos = temp;
		}

		end = result + reslen;

		while(i < bufsize && pos != end)
		{
			*buf++ = *pos++;
			i++;
		}

		*buf = '\0';		
	}

	void RTSPRequestParser::parseName(char *buf, int bufsize)
	{
		if(buf == NULL || reslen <= 1) return;

		int i = 0;
		char *pos, *cur, *end;

		bzero(buf, bufsize);

 		pos = strstr(result, "rtsp://");
		if(pos == NULL) return;
		pos += strlen("rtsp://");

		cur = pos;

		end = result + reslen;

		while(*cur != '/' && *cur != '\0') cur++;
		if(*cur == '\0') return;
		pos = cur + 1;
		
		while(i < bufsize && pos != end)
		{
			*buf++ = *pos++;
			i++;
		}

		*buf = '\0';
	}

	void RTSPRequestParser::parseVersion(const char *message)
	{
		if(message == NULL) return;

		int i = 0;
		const char *pos = strstr(message, "RTSP/");

		if(pos == NULL) return;

		reset();

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
	}

	double RTSPRequestParser::parseVernum()
	{
		int i = 0;
		int bufsize = 8;
		char buf[bufsize];
		char *pos, *end, *buf_pos = buf;

		pos = strstr(result, "RTSP/");
		if(pos == NULL) return -1;

		end = result + reslen;
		
		while(*pos++ != '/');

		while(i < bufsize && pos != end)
		{
			*buf_pos++ = *pos++;
			i++;
		}

		*buf_pos = '\0';

		return strtod(buf, NULL);
	}

	void RTSPRequestParser::parseAccept(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Accept: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);		
	}

	void RTSPRequestParser::parseAcceptEncoding(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Accept-Encoding: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);		
	}

	void RTSPRequestParser::parseAcceptLanguage(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Accept-Language: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);	
	}

	int RTSPRequestParser::parseBandwidth(char *message)
	{
		if(message == NULL) return -1;

		int i = 0;
		char bufsize = 8;
		char buf[bufsize];

		char *pos = strstr(message, "Bandwidth: ");
		if(pos == NULL) return -1;

		while(*pos++ != ' '); 

		while(i < bufsize && pos[i] != '\r')
		{
			buf[i] = pos[i];
			i++;
		}
		
		buf[i] = '\0';
		
		return strtol(buf, NULL, 10);	
	}

	 int RTSPRequestParser::parseBlocksize(char *message)
	 {
		if(message == NULL) return -1;

		int i = 0;
		char bufsize = 8;
		char buf[bufsize];

		char *pos = strstr(message, "Blocksize: ");
		if(pos == NULL) return -1;

		while(*pos++ != ' '); 

		while(i < bufsize && pos[i] != '\r')
		{
			buf[i] = pos[i];
			i++;
		}
		
		buf[i] = '\0';
		
		return strtol(buf, NULL, 10);		 
	 }

	void RTSPRequestParser::parseConference(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Conference: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);
	}

	void RTSPRequestParser::parseFrom(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "From: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);
	}

	void RTSPRequestParser::parseIfModifiedSince(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "If-Modified-Since: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);
	}

	void RTSPRequestParser::parseProxyRequire(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Proxy-Require: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);		
	}

	void RTSPRequestParser::parseReferer(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Referer: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);		
	}

	void RTSPRequestParser::parseReuire(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Reuire: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);		
	}

	void RTSPRequestParser::parseUserAgent(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "User-Agent: ");

		if(pos == NULL) return;

		reset();

		while(*pos++ != ' ');

		while(i < ressize && pos[i] != '\r')
		{
			result[i] = pos[i];
			i++;
		}

		result[i] = '\0';
		reslen = strlen(result);		
	}
