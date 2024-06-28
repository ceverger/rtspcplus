#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "h264.hpp"

	static uint8_t *buf_pos;
	static uint8_t *buf_end;

	static bool startcode3(uint8_t *buf)
	{
		/* Поиск в видео-файле кодека h264 стартового кода длиной 3 байта */

		if(buf_pos == nullptr || buf_end == nullptr || buf == nullptr) return false;
		if(buf_end - buf <= 3) return false;
		
		return (buf[0] == 0 && buf[1] == 0 && buf[2] == 1) ? true : false;
	}

	static bool startcode4(uint8_t *buf)
	{
		/* Поиск в видео-файле кодека h264 стартового кода длиной 4 байта */

		if(buf_pos == nullptr || buf_end == nullptr || buf == nullptr) return false;
		if(buf_end - buf <= 4) return false;

		return (buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 1) ? true : false;
	}

	H264NalUnit::H264NalUnit()
	{
		pos = nullptr;
		end = nullptr;
	}

	int H264NalUnit::getSize() const
	{
		if(pos == nullptr || end == nullptr) return 0;

		return end - pos;
	}

	uint8_t *H264NalUnit::getPosition() const
	{
		return pos;
	}

	uint8_t *H264NalUnit::getPayload() const
	{
		return pos + 1;
	}

	int H264NalUnit::getPayloadLen() const
	{
		return end - pos - 1;
	}

	void H264NalUnit::setPosition(uint8_t *position)
	{
		pos = position;
		end = nullptr;
	}

	void H264NalUnit::init()
	{
		if(pos == nullptr) return;

		uint8_t *cur = pos;

		if(startcode3(cur) || startcode4(cur))
			while(*cur++ != 1);
		else
			return;

		pos = cur;
		
		while(cur != buf_end)
		{
			if(startcode3(cur) || startcode4(cur)) break;
			cur++;
		}

		end = pos;

		header = *pos;
	}

	H264AccessUnit::H264AccessUnit()
	{
		pos = nullptr;
		cur = nullptr;
		end = nullptr;
		naluCount = 0;
	}

	int H264AccessUnit::getSize() const
	{
		if(pos == nullptr || end == nullptr) return 0;

		return end - pos;
	}

	int H264AccessUnit::getPosition() const
	{
		return pos;
	}

	void H264AccessUnit::setPosition(uint8_t *position)
	{
		if(buf_pos == nullptr || buf_end == nullptr) return;
		if(position == nullptr || position == buf_end) return;
		if(startcode3(cur) == false && startcode4(cur) == false) return;

		pos = position;
		cur = nullptr;
		end = nullptr;
	}

	void H264AccessUnit::parseNalUnit(H264NalUnit & nu)
	{
		if(pos == nullptr || cur == nullptr) return;

		if(startcode3(cur) || startcode4(cur))
		{
			while(*cur++ != 1);
			nu.setPosition(cur);
			nu.init();
		}

		else return;

		while(cur != buf_end)
		{
			if(startcode3(cur) || startcode4(cur)) break;
			cur++;
		}
	 }

	void H264AccessUnit::init()
	{
		if(buf_pos == nullptr || buf_end == nullptr || pos == nullptr) return;

		H264NalUnit nalu;

		cur = pos;

		parseNulUnit(nalu);
		if(nalu.getPayloadType() != 9) return;

		naluCount++;

		do 
		{ 
			parseNulUnit(nalu);
			naluCount++;
			if(nalu.getPayloadType() == 10) break;
		} 
		while(nalu.getPayloadType() != 9);

		end = cur;
		cur = pos;					
	}

	H264Parser::H264Parser()
	{
		/* Конструктор класса по умолчанию */

		pos = nullptr;
		end = nullptr;
	}

	void H264Parser::parseAU(H264AccessUnit & au)
	{
		if(buf_pos == nullptr || pos == nullptr) return;

		cur = pos;
		
		if(startcode3(cur) || startcode4(cur))
		{
			au.setPosition(cur);
			au.init();	
		}
	}


	int H264Parser::loadFile(const char *filename)
	{
		/* Загрузка потока видео-файла h264 в буфер */
		
		if(filename == NULL)
		{
			printf("h264LoadFile(): invalid argument");
			return -1;
		}
		
		int fd, bufsize;
		struct stat file_info;

		fd = open(filename, O_RDONLY);
		
		if(fd < 0)
		{
			perror("open()");
			return -1;
		}
		
		if(fstat(fd, &file_info) < 0)
		{
			perror("fstat()");
			return -1;
		}
		
		bufsize = (int) file_info.st_size + 1;	
		buf_pos = (uint8_t *) calloc(bufsize, sizeof(uint8_t));

		if(*buf_pos == nullptr)
		{
			printf("H264::loadFile(): allocation error");
			return -1;
		}

		if(read(fd, buf_pos, bufsize) < 0)
		{
			perror("read");
			return -1;
		}

		buf_pos[bufsize] = '\0';
		buf_end = buf_pos + bufsize;

		return bufsize;
	}
