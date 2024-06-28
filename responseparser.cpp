// responseparser.cpp - реализация методов класса RTSPResponseParser

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "rtspparser.h"


	void RTSPResponseParser::parseStatusLine(const char *message)
	{
		
	}

	 int RTSPResponseParser::parseStatusCode(const char *message)
	 {
	    
	 }

	 int RTSPResponseParser::parseReasonPhrase(const char *message)
	 {
		 
	 }

	void RTSPResponseParser::parseAllow(const char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Allow: ");

		if(pos == NULL) return -1;

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

	void RTSPResponseParser::parsePublic(const char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Public: ");

		if(pos == NULL) return -1;

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

	void RTSPResponseParser::parseRetryAfter(const char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Retry-After: ");

		if(pos == NULL) return -1;

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

	void RTSPResponseParser::parseRtpInfo(const char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "RTP-Info: ");

		if(pos == NULL) return -1;

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

	void RTSPResponseParser::parseUnsupported(const char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "Unsupported: ");

		if(pos == NULL) return -1;

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

	void RTSPResponseParser::parseWWWAuthenticate(const char *message)
	{
		if(message == NULL) return;

		int i = 0;
		char *pos = strstr(message, "WWW-Authenticate: ");

		if(pos == NULL) return -1;

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
