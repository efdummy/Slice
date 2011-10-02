// Slice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/utime.h>

#define IO_BLOCK_SIZE 512
#define FILE_NAME_SIZE 125
#define SEPARATOR L"\n________________________________________________________________________"

void pub()
{
	wprintf(SEPARATOR);
	wprintf(L"\n");
	wprintf(L"\nIf you like slice and have any question go to http://slice.codeplex.com");
	wprintf(SEPARATOR);
	wprintf(L"\n");
}

void usage()
{
	pub();
	wprintf(L"\nSLICE cut your big files in chunks or rebuild them from the chunks.\n");
	wprintf(L"\nSyntax to slice   : slice filename [size_of_chunks]");
	wprintf(L"\nSyntax to unslice : slice filename.chunk.1");
	wprintf(L"\nDefault value for size_of_chunks is 5 Mo");
	wprintf(L"\n");
	wprintf(L"\nTo slice the 10 Mo bigvideo.mp4 file in two 5 Mo chunks:");
	wprintf(L"\nC:\\> slice bigvideo.mp4");
	wprintf(L"\n");
	wprintf(L"\nTo slice the 10 Mo bigvideo.mp4 file in five 2 Mo chunks:");
	wprintf(L"\nC:\\> slice bigvideo.mp4 2000000");
	wprintf(L"\n");
	wprintf(L"\nTo rebuild the bigvideo.mp4 from the chunks:");
	wprintf(L"\nC:\\> slice bigvideo.mp4.chunk.1");
	wprintf(L"\n");
	wprintf(L"\nNB : all the chunks must be in the same folder");
	wprintf(SEPARATOR);
}

#define CHUNK         L".chunk."

void BuildChunkFileName(_TCHAR *szFile, int iChunkNumber, _TCHAR *szChunkFileName)
{
	_TCHAR  szFileNum[4];

	wcscpy(szChunkFileName, szFile);
	wcscat(szChunkFileName, CHUNK);
	_itow(iChunkNumber, szFileNum, 10);
	wcscat(szChunkFileName, szFileNum);
}

void BuildChunkFileNameFromChunk(_TCHAR *szFile, int iChunkNumber, _TCHAR *szChunkFileName)
{
	_TCHAR  szChunkNum[4];
	_TCHAR  *chunkStart;
	size_t  n;

	chunkStart=wcsstr(szFile, CHUNK);
	n=chunkStart-szFile;
	wcsncpy(szChunkFileName, szFile, n);
	szChunkFileName[n]=0;
	wcscat(szChunkFileName, CHUNK);
	_itow(iChunkNumber, szChunkNum, 10);
	wcscat(szChunkFileName, szChunkNum);
}

void BuildFileNameFromChunk(_TCHAR *szChunkFile, _TCHAR *szFileName)
{
	_TCHAR  *chunkStart;
	size_t  n;

	chunkStart=wcsstr(szChunkFile, CHUNK);
	n=chunkStart-szChunkFile;
	wcsncpy(szFileName, szChunkFile, n);
	szFileName[n]=0;
}

// Set ut structure with file's last access and last write time
void setFileUpdateTime(_TCHAR * file, struct _utimbuf *ut)
{
	struct tm* lastwritetime;		// time structure
	struct _stat attrib;			// file attribute structure
    _wstat(file, &attrib);		    // get the file's attributes
	ut->actime=attrib.st_atime;
	ut->modtime=attrib.st_mtime;
}

