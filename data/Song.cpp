#include "Song.h"
#include "Ksf.h"
#include "../video/SurfaceStore.h"
#include "../video/Surface.h"
#include <sys/types.h>
// #include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <io.h>
#endif // _WIN32

using std::ifstream;

Song::Song(void)	:	m_playMode( ePM_None ),
    m_pDiscImg( 0 )
{
}

Song::~Song(void)
{
}

// read ksf file from file.
bool Song::ReadKsf( const int & index, const string & filename )
{
	if( _readKsf( filename, m_ksf[index] ) == false )
        return false;

	if( !_getFullPath( "title.bmp", m_BGImgPath ) )
	    _getFullPath( "back.bmp", m_BGImgPath );

    if( !_getFullPath( "song.mp3", m_songPath ) ) {
	   if( !_getFullPath( "song.wav", m_songPath ) )
	        _getFullPath( "song.mpg", m_songPath );
    }

	_getFullPath( "intro.wav", m_introWavPath );
	_getFullPath( "intro.mp3", m_introMp3Path );

	_loadDiscImg( "Disc.bmp" );
	return true;
}

string Song::GetIntroPath() const
{
    if( m_introMp3Path.length() )
        return m_introMp3Path;

    if( m_introWavPath.length() )
        return m_introWavPath;

    return m_introMp3Path;
}

string Song::GetSongPath() const
{
    return m_songPath;
}

string Song::GetBGImgPath() const
{
    return m_BGImgPath;
}


// Get the Full file path.
bool Song::_getFullPath( const string & inFileName, string & outFileName )
{
    // Get Current Directory.
	char    curDir[256] = { 0, };
	if( getcwd( curDir, sizeof( curDir ) ) == NULL )
		return false;

    // get ignore filename
	string realFileName;
	_findFile(inFileName, realFileName );

    // fullpath = (current_directory + '/' + filename)
	outFileName = string(curDir) + "/" + realFileName;

    if( ::access( outFileName.c_str(), 04) != 0 ) {
        outFileName.clear();
        return false;
    }

	return true;
}

bool Song::_readKsf( const string & filename, Ksf & ksf )
{

	ifstream	inFile( filename.c_str() );
	if( !inFile )
		return false;

	bool ret = ksf.ReadFromStrem( inFile );
    inFile.close();

    return ret;
}

// load music disc image.
bool Song::_loadDiscImg( const string & filename )
{
	string	imgFullPath;
	_getFullPath( filename, imgFullPath );

    // if File existed.
    if( ::access( imgFullPath.c_str(), 04 ) == 0 ) {
        // load disc image.
	    m_pDiscImg = g_pSurfaceStore->Order();
	    m_pDiscImg->Load( filename );
        m_pDiscImg->SetColorKey();
    }
    // if file didnot exist.
    else {
        // set default no disc image.
        m_pDiscImg = g_pSurfaceStore->Find( "no_disc" );
    }

    return true;
}


#ifdef _WIN32
// find absolutely file name.
// ��ҹ��� ������ Ȯ���� ������ ã�´�.
// �����쿡���� ��ұ����� �ʿ� ����.
bool Song::_findFile( const string & inFilename, string & outFiename )
{
	outFiename = inFilename;
	return ( ::access( inFilename.c_str(), 04 ) == 0 );
}

#else // _WIN32

#include <sys/types.h>
#include <dirent.h>
#include <strings.h>

// find absolutely file name.
// ���н������� ��ҹ��ڰ� ���еǱ⶧���� ������ ������ ã�� �̸� �˾Ƴ����Ѵ�.
bool Song::_findFile ( const string & inFilename, string & outFiename )
{
	struct dirent * item;
	DIR * dp;
	// char    cPathStr[PATH_LEN+1] = { 0, };

	dp = opendir ( "." );
	bool	bFound = false;
	if ( dp != NULL )
	{
		while ( true )
		{
			item = readdir ( dp );
			if ( item == NULL )
				break;

			if ( strcasecmp ( item->d_name, inFilename.c_str() ) == 0 )
			{
				outFiename = item->d_name;
				bFound = true;
				break;
			}
		}
	}

	closedir ( dp );

	if ( !bFound )
	{
		std::stringstream	str;
		str.str( inFilename );
		str << "directory or file is not found ";
		return false;
	}
	return true;
}

#endif // _WIN32