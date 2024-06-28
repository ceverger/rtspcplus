#ifndef RTSP_GENERATOR_HPP
#define RTSP_GENERATOR_HPP

	class RTSPGenerator 
	{

		public:

			enum { ressize = 1024 }; // Максимальный размер буфера для 
											 // сохранения результата генерации
			
		protected:

			int reslen;			       // Размер полезных данных в буфере
			char result[ressize];    // Буфер для сохранения результата генерации
			const char *errorString; // Указатель на строку с ошибкой

		public:
			
			/* Конструкторы и деструкторы класса */

				RTSPGenerator();
				virtual ~RTSPGenerator() {}

			/* Селекторы класса */
		
				int getReslen() const { return reslen; }
				const char *getResult() const { return result; }
				const char *getErrstr() const { return errorString; }

			/* Методы для добавления полей в rtsp-сообщения */

				 int addCSeq(int cseq);
				void addDate(const char *date);

			/* Другие методы класса */
				
				void reset();

	}; // RTSPGenerator

#endif // RTSP_GENERATOR_HPP
