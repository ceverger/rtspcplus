#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "requestgenerator.hpp"

	bool checkMethod(const char *method)
	{
		if(method == nullptr) return false;
		if(strlen(method) == 0) return false;

		if(strcmp(method, "OPTIONS")       != 0)
		if(strcmp(method, "DESCRIBE")      != 0)
		if(strcmp(method, "ANNOUNCE")      != 0)
		if(strcmp(method, "SETUP")         != 0)
		if(strcmp(method, "PLAY")          != 0)
		if(strcmp(method, "PAUSE")         != 0)
		if(strcmp(method, "TEARDOWN")      != 0)
		if(strcmp(method, "GET_PARAMETER") != 0)
		if(strcmp(method, "SET_PARAMETER") != 0)
		return false;

		return true;
	}

	bool checkURI(const char *uri)
	{
		if(uri == nullptr) return false;
		if(strlen(uri) == 0) return false;

		if(*uri == '*') return true;
		if(strstr(uri, "rtsp://") != nullptr) return true;

		return false;
	}

	void RTSPRequestGenerator::genRequest(const char *method, const char *uri, double vernum)
	{
		if(method == nullptr || uri == nullptr) return;
		if(strlen(method) == 0 || strlen(uri) == 0) return;
		if(checkMethod(method) == false || checkURI(uri) == false) return;

		int ret, i = 0;
		char *pos, *end, *buf;

		int cachesize = 512;
		char cache[cachesize];

		strcpy(cache, method);
		buf = result;

		pos = cache;
		end = cache + strlen(cache);

		while(i < ressize && pos != end)
		{
			*buf++ = *pos++;
			 i++;
		}

		*buf++ = ' ';
		 i++;

		bzero(cache, cachesize);
		strcpy(cache, uri);

		pos = cache;
		end = cache + strlen(cache);

		while(i < ressize && pos != end)
		{
			*buf++ = *pos++;
			 i++;
		}

		*buf++ = ' ';
		 i++;

		ret = snprintf(cache, cachesize, "%s%.1f\r\n", "RTSP/", vernum);
		if(ret < 0) return;
		
		pos = cache;
		end = cache + strlen(cache);

		while(i < ressize && pos != end)
		{
			*buf++ = *pos++;
			 i++;
		}

		*buf = '\0';
		 reslen = strlen(buf);
	}

	void RTSPRequestGenerator::setMethod(char *method)
	{
		if(!checkMethod(method)) return;

		int i = 0;
		int bufsize = 1024;
		char buf[bufsize];
		char *pos, *end, *temp, *buf_pos;

		if(reslen == 0)
		{
			strcpy(result, method);
			reslen = strlen(result);
			return;
		}

		pos = result;
		end = result + reslen;
		buf_pos = buf;

		while(*pos != ' ' && pos != end) pos++;

		bzero(result, reslen);

		if(pos == end)
		{
			strcpy(result, method);
			reslen = strlen(result);
			return;			
		}

		temp = pos;
		pos = method;
		end = method + strlen(method);

		while(i < bufsize && pos != end)
		{
			*buf_pos++ = *pos++;
			 i++;
		}

		*buf_pos = '\0';

		pos = temp;
		end = result + reslen;

		while(i < bufsize && pos != end)
		{
			*buf_pos++ = *pos++;
			 i++;
		}

		*buf_pos = '\0';

		strcpy(result, buf);
	}

	void RTSPRequestGenerator::setURI(char *uri)
	{
		if(uri == nullptr || reslen == 0) return;

		int i = 0;
		int bufsize = 1024;
		char buf[bufsize];
		char *pos, *end, *temp, *buf_pos;
	
		pos = strstr(result, "OPTION");
		if(pos == nullptr) pos = strstr(result, "DESCRIBE");
		if(pos == nullptr) pos = strstr(result, "ANNOUNCE");
		if(pos == nullptr) pos = strstr(result, "SETUP");
		if(pos == nullptr) pos = strstr(result, "PLAY");
		if(pos == nullptr) pos = strstr(result, "PAUSE");
		if(pos == nullptr) pos = strstr(result, "TEARDOWN");
		if(pos == nullptr) pos = strstr(result, "GET_PARAMETER");
		if(pos == nullptr) pos = strstr(result, "SET_PARAMETER");
		if(pos == nullptr) return;

		buf_pos = buf;
		end = result + reslen;

		while(i < bufsize && *pos!= ' ' && pos != end)
		{
			*buf_pos++ = *pos++ ;
			 i++;
		}

	  *buf_pos++ = ' ';
		i++;

		temp = pos;
		pos = uri;
		end = uri + strlen(uri);

		while(i < bufsize && pos != end)
		{
			*buf_pos++ = *pos++ ;
			 i++;
		}

	  *buf_pos++ = ' ';
		pos = temp + 1;
		end = result + reslen;

		if(pos == end)
		{
			reset();
			*buf_pos = '\0';
			strcpy(result, buf);
			reslen = strlen(result);
			return;
		}

		while(i < bufsize && pos != end)
		{
			*buf_pos++ = *pos++ ;
			 i++;
		}

		*buf_pos = '\0';

		strcpy(result, buf);
		reslen = strlen(result);
		return;
	}
