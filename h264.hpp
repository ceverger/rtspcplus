#include <cstdint>

#ifndef H264_HPP
#define H264_HPP

	class H264NalUnit
	{
		private:

			uint8_t *pos; // Начальная позиция буфера с nal unit, в котором хранится поток h264
			uint8_t *end; // Конечная позиция буфера с nal unit, в котром хранится поток h264

			uint8_t header; // Заголовок nal unit

		public:

			/* Конструкторы и деструкторы класса */

				H264NalUnit();
				virtual ~H264NalUnit() {}

			/* Селекторы класса */

				int getSize() const;
				uint8_t *getPosition() const;
				uint8_t *getPayload() const;
				int getPayloadLen() const;

				uint8_t getForbiddenBit() const;
				uint8_t getReferenceIDC() const;
				uint8_t getPayloadType() const;

				friend uint8_t HDRGetForbiddenBit(uint8_t header);
				friend uint8_t HDRGetReferenceIDC(uint8_t header);
				friend uint8_t HDRGetPayloadType(uint8_t header);

			/* Модификаторы класса */

				void setPosition(uint8_t *position);

				friend uint8_t HDRSetForbiddenBit(uint8_t forbiddenBit, uint8_t header);
				friend uint8_t HDRSetReferenceIDC(uint8_t referenceIDC, uint8_t header);
				friend uint8_t HDRSetPayloadType(uint8_t payloadType, uint8_t header);

			/* Другие методы класса */

				bool init();
				void reset();
	
	}; // class H264NalUnit


	class H264AccessUnit
	{
		private:

			uint8_t *pos; // Начальная позиция буфера с access unit, в котором хранится поток h264
			uint8_t *cur; // Текущая позиция буфера с access unit, в котором хранится поток h264
			uint8_t *end; // Конечная позиция буфера с access unit, в котром хранится поток h264

			int naluCount; // Количество nal unit в одном access unit
		
		public:

			/* Конструкторы и деструкторы класса */

				H264AccessUnit();
				virtual ~H264AccessUnit() {}
		
			/* Селекторы класса */

				int getSize() const;
				uint8_t *getPosition() const;

			/* Модификаторы класса */

				void setPosition(uint8_t *position);

			/* Другие методы класса */

				int parseNalUnit(H264NalUnit & nu);
				void init();

	}; // class H264AccessUnit


	class H264Parser
	{
		private:

			uint8_t *pos; // Начальная позиция буфера, в котором хранится поток h264
			uint8_t *cur; // Текущая позиция буфера, в котором хранится поток h264
			uint8_t *end; // Конечная позиция буфера, в котром хранится поток h264

			char *errstr; // Указатель на строку с текстом ошибки

		public:

			/* Конструкторы и деструкторы класса */

				H264Parser();
				virtual ~H264Parser() {}

				void parseAU(H264AccessUnit & au);

			/* Другие методы класса */
				
				int loadFile(const char *filename, uint8_t **buffer);
				int getAUCount() const;
				int getNALUCount() const;
				void reset();
		
	}; // class H264Parser


#endif // H264_HPP
