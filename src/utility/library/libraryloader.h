
#ifndef _LIBRARY_LOADER_H_
#define _LIBRARY_LOADER_H_


#if defined(_WIN32)
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#elif defined(__linux)
#define DLLEXPORT
#define DLLIMPORT
#endif

#include <string>
#include "library.h"
#include <map>

namespace ysl
{
	namespace util
	{
		class LibraryReposity
		{
		public:
			static LibraryReposity * GetInstance();

			/**
			 * \brief Add a library to the repository
			 * \param name
			 */
			void AddLibrary(const std::string & name);

			/**
			 * \brief  return the function pointer by the given name
			 */
			void * GetSymbol(const std::string & name)const;

			/**
			 * \brief  Add the default Library to repository
			 */
			void AddDefaultLibrary();

			/**
			 * \brief Check whether the library exists
			 */
			bool Exists(const std::string & name)const;

		private:
			LibraryReposity();
			static LibraryReposity * instance;
			std::map<std::string, std::shared_ptr<Library>> repo;
		};
	}

}
#endif