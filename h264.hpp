#include <cstdint>

#ifndef H264_HPP
#define H264_HPP

	class H264NalUnit
	{
		private:

			uint8_t header;       // Заголовок nal unit
			uint8_t *m_pos; 		 // Начальная позиция буфера с nal unit, в котором хранится поток h264
			uint8_t *m_end; 		 // Конечная позиция буфера с nal unit, в котром хранится поток h264
			uint8_t *m_startCode; // Указатель на начальную позиция стартового кода

		public:

			/* Конструкторы и деструкторы класса */

				H264NalUnit();
				virtual ~H264NalUnit() {}

			/* Селекторы класса */

				    int  getSize()         const { return m_end - m_pos; 		}
				uint8_t *getPos()          const { return m_pos;         		}
				uint8_t *getEnd()          const { return m_end;         	   }
				uint8_t *getStartCode()    const { return m_startCode;   		}
				    int  getStartCodeLen() const { return m_pos - m_startCode; }

				uint8_t getForbiddenBit() const;
				uint8_t getReferenceIDC() const;
				uint8_t getPayloadType()  const;

			/* Модификаторы класса */

				void setPos(uint8_t *pos)             { m_pos = pos;             }
				void setEnd(uint8_t *end)             { m_end = end;             }
				void setStartCode(uint8_t *startCode) { m_startCode = startCode; }

			/* Другие методы класса */

				int payload(char *buf, int bufsize);

				void init();
				void clear();
	
	}; // class H264NalUnit


	class H264AccessUnit
	{
		private:

			uint8_t *m_pos; // Начальная позиция буфера с access unit, в котором хранится поток h264
			uint8_t *cur;   // Текущая позиция буфера с access unit, в котором хранится поток h264
			uint8_t *m_end; // Конечная позиция буфера с access unit, в котром хранится поток h264

			int naluCount; // Количество nal unit в одном access unit
		
		public:

			/* Конструкторы и деструкторы класса */

				H264AccessUnit();
				virtual ~H264AccessUnit() {}
		
			/* Селекторы класса */

				uint8_t *getPos()       const { return m_pos;         }
				uint8_t *getEnd()       const { return m_end;         }
				    int  getSize()      const { return m_end - m_pos; }
				    int  getNaluCount() const { return naluCount;     }

			/* Модификаторы класса */

				void setPos(uint8_t *pos) { m_pos = pos; }
				void setEnd(uint8_t *end) { m_end = end; }

			/* Другие методы класса */

				void parseNalUnit(H264NalUnit & nu);
				void init();
				void reset();
				void clear();

	}; // class H264AccessUnit


	class H264Parser
	{
		private:

			uint8_t *cur; // Текущая позиция буфера, в котором хранится поток h264

			int auCount;        // Количество access unit в буфере
			const char *errstr; // Указатель на строку с текстом ошибки

		public:

			/* Конструкторы и деструкторы класса */

				H264Parser();
				virtual ~H264Parser() {}

			/* Селекторы класса */

				int getAUCount() const { return auCount; }

			/* Другие методы класса */

				int loadFile(const char *filename);
				void parseAccessUnit(H264AccessUnit & au);

				void init();
				void reset();
				void clear();
		
	}; // class H264Parser

	
	class H264AUPacker
	{
		private:

			int count;     // Текущее количество фреймов в access unit
			int m_maxSize; // Максимальный размер фрейма

			uint8_t *pos; // Начальная позиция буфера с access unit
			uint8_t *end; // Конечная позиция буфера с access unit

			uint8_t *сache_pos; // Начальная позиция буфера для сохранения фрейма
			uint8_t *cache_end; // Конечная позиця буфера для сохранения фрейма
		
		public:

			// Констуркторы  и деструкторы класса

				H264AUPacker();
				virtual ~H264AUPacker() {}

			// Селекторы класса
 
				int getCount() const { return count; }
				int getMaxSize() const { return m_maxSize; }

			// Модификаторы класса
			
				void setMaxSize(int maxSize);
				void setAU(H264AccessUnit & nu);

			// Другие методы класса

				int naluFraming(uint8_t buf, int bufsize);

				void init();
				void clear();

	}; // class H264AUPacker


	class H264AUUnpacker
	{
		
	}; // H264AUUnpacker

	
	uint8_t H264GetForbiddenBit(uint8_t header);
	uint8_t H264GetReferenceIDC(uint8_t header);
	uint8_t H264GetPayloadType(uint8_t header);

	uint8_t H264SetForbiddenBit(uint8_t forbiddenBit, uint8_t header);
	uint8_t H264SetReferenceIDC(uint8_t referenceIDC, uint8_t header);
	uint8_t H264SetPayloadType(uint8_t payloadType, uint8_t header);

#endif // H264_HPP
