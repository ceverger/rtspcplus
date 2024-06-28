// rtspparser.cpp - реализация методов класса RTSPParser

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "rtspparser.hpp"

	RTSPParser::RTSPParser()
	{
		/* Конструктор класса по умолчанию */

			reslen = 0;
			bzero(result, ressize);
			errorString = nullptr;
	}

	void RTSPParser::parseCacheControl(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Content-Control: ");

		if(pos == NULL) return ;

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

	void RTSPParser::parseConnection(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Connection: ");

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

	void RTSPParser::parseContentBase(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Content-Base: ");

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

	void RTSPParser::parseContentEncoding(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Content-Encoding: ");

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

	void RTSPParser::parseContentLanguage(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Content-Language: ");

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

	int RTSPParser::parseContentLength(char *message)
	{
		if(message == NULL) return -1;

		int i = 0;
		char *pos;
		char bufsize = 16;
		char buf[bufsize];

		pos = strstr(message, "Content-Length: ");
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

	void RTSPParser::parseContentLocation(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Content-Location: ");

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

	void RTSPParser::parseContentType(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Content-Type: ");

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

	void RTSPParser::parseContentTypeMediaType(char *buf, int bufsize)
	{
		if(reslen == 0) return;

		int i = 0;
		char *pos = result;
		
		while(i < bufsize && pos[i] != ';' && pos[i] != '\r')
		{
			buf[i] = pos[i];
			i++;
		}

		buf[i] = '\0';						
	}

	void RTSPParser::parseContentTypeCharset(char *buf, int bufsize)
	{
		if(reslen == 0) return;

		int i = 0;
		char *pos = strstr(result, "charset=");

		while(i < bufsize && pos[i] != ';' && pos[i] != '\r')
		{
			buf[i] = pos[i];
			i++;
		}

		buf[i] = '\0';
	}

	int RTSPParser::parseCSeq(char *message)
	{
		/* Получение номера сообщения */

			if(message == NULL) return -1;

			int i = 0;
			char bufsize = 16;
			char buf[bufsize];

			char *pos = strstr(message, "CSeq: ");
			if(pos == NULL) return -1;

			reset();

			while(*pos++ != ' '); 

			while(i < bufsize && pos[i] != '\r')
			{
				buf[i] = pos[i];
				i++;
			}
			
			buf[i] = '\0';
			
			return strtol(buf, NULL, 10);	
	}

	void RTSPParser::parseDate(char *message)
	{

		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Date: ");

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

	void RTSPParser::parseExpires(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Expires: ");

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

	void RTSPParser::parseLastModified(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Last-Modified: ");

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

	void RTSPParser::parseRange(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Range: ");

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

	double RTSPParser::parseScale(char *message)
	{
		if(message == NULL) return -1;

		int i = 0;
		char *pos;
		int bufsize = 16;
		char buf[bufsize]; 

		pos = strstr(message, "Scale: ");
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

	void RTSPParser::parseServer(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Server: ");

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

	void RTSPParser::parseSession(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Session:");

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

	void RTSPParser::parseSessionID(char *buf, int bufsize)
	{
		if(reslen == 0) return;

		int i = 0;
		char *pos = result;

		while(*pos++ != ' ');

		while(i < bufsize && pos[i] != ';' && pos[i] != '\r')
		{
			buf[i] = pos[i];
			i++;
		}

		buf[i] = '\0';
	}

	int RTSPParser::parseSessionTimeout()
	{
		if(strstr(result, "Session:") == NULL) return -1;

		int i = 0;
		char *pos;
		char bufsize = 8;
		char buf[bufsize];

		pos = strstr(result, "timeout=");
		if(pos == NULL) return -1;

		while(*pos++ != '=');

		while(i < bufsize && pos[i] != ';' && pos[i] != '\r')
		{
			buf[i] = pos[i];
			i++;
		}

		buf[i] = '\0';

		return strtol(buf, NULL, 10);
	}

	int RTSPParser::parseSpeed(char *message)
	{
		if(message == NULL) return -1;

		int i = 0;
		char bufsize = 8;
		char buf[bufsize];

		char *pos = strstr(message, "Speed: ");
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

	void RTSPParser::parseVia(char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Via:");

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

	void RTSPParser::reset()
	{
		/* Сброс данных класса */

			reslen = 0;
			bzero(result, ressize);
			errorString = 0;			
	}
