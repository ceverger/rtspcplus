#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "rtsp.hpp"

	static bool checkMethod(const char *method)
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

	static bool checkURI(const char *uri)
	{
		if(uri == nullptr) return false;
		if(strlen(uri) == 0) return false;

		if(*uri == '*') return true;
		if(strstr(uri, "rtsp://") != nullptr) return true;

		return false;
	}

	static const char *searchMethod(const char *str)
	{
		const char *pos = strstr(str, "OPTION");
		if(pos == nullptr) pos = strstr(str, "DESCRIBE");
		if(pos == nullptr) pos = strstr(str, "ANNOUNCE");
		if(pos == nullptr) pos = strstr(str, "SETUP");
		if(pos == nullptr) pos = strstr(str, "PLAY");
		if(pos == nullptr) pos = strstr(str, "PAUSE");
		if(pos == nullptr) pos = strstr(str, "TEARDOWN");
		if(pos == nullptr) pos = strstr(str, "GET_PARAMETER");
		if(pos == nullptr) pos = strstr(str, "SET_PARAMETER");
		if(pos == nullptr) return nullptr;

		return pos;
	}

	static const char *searchURI(const char *str)
	{
		const char *pos = strstr(str, "*");

		if(pos == nullptr)
		{	
			pos = strstr(str, "rtsp://");
		}

		return pos;
	}

	static const char *searchVernum(const char *str)
	{
		const char *pos = strstr(str, "RTSP/");
		return pos;
	}

	namespace Rtsp
	{
		Request::Request() : Buffer() 
		{
			// Конструктор по умолчанию
		}

		Request::Request(char *buf, int bufsize) : Buffer((uint8_t *)buf, bufsize)
		{
			// Пользовательский конструктор
		}

		Request::Request(const Array & array) : Buffer(array)
		{
			// Констуктор копирования
		}

		void Request::getMethod(char *buf, int bufsize)
		{
			// Получение имени метода

				if(buf == nullptr) return;
				if(isInit()) *cur = '\0'; else return;

				int i = 0;
				const char *pos = searchMethod((const char *) m_pos);

				bzero(buf, bufsize);

				if(pos == nullptr) return;

				while(i < bufsize && *pos != '\0' && *pos != ' ')
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';
		}

		void Request::getURI(char *buf, int bufsize)
		{
			// Получение uri ресурса

				if(buf == nullptr) return;
				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((const char *) m_pos)) return;

				int i = 0;
				const char *pos = searchURI((const char *) m_pos);

				bzero(buf, bufsize);

				if(pos == nullptr) return;
	
				while(i < bufsize && *pos != '\0' && *pos != ' ')
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';
		}

		void Request::getLogin(char *buf, int bufsize)
		{
			if(buf == nullptr) return;
			if(isInit()) *cur = '\0'; else return;
			if(!searchMethod((const char *) m_pos)) return;

			int i = 0;
			const char *pos, *end, *temp;

			bzero(buf, bufsize);

			pos = searchURI((const char *) m_pos);
			if(pos == nullptr || *pos == '*') return;
			end = (const char *) cur;

			temp = pos;

			while(*pos != ' ' && pos < end) pos++;
			if(pos == end) return;
			end = pos;

			pos = temp;
			while(*pos !=  '/' && pos < end) pos++;

			pos = temp;
			while(*pos !=  '@' && pos < end) pos++;
			if(pos == end) return;

			end = pos;
			pos = temp + strlen("rtsp://");

			while(*end != ':' && end > pos) end--;
			if(end == pos) return;

			while(i < bufsize && pos < end)
			{
				*buf++ = *pos++;
				 i++;
			}

			*buf = '\0';
		}

		void Request::getPassword(char *buf, int bufsize)
		{
			if(buf == nullptr) return;
			if(isInit()) *cur = '\0'; else return;
			if(!searchMethod((const char *) m_pos)) return;

			int i = 0;
			const char *pos, *end, *temp;

			bzero(buf, bufsize);

			pos = searchURI((const char *) m_pos);
			if(pos == nullptr || *pos == '*') return;
			end = (const char *) cur;

			temp = pos;
			while(*pos != ' ' && pos < end) pos++;
			if(pos == end) return;
			end = pos;

			pos = temp;
			while(*pos !=  '/' && pos < end) pos++;

			pos = temp;
			while(*pos !=  '@' && pos < end) pos++;
			if(pos == end) return;

			end = pos;
			pos = temp + strlen("rtsp://");

			while(*pos != ':' && pos < end) pos++;
			if(pos == end) return;
			pos++;

			while(i < bufsize && pos < end)
			{
				*buf++ = *pos++;
				 i++;
			}

			*buf = '\0';			
		}

		void Request::getHost(char *buf, int bufsize)
		{
			// Получение хоста

				if(buf == nullptr) return;
				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((const char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;
				
				int i = 0;
				const char *pos, *end, *temp;

				bzero(buf, bufsize);

				pos = searchURI((const char *) m_pos);
				if(pos == nullptr || *pos == '*') return;

				temp = pos;
				while(*pos != ' ' && pos < end) pos++;
				if(pos == end) return;
				end = pos;

				pos = temp + strlen("rtsp://");
				while(*pos !=  '/' && pos < end) pos++;
				end = pos;
				
				pos = temp + strlen("rtsp://");
				while(*pos !=  '@' && pos < end) pos++;
				if(*pos == '@') pos++;

				temp = pos;
				while(*pos !=  ':' && pos < end) pos++;
				if(*pos == ':') end = pos;

				pos = temp;
				while(i < bufsize && pos < end)
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';
		}

		int Request::getPort()
		{
			// Получение номера порта

				if(isInit()) *cur = '\0'; else return -1;
				if(!searchMethod((const char *) m_pos)) return -1;
				if(!searchURI((const char *) m_pos)) return -1;

				int i = 0, bufsize = 8;
				char buf[bufsize], *buf_pos;
				const char *pos, *end, *temp;

				bzero(buf, bufsize);

				pos = searchURI((const char *) m_pos);
				if(pos == nullptr || *pos == '*') return 0;
				end = (const char *) m_end;

				temp = pos;
				while(*pos != ' ' && pos < end) pos++;
				end = pos;

				pos = temp + strlen("rtsp://");
				while(*pos !=  '/' && pos < end) pos++;
				end = pos;
				
				pos = temp + strlen("rtsp://");
				while(*pos !=  '@' && pos < end) pos++;
				if(*pos == '@') pos++;

				temp = pos;
				while(*pos !=  ':' && pos < end) pos++;
				if(pos == end) return 0;
				
				pos++;
				buf_pos = buf;
				while(i < bufsize && pos < end)
				{
					*buf_pos++ = *pos++;
					 i++;
				}

				*buf_pos = '\0';

				return strtol(buf, NULL, 10);
		}

		void Request::getName(char *buf, int bufsize)
		{
			// Получение имени ресурса

				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((const char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int i = 0;
				const char *pos, *end, *temp;

				bzero(buf, bufsize);

				pos = searchURI((const char *) m_pos);
				if(pos == nullptr || *pos == '*') return;
				end = (const char *) m_end;

				temp = pos;
				while(*pos != ' ' && pos < end) pos++;
				end = pos;

				pos = temp + strlen("rtsp://");
				while(*pos !=  '/' && pos < end) pos++;
				if(pos == end) return;

				while(i < bufsize && pos < end)
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';
		}

		void Request::getURL(char *buf, int bufsize)
		{
			// Получение url ресурса

				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((const char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int ret, i = 0;
				const char *pos, *end, *temp;

				bzero(buf, bufsize);

				pos = searchURI((const char *) m_pos);
				if(pos == nullptr || *pos == '*') return;
				end = (const char *) m_end;

				ret = snprintf(buf, bufsize, "%s", "rtsp://");
				if(ret < 0) return;
				pos += ret;
				buf += ret;
				bufsize -= ret;

				temp = pos;
				while(*pos !=  '/' && pos < end) pos++;
				end = pos;

				pos = temp;
				while(*pos !=  '@' && pos < end) pos++;
				pos = (pos == end) ? temp : pos + 1;

				while(i < bufsize && *pos != ':' && pos < end)
				{
					*buf++ = *pos++;
					 i++;
				}

				if(*pos == ':') while(pos < end) pos++;

				temp = pos;
				end = (const char *) m_end;
				while(*pos != ' ' && pos < end) pos++;			
				end = pos;
				pos = temp;

				while(i < bufsize && pos < end)
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';																										
		}

		double Request::getVernum()
		{
			// Получение номера версии протокола

				if(isInit()) *cur = '\0'; else return -1;
				if(!searchMethod((const char *) m_pos)) return -1;
				if(!searchURI((const char *) m_pos)) return -1;

				int i = 0;
				int bufsize = 8;
				const char *pos;
				char buf[bufsize], *buf_pos;

			// Поиск в строке c cообщением подстроки с версией протокола
	
				pos = searchVernum((char *) m_pos);
				if(pos == nullptr) return 0;

				buf_pos = buf;

				while(*pos++ != '/');
				
				while(i < bufsize && *pos != '\r')
				{
					*buf_pos++ = *pos++;
					 i++;
				}

				*buf_pos = '\0';

				return atof(buf);
		}

		int Request::getCSeq()
		{
			// Получение номера сообщения

				if(isInit()) *cur = '\0'; else return -1;
				if(!searchMethod((const char *) m_pos)) return -1;
				if(!searchURI((const char *) m_pos)) return -1;
				if(!searchVernum((const char *) m_pos)) return -1;

				int ret, i = 0;
				int bufsize = 8;
				const char *pos;
				char buf[bufsize], *buf_pos;

			// Поиск в строке c cообщением подстроки с номером сообщения

				pos = strstr((char *) m_pos, "CSeq: ");
				if(pos == nullptr) return 0;

				while(*pos++ != ' ');

				buf_pos = buf;
				while(i < bufsize && *pos != '\r')
				{
					*buf_pos++ = *pos++;
					 i++;
				}

				*buf_pos = '\0';

				return atoi(buf);
		}

		void Request::generate(const char *method, const char *uri, double vernum)
		{
			// Генерация основной строки rtsp-запроса

				if(!isInit()) return;
				if(method == nullptr || uri == nullptr) return;
				if(strlen(method) == 0 || strlen(uri) == 0) return;
				if(checkMethod(method) == false || checkURI(uri) == false) return;

				reset();

				int ret;
				uint8_t *pos, *end;

				pos = (uint8_t *) method;
				end = (uint8_t *) (pos + strlen(method));

				while(cur < m_end && pos < end)
				{
					*cur++ = *pos++;
				}

				*cur++ = ' ';
				
				pos = (uint8_t *) uri;
				end = (uint8_t *) (pos + strlen(uri));

				while(cur < m_end && pos < end)
				{
					*cur++ = *pos++;
				}

				*cur++ = ' ';

				ret = snprintf((char *)cur, getFree(), "%s%.1f\r\n", "RTSP/", vernum);
				if(ret < 0) return;

				cur += ret;
		}

		void Request::setMethod(const char *method)
		{
			// Установка имени метода

				if(!isInit()) return;
				if(method == nullptr) return;
				if(!checkMethod(method)) return;

				int ret;
				uint8_t buf[getLen() + 1];
				uint8_t *pos, *first, *next, *buf_end;

			// Поиск метода в сообщении

				if(!searchMethod((const char *) m_pos))
				{
					/* Если метод не присутствует в сообщении, то просто
						записываем его в буфер и выходим из программы */

						pos = (uint8_t *) method;

						while(*pos != '\0' && cur < m_end)
						{
							*cur++ = *pos++;
						}

						*cur++ = ' ';
						*cur = '\0';

						return;
				}

			/* Если метод присутствует в сообщении, сохраняем
			   исходное сообщение в запасном буфере и заменяем
				метод на новый  */

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			// Поиск во временном буфере начала и конца строки с именем метода

				pos = (uint8_t *) searchMethod((const char *) buf);

				first = pos;
				while(*pos != ' ' && pos < buf_end) pos++;
				next = (*pos == ' ') ? pos + 1 : pos;

			// Копируем имя нового метода в основной буфер

				pos = (uint8_t *) method;

				while(*pos != '\0' && cur < m_end)
				{
					*cur++ = *pos++;
				}

				*cur++ = ' ';
				*cur = '\0';

			// Копируем остальную часть сообщения в основной буфер, если она есть

				if(next == buf_end)  return;

				pos = next;

				while(pos < buf_end && cur < m_end)
				{
					*cur++ = *pos++;
				}

				*cur = '\0';
		}

		void Request::setURI(const char *uri)
		{
			// Установка uri ресурса

				if(!isInit()) return;
				if(uri == nullptr) return;
				if(!checkURI(uri)) return;	
				if(!searchMethod((const char *) m_pos)) return;

				int ret;
				uint8_t buf[getLen() + 1];
				uint8_t *pos, *end, *first, *next, *buf_end;

			// Поиск URI в сообщении

				if(!searchURI((const char *) m_pos))
				{
					/* Если URI не присутствует в сообщении, то просто
						записываем его в буфер и выходим из программы */

						pos = (uint8_t *) uri;

						while(*pos != '\0' && cur < m_end)
						{
							*cur++ = *pos++;
						}

						*cur++ = ' ';
						*cur = '\0';

						return;
				}

			/* Если URI присутствует в сообщении, сохраняем
			   исходное сообщение в запасном буфере и заменяем
				URI на новый  */

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			// Поиск во временном буфере начала и конца строки с URI
			
				pos = (uint8_t *) searchURI((const char *) buf);

				first = pos;
				while(*pos != ' ' && pos < buf_end)  pos++;
				next = (*pos == ' ') ? pos + 1 : pos;

			// Копирование в основной буфер предыдущей части сообщения

				pos = buf;
				while(pos < first && cur < m_end)
				{
					*cur++ = *pos++;
				}

			// Копируем имя нового URI в основной буфер

				pos = (uint8_t *) uri;

				while(*pos != '\0' && cur < m_end)
				{
					*cur++ = *pos++;
				}

			// Копируем остальную часть сообщения в основной буфер, если она есть

				if(next == buf_end) 
				{
					*cur++ = ' ';
					*cur = '\0';
				}

				pos = next;

				while(pos < buf_end && cur < m_end)
				{
					*cur++ = *pos++;
				}

				*cur = '\0';
		}

		void Request::setLogin(const char *login)
		{
			// Установка логина пользователя

				if(!isInit()) return;
				if(login == nullptr) return;
				if(!searchMethod((char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int ret;
				uint8_t buf[getLen() + 1];
				uint8_t *pos, *end, *first, *next, *buf_end;

			// Копирование данных из текущего буфера во временный

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			// Поиск во временном буфере начала и конца строки с URI

				pos = (uint8_t *) searchURI((const char *) buf);

				first = pos;
				while(*pos != ' ' && pos < buf_end)  pos++;
				next = (*pos == ' ') ? pos + 1 : pos;
				end = pos;

			// Копирование в основной буфер предыдущей части сообщения

				pos = buf;
				while(pos < first && cur < m_end)
				{
					*cur++ = *pos++;
				}

			// Поиск в строке URI конца имени хоста URI

				pos = first + strlen("rtsp://");
				while(*pos != '/' && pos < end) pos++;
				end = pos;

			// Поиск в строке с URI подстроки с логином и паролем

				pos  = first + strlen("rtsp://");
				while(*pos != '@' && pos < end) pos++;

				if(pos == end)
				{
					/* Если подстрока с логином и паролем отсутствует в 
						строке URI, то создаём её и записываем в основной 
						буфер вместе с основным сообщением. После этого
						выходим из метода */

						pos = first + strlen("rtsp://");
						ret = snprintf((char *) cur, getFree(), "rtsp://%s:@", login);
						cur += ret;

						while(cur < m_end && pos < buf_end)
						{	
							*cur++ = *pos++;
						}

						*cur++ = ' ';
						*cur = '\0';

						return;
				}

				/* Если подстрока с логином в строке URI присутсвует,
					то меняем в ней только логин и записываем изменённую
					строку URI в основной буфер вместе с основным 
					сообщением. */

					end = pos;
					pos  = first + strlen("rtsp://");
					while(*pos != ':' && pos < end) pos++;

					ret = snprintf((char *)cur, getFree(), "rtsp://%s", login);
					cur += ret;

					while(cur < m_end && pos < buf_end)
					{
						*cur++ = *pos++;
					}

					*cur++ = ' ';
					*cur = '\0';
		}

		void Request::setPassword(const char *password)
		{
			// Установка пароля пользователя

				if(!isInit()) return;
				if(password == nullptr) return;
				if(!searchMethod((char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int ret, i = 0;
				uint8_t buf[getLen() + 1];
				uint8_t *pos, *end, *first, *next, *buf_end;

			// Копирование данных из текущего во временный и его очистка

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			// Поиск во временном буфере начала и конца строки с URI

				pos = (uint8_t *) searchURI((const char *) buf);

				first = pos;
				while(*pos != ' ' && pos < buf_end)  pos++;
				next = (*pos == ' ') ? pos + 1 : pos;
				end = pos;

			// Копирование в основной буфер предыдущей части сообщения

				pos = buf;
				while(pos < first && cur < m_end)
				{
					*cur++ = *pos++;
				}

			// Поиск в строке URI конца имени хоста URI

				pos = first + strlen("rtsp://");
				while(*pos != '/' && pos < end) pos++;
				end = pos;

			// Поиск в строке с URI подстроки с логином и паролем

				pos  = first + strlen("rtsp://");
				while(*pos != '@' && pos < end) pos++;

				if(pos == end)
				{
					/* Если подстрока с логином и паролем отсутствует в 
						строке URI, то создаём её и записываем в основной 
						буфер вместе с основным сообщением. После этого
						выходим из метода */

						pos = first + strlen("rtsp://");
						ret = snprintf((char *)cur, getFree(), "rtsp://:%s@", password);
						cur += ret;

						while(cur < m_end && pos < buf_end)
						{	
							*cur++ = *pos++;
						}

						*cur = '\0';

						return;
				}

				/* Если подстрока с логином  и паролем в строке URI ,
					присутсвует то меняем в ней только пароль и записываем
					изменённую строку URI в основной буфер вместе с основным 
					сообщением. */

					next = pos;
					pos  = first + strlen("rtsp://");
					while(*pos != ':' && pos < next) pos++;
					end = (*pos == ':') ? pos + 1 : next;
					
					pos = first;
					while(pos < end && cur < m_end)
					{
						*cur++ = *pos++;
					}

					pos = (uint8_t *) password;
					while(*pos != '\0' && cur < m_end)
					{
						*cur++ = *pos++;
					}

				// Копируем остальную часть сообщения в основной буфер, если она есть

					pos = next;
					while(pos < buf_end && cur < m_end)
					{
						*cur++ = *pos++;
					}

					*cur = '\0';
		}

		void Request::setPort(int port)
		{
			// Установка порта сервера

				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int ret, i = 0;
				uint8_t buf[getLen() + 1], *buf_end;
				uint8_t *pos, *end, *temp, *first, *next;

			// Копирование данных из текущего во временный и его очистка

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			/* Поиск во временном буфере начала и конца строки с именем метода
				и компирование его в основной буфер */

				pos = (uint8_t *) searchMethod((const char *) buf);

				temp = pos;
				while(*pos != ' ' && pos < buf_end)  pos++;

				end = pos + 1;
				pos = temp;

				while(pos < end && cur < m_end)
				{
					*cur++ = *pos++;
				}		

			// Поиск во временном буфере начала и конца строки с URI

				pos = (uint8_t *) searchURI((const char *) buf);
				first = pos;

				temp = pos;
				while(*pos != ' ' && pos < buf_end) pos++;
				end = pos;
				next = end;

				pos = temp + strlen("rtsp://");
				while(*pos != '/' && pos < end) pos++;
				end = pos;

			// Поиск в строке подстроки с URI с логином и паролем

				pos  = temp + strlen("rtsp://");

				temp = pos;
				while(*pos != '@' && pos < end) pos++;

				if(pos == end) pos = temp;

			// Поиск в строке URI подстроки с номером порта

				while(*pos != ':' && pos < end) pos++;
		
			// Копирование в основной буфер строки URI без номера порта
				
				next = end;
				end = pos;
				pos = first;

				while(pos < end && cur < m_end)
				{
					*cur++ = *pos++;
				}
	
				*cur++ = ':';

			// Запись в основной буфер номера порта

				ret = snprintf((char *) cur, getFree(), "%d", port);
				if(ret < 0) return;
				cur += ret;

			// Запись остального сообщения в основной буфер

				pos = next;
				while(pos < buf_end && cur < m_end)
				{
					*cur++ = *pos++;
				}
	
				*cur = '\0';
				
		}

		void Request::setName(const char *name)
		{
			// Установка имени ресурса

				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int ret, i = 0;
				uint8_t buf[getLen() + 1], *buf_end;
				uint8_t *pos, *end, *temp, *first, *next;

			// Копирование данных из текущего во временный и его очистка

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			/* Поиск во временном буфере начала и конца строки с именем метода
				и компирование его в основной буфер */

				pos = (uint8_t *) searchMethod((const char *) buf);

				temp = pos;
				while(*pos != ' ' && pos < buf_end)  pos++;

				end = pos + 1;
				pos = temp;

				while(pos < end && cur < m_end)
				{
					*cur++ = *pos++;
				}

			/* Поиск во временном буфере начала и конца строки с URI
				и компирование его в основной буфер вместе с новым 
				именем ресурса */

				pos = (uint8_t *) searchURI((const char *) buf);
				first = pos;

				temp = pos;
				while(*pos != ' ' && pos < buf_end) pos++;
				end = pos;
				next = end;

				pos = temp + strlen("rtsp://");
				while(*pos != '/' && pos < end) pos++;
				end = pos;

				pos = temp;
				while(pos < end && cur < m_end)
				{
					*cur++ = *pos++;
				}
			
				*cur++ = '/';

			// Запись нового имени в основной буфер

				ret = snprintf((char *) cur, getFree(), "%s", name);
				if(ret < 0) return;
				cur += ret;

				if(pos < buf_end)
				{
					// Запись остального сообщения в основной буфер

						pos = next;
						while(pos < buf_end && cur < m_end)
						{
							*cur++ = *pos++;
						}
				}

				*cur = '\0';
				
				return;						
		}

		void Request::setVernum(double vernum)
		{
			// Установка версии rtsp-протокола

				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int ret, i = 0;
				uint8_t buf[getLen() + 1], *buf_end;
				uint8_t *pos, *end, *temp, *first, *next;

			// Копирование данных из текущего во временный и его очистка

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			/* Поиск во временном буфере начала и конца строки с именем метода
				и копирование его в основной буфер */

				pos = (uint8_t *) searchMethod((const char *) buf);

				temp = pos;
				while(*pos != ' ' && pos < buf_end)  pos++;

				end = pos + 1;
				pos = temp;

				while(pos < end && cur < m_end)
				{
					*cur++ = *pos++;
				}

			/* Поиск во временном буфере начала и конца строки с URI
				и копирование его в основной буфер */

				pos = (uint8_t *) searchURI((const char *) buf);

				temp = pos;
				while(*pos != ' ' && pos < buf_end)  pos++;

				end = (*pos == ' ') ? pos + 1 : pos;
				next = end;
				pos = temp;

				while(pos < end && cur < m_end)
				{
					*cur++ = *pos++;
				}

			// Копирование версии протокола в основной буфер
				
				ret = snprintf((char *)cur, getFree(), "RTSP/%.1f\r\n", vernum);
				if(ret < 0) return;
				cur += ret;

			// Поиск во временном буфере начала строки c версией протокола

				pos = (uint8_t *) searchVernum((const char *) buf);

			/* Если в дополнительном буфере в сообщении отсутсвует строка 
				с версией протокола, то выходим из метода */

				if(pos == nullptr)
				{
					*cur = '\0';
					 return;
				}

			/* Находим конец строки с версией протокола */

				pos = next;
				while(*pos != '\n' && pos < buf_end)  pos++;

				if(pos == buf_end)
				{
					*cur = '\0';
					 return;
				}
	
				pos++;

			// Копируем остальную часть сообщения в буфер

				while(pos < buf_end && cur < m_end)
				{
					*cur++ = *pos++;
				}

				*cur = '\0';	
		}

		void Request::setCSeq(int cseq)
		{
		// Установка номера сообщения

				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((const char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;
				if(!searchVernum((const char *) m_pos)) return;

				int ret, i = 0;
				uint8_t buf[getLen() + 1], *buf_end;
				uint8_t *pos, *end, *temp, *first, *next;

			// Копирование данных из текущего во временный и его очистка

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			// Поиск во временном буфере подстроки с номером сообщения

				pos = (uint8_t *) strstr((const char *) buf, "CSeq: ");
				
			/* Если номер сообщения отсутсвует, то просто добавляем его
				и копируем в основной буфер и  выходим из метода */

				if(pos == nullptr)
				{
					// Копирование в основной буфер первой части текста собщения

						pos = buf;

						while(pos < buf_end && cur < m_end)
						{
							*cur++ = *pos++;
						}

					// Копирование номера сообщения
						
						ret = snprintf((char *) cur, getFree(), "CSeq: %d\r\n", cseq);
						if(ret < 0) return;

						cur += ret;
					  *cur = '\0';

						return;
				}

			// Находим конец подстроки с номером сообщения

				first = pos;
				while(*pos != ' ' && pos < buf_end) pos++;
				next = pos;
				
			// Копируем в основной буфер первую часть сообщения

				pos = buf;
				while(pos < first && cur < m_end)
				{
					*cur++ = *pos++;
				}

			// Копируем в основной буфер новый номер сообщения

				ret = snprintf((char *) cur, getFree(), "CSeq: %d\r\n", cseq);
				if(ret < 0) return;

				cur += ret;
			  *cur = '\0';

			// Копируем в основной буфер вторую часть сообщения, если она есть

				if(next == buf_end) return;

				pos = next;
				while(pos < buf_end && cur < m_end)
				{
					*cur++ = *pos++;
				}

				*cur = '\0';
		}

		void Request::setDate()
		{
			if(!isInit()) return;
				else *cur = '\0';
			if(!searchMethod((const char *)m_pos)) return;
			if(!searchURI((const char *)m_pos)) return;
			if(!searchVernum((const char *)m_pos)) return;

			int ret;
			time_t tm = time(NULL);
			struct tm *gm = gmtime(&tm);		

			ret = strftime((char *)cur, getFree(), "Date: %a, %b %d %Y %T GMT\r\n", gm);
			if(ret < 0) return;

			cur += ret;
		}

		void Request::setPublicMethods(const char *methods)
		{
			if(!isInit()) return; else *cur = '\0';
			if(!searchMethod((const char *) m_pos)) return;
			if(!searchURI((const char *) m_pos))    return;
			if(!searchVernum((const char *) m_pos)) return;

			int ret = snprintf((char *)cur, getFree(), "Public: %s\r\n", methods);
			if(ret < 0) return;

			cur += ret;
		}

		void Request::setMessageEnd()
		{
			if(!isInit()) return; else *cur = '\0';
			if(!searchMethod((const char *) m_pos)) return;
			if(!searchURI((const char *) m_pos))    return;
			if(!searchVernum((const char *) m_pos)) return;

			*cur++ = '\r';
			*cur++ = '\n';
		}

		void Request::getAccept(char *message)
		{
			
		}

		void Request::getAcceptEncoding(char *message)
		{
			
		}

		void Request::getAcceptLanguage(char *message)
		{
			
		}

		void Request::getAuthorization(char *message)
		{
			
		}

		int Request::getBandwidth(char *message)
		{
			return 0;
		}

		int Request::getBlocksize(char *message)
		{
			return 0;
		}

		void Request::getConference(char *message)
		{
			
		}

		void Request::getFrom(char *message)
		{
			
		}

		void Request::getIfModifiedSince(char *message)
		{
			
		}

		void Request::getProxyRequire(char *message)
		{
			
		}

		void Request::getReferer(char *message)
		{
			
		}

		void Request::getReuire(char *message)
		{
			
		}

		void Request::getUserAgent(char *message)
		{
			
		}

		void Request::addCSeq(int cseq)
		{
			
		}

		Response::Response() : Byte::Buffer()
		{
			// Конструктор по умолчанию
		}

		Response::Response(char *buf, int bufsize) : Buffer((uint8_t *) buf, bufsize)
		{
			// Пользовательский конструктор
		}

		Response::Response(const Array & array) : Buffer(array)
		{
			// Констуктор копирования
		}

		void Response::getStatusLine(const char *message)
		{
			
		}

		int Response::getStatusCode(const char *message)
		{
			return 0;
		}

		int Response::getReasonPhrase(const char *message)
		{
			return 0;
		}

		void Response::getAllow(const char *message)
		{

		}

		void Response::getPublic(const char *message)
		{

		}

		void Response::getRetryAfter(const char *message)
		{

		}

		void Response::getRtpInfo(const char *message)
		{
			
		}

		void Response::getUnsupported(const char *message)
		{
			
		}

		void Response::getWWWAuthenticate(const char *message)
		{
			
		}

	}; // namespace Rtsp