void slice(_TCHAR *szFile, unsigned long lChunkMax)
{
	FILE * input;
	FILE * output;
	char   buffer[IO_BLOCK_SIZE];
	_TCHAR   szCurrentChunkFile[FILE_NAME_SIZE];
	unsigned int    flContinue, nbChar, iCurrentChunkNum, iBytesInCurrentChunk, lBytes;

	flContinue=1;

    // Open in byte mode
	if( (input  = _wfopen( szFile, L"rb" )) == NULL ) {
      wprintf( L"\nError %d while opening %s\n", errno, szFile);
	  flContinue=0;
	}

	// Initialize chunk file name
	iCurrentChunkNum=1;
	BuildChunkFileName(szFile, iCurrentChunkNum, szCurrentChunkFile);

    if (flContinue) {
		// Memorize file's last date and time in _utimbuf structure
		struct _utimbuf ut;
		setFileUpdateTime(szFile, &ut);

     	wprintf(L"\nI'm slicing %s in %d bytes chunks, please wait...", szFile, lChunkMax);

		output=_wfopen(szCurrentChunkFile, L"wb");
    	iBytesInCurrentChunk=0;
		lBytes=0;
		wprintf(L"\nSlice %s...", szCurrentChunkFile);

        while ((nbChar = fread(buffer, sizeof(char), IO_BLOCK_SIZE, input))>0) {
			iBytesInCurrentChunk+=nbChar;
			if ((iBytesInCurrentChunk<lChunkMax) || (iBytesInCurrentChunk==nbChar))
				fwrite(buffer, sizeof(char), nbChar, output);
			else {
				// New chunk file name needed
				++iCurrentChunkNum;
				// Set new chunck fine name
				BuildChunkFileName(szFile, iCurrentChunkNum, szCurrentChunkFile);
				// Close old chunk and create new
				fclose(output);
				output=_wfopen(szCurrentChunkFile, L"wb");
        		wprintf(L"\nSlice %s...", szCurrentChunkFile);
				// Write last nbChar
                fwrite(buffer, sizeof(char), nbChar, output);
            	iBytesInCurrentChunk=nbChar;
			}
			lBytes+=nbChar;
		}
		fclose(input);
		fclose(output);

		// Set chuncks last access and last write time
		for (int i=0;i<=iCurrentChunkNum;i++)
		{
			BuildChunkFileName(szFile, i, szCurrentChunkFile);
			// Set file's attributes only if file exists
			if (_waccess_s(szCurrentChunkFile, 0)!=ENOENT) _wutime(szCurrentChunkFile, &ut);
		}

		wprintf(L"\nEverything sliced (%d chunks & %d bytes).\n", iCurrentChunkNum, lBytes);
	}
}

//_______ USLICE

void unslice(_TCHAR *szChunkFile)
{
	FILE *input;
	FILE *output;
	char  buffer[IO_BLOCK_SIZE];
	_TCHAR  szCurrentChunkFile[FILE_NAME_SIZE];
	_TCHAR  szCurrentFile[FILE_NAME_SIZE];
	unsigned int   flContinue, nbChar, iCurrentChunkNum;
	long  lBytes;

	flContinue=1;


    // Open in byte mode
    BuildFileNameFromChunk(szChunkFile, szCurrentFile);
	if( (output  = _wfopen( szCurrentFile, L"wb" )) == NULL ) {
      wprintf(L"\nError %d while opening %s\n", errno, szCurrentFile);
	  flContinue=0;
	}

    if (flContinue) {

		// Initialize chunk file name
		iCurrentChunkNum=1;
		BuildChunkFileNameFromChunk(szChunkFile, iCurrentChunkNum, szCurrentChunkFile);
	    // Memorize file's last date and time in _utimbuf structure
		struct _utimbuf ut;
		setFileUpdateTime(szCurrentChunkFile, &ut);

     	wprintf(L"\nI'm unslicing %s, please wait...", szChunkFile);
		// Open first chunk
		input=_wfopen(szCurrentChunkFile, L"rb");
		wprintf(L"\nSlice %s...", szCurrentChunkFile);
		lBytes=0;

		// While there are chunks
		while (flContinue) {

	       while ((nbChar = fread(buffer, sizeof(char), IO_BLOCK_SIZE, input))>0) {
  				lBytes+=nbChar;
				fwrite(buffer, sizeof(char), nbChar, output);
			}
             
		    // New chunk file name
			++iCurrentChunkNum;
			BuildChunkFileNameFromChunk(szChunkFile, iCurrentChunkNum, szCurrentChunkFile);

			// Close old chunk and opent next
			fclose(input);

			// If next chunk exists
			if (_waccess_s(szCurrentChunkFile, 0)!=ENOENT)
			{
				input=_wfopen(szCurrentChunkFile, L"rb");
				wprintf(L"\nSlice %s...", szCurrentChunkFile);
			}
			else
				flContinue=0;
		}

		fclose(output);

		// Set file's last update time
		if (_waccess_s(szChunkFile, 0)!=ENOENT) _wutime(szCurrentFile, &ut);

		wprintf(L"\nEverything unsliced (%ld bytes).\n", lBytes);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int rc=0;
	switch (argc)
	{
	case 1:
	case 0:
		usage();
		break;
	case 2:
		// Looking for CHUNK marker in file name
		if (wcsstr(argv[1], CHUNK)==NULL)
			slice(argv[1], 5000000);
		else
			unslice(argv[1]);
		break;
	case 3:
		// Size of chunks is specified
		slice(argv[1], _wtol(argv[2]));
		break;
	default:
		usage();
		break;
	}
	return 0;
}