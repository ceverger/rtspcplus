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

		if(buf_end == nullptr || buf == nullptr) return false;
		if(buf_end - buf <= 3) return false;
		
		return (buf[0] == 0 && buf[1] == 0 && buf[2] == 1) ? true : false;
	}

	static bool startcode4(uint8_t *buf)
	{
		/* Поиск в видео-файле кодека h264 стартового кода длиной 4 байта */

		if(buf_end == nullptr || buf == nullptr) return false;
		if(buf_end - buf <= 4) return false;

		return (buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 1) ? true : false;
	}

	uint8_t H264GetPayloadType(uint8_t header)
	{
		return header & 0x1F;
	}

	H264NalUnit::H264NalUnit()
	{
		header = 0;

		m_pos       = nullptr;
		m_end       = nullptr;
		m_startCode = nullptr;
	}

	uint8_t H264NalUnit::getForbiddenBit() const
	{
		return header >> 7;
	}

	uint8_t H264NalUnit::getReferenceIDC() const
	{
		return (header >> 5) & 0x03;
	}

	uint8_t H264NalUnit::getPayloadType() const
	{
		return header & 0x1F;
	}

	int H264NalUnit::payload(char *buf, int bufsize)
	{
		if(m_pos == nullptr || m_end == nullptr) return -1;

		int i = 0;
		uint8_t *cur = m_pos;
		
		while(i < bufsize && cur != m_end)
		{
			*buf++ = *cur++;
			 i++;
		}

		return i;
	}

	void H264NalUnit::init()
	{
		if(m_startCode == nullptr) return;
		if(m_pos == nullptr || m_end == nullptr) return;
		if(startcode3(m_startCode) == false && startcode4(m_startCode) == false) return;

		header = *m_pos;
	}

	void H264NalUnit::clear()
	{
		m_pos       = nullptr;
		m_end       = nullptr;
		m_startCode = nullptr;		
	}

	H264AccessUnit::H264AccessUnit()
	{
		m_pos     = nullptr;
		cur       = nullptr;
		m_end     = nullptr;
		naluCount = 0;
	}

	void H264AccessUnit::parseNalUnit(H264NalUnit & nu)
	{
		if(cur == nullptr) return;
	 	if(m_pos == nullptr || m_end == nullptr) return;
		if(startcode3(cur) == false && startcode4(cur) == false) return;
		
		nu.setStartCode(cur);
		while(*cur++ != 1);
		nu.setPos(cur);

		while(cur != m_end)
		{
			if(startcode3(cur) || startcode4(cur)) break;
			cur++;
		}

		nu.setEnd(cur);
		nu.init();
	}

	void H264AccessUnit::init()
	{
		if(buf_pos == nullptr) return;
	 	if(m_pos == nullptr || m_end == nullptr) return;

		cur = m_pos;
		naluCount = 0;

		while(cur != m_end)
		{
			if(startcode3(cur) || startcode4(cur)) 
			{
				while(*cur++ != 1);
				naluCount++;
			}
			else cur++;
		}

		cur = m_pos;
	}

	void H264AccessUnit::reset()
	{
		cur = m_pos;
		naluCount = 0;		
	}

	void H264AccessUnit::clear()
	{
		cur   = nullptr;
		naluCount = 0;
	}

	H264Parser::H264Parser()
	{
		/* Конструктор класса по умолчанию */

		cur     = nullptr;
		auCount = 0;
		errstr  = nullptr;
	}


	int H264Parser::loadFile(const char *filename)
	{
		/* Загрузка потока видео-файла h264 в буфер */
		
		if(filename == nullptr)
		{
			printf("H264loadFile(): invalid argument");
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

		if(buf_pos == nullptr)
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

	void H264Parser::parseAccessUnit(H264AccessUnit & au)
	{
		if(buf_pos == nullptr || cur == nullptr) return;
		if(startcode3(cur) == false && startcode4(cur) == false) return;

		uint8_t header;
		uint8_t *temp = cur;

		while(cur != buf_end && *cur != 1) cur++;
		if(cur == buf_end) return;
		cur++;

		header = *cur;
		if(H264GetPayloadType(header) != 9 && H264GetPayloadType(header) != 10) return;
		au.setPos(temp);

		while(cur != buf_end)
		{
			if(startcode3(cur) || startcode4(cur))
			{
				temp = cur;
				while(*cur++ != 1);

				header = *cur;
				if(H264GetPayloadType(header) == 9 || H264GetPayloadType(header) == 10)
				{
					au.setEnd(temp);	
					cur = temp;				
					break;
				}
			}

			cur++;
		}

		if(cur == buf_end) au.setEnd(buf_end);
		au.init();
	}

	void H264Parser::init()
	{
		if(buf_pos == nullptr) return;

		cur = buf_pos;

		if(startcode3(cur) == false && startcode4(cur) == false) return;

		uint8_t header;
		
		while(cur != buf_end)
		{
			if(startcode3(cur) || startcode4(cur))
			{
				while(*cur++ != 1);

				header = *cur;
				if(H264GetPayloadType(header) == 9 || H264GetPayloadType(header) == 10) auCount++;
			}

			cur++;
		}

		cur = buf_pos;
	}

	void H264Parser::reset()
	{
		cur = buf_pos;
		auCount = 0;
		errstr = nullptr;
	}

	void H264Parser::clear()
	{
		cur = nullptr;
		auCount = 0;
	}

	H264AUPacker()::H264AUPacker()
	{
		count = 0;
		m_maxSize = 0;

		pos = nullptr;
		end = nullptr;

		cache_pos = nullptr;
		cache_end = nullptr;
	}

	void H264AUPacker::setMaxSize(int maxSize)
	{
		if(count != 0) return;
		if(pos != nullptr || end != nullptr) return;

		m_maxsize = maxSize;
	}

	void H264AUPacker::setAU(H264AccessUnit & au)
	{
		if(count != 0) return;
		if(pos != nullptr || end != nullptr) return;

		pos = nu.getPos();
		end = nu.getEnd();
	}

	void H264AUPacker::init()
	{
		if(m_maxSize == 0) return;
		if(pos != nullptr || end != nullptr) return;
			
		
	}

	void H264AUPacker()::clear()
	{
		count = 0;
		m_maxSize = 0;

		pos = nullptr;
		end = nullptr;

		cache_pos = nullptr;
		cache_end = nullptr;		
	}
