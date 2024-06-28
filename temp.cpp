	void RTSPRequestParser::parseName(char *buf, int bufsize)
	{
		/* Получение хоста  */

		if(buf == NULL || reslen <= 1) return;

		int i = 0;
		char *pos, *cur, *end, *temp;

		bzero(buf, bufsize);

		pos = strstr(result, "rtsp://");
		if(pos == NULL) return;

		end = result + reslen;

		pos = pos + strlen("rtsp://");
		pos = strchr(pos, '/');
		if(pos == NULL) return;

		while(i < bufsize && pos[i] != *end)
		{
			buf[i] = pos[i];
			i++;				
		}

		buf[i] = '\0';
	}
