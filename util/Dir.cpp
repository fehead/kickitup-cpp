/*
 * Dir.cpp
 *
 *  Created on: 2013. 3. 28.
 *      Author: chaehoon-win
 */

#include "Dir.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

#ifdef _WIN32
#	include <io.h>
#else
#	include <dirent.h>
#	include <strings.h>
#endif //_WIN32

/**
 * 현재 디렉토리 리스트를 얻는다.
 * @param	dirs	디렉토리 리스트.
 * @return	디렉토리 리스트가 있는지 없는지.
 */
bool Dir::GetDirs ( vector<string> & dirs )
{
#ifdef _WIN32	
	struct _finddata_t fd;
	intptr_t dp = _findfirst(".", &fd);

	int ret = 0;
	while (ret != -1) {
		printf("파일명 : %s, 크기:%d\n", fd.name, fd.size);
		if (fd.name[0] == '.') {
			ret = _findnext(dp, &fd);
			continue;
		}

		// is dir?
		if (fd.attrib & _A_SUBDIR) {
			string	dirname(fd.name);
			dirs.push_back(dirname);
		}

		ret = _findnext(dp, &fd);
	}

	_findclose(dp);

	if (dirs.empty()) {
		std::cerr << "GetDirs() - directory or file is not found\n";
		return false;
	}
	return true;

#else
	DIR * dp = opendir ( "." );

	while ( dp != NULL ) {
		struct dirent * item = readdir ( dp );
		if ( item == NULL )
			break;

		if ( item->d_name[0] == '.' )
			continue;

		// is dir?
		struct stat stat_p;
		if ( stat ( item->d_name, &stat_p ) == -1 )
			continue;
		if ( !S_ISDIR ( stat_p.st_mode ) )
			continue;

		string	dirname ( item->d_name );
		dirs.push_back ( dirname );
	}
	closedir ( dp );

	if ( dirs.empty() ) {
		std::cerr << "GetDirs() - directory or file is not found\n";
		return false;
	}
	return true;
#endif // _WIN32
}
