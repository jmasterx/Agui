#include "Agui/Clipboard/WinClipboard.hpp"
#include <windows.h>
namespace agui
{
	WinClipboard::WinClipboard(void)
	{
	}

	WinClipboard::~WinClipboard(void)
	{
	}

	void WinClipboard::copy( const std::string& input )
	{
		LPWSTR  lptstrCopy; 
		HGLOBAL hglbCopy; 
		std::wstring text;

		text = _winUTF8ToUTF16(input);

		// Open the clipboard, and empty it. 

		if (!OpenClipboard(NULL)) 
			return; 

		EmptyClipboard(); 

		// Allocate a global memory object for the text. 
		hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
			((text.length() + 1) * sizeof(WCHAR))); 

		if (hglbCopy == NULL) 
		{ 
			CloseClipboard(); 
			return; 
		} 

		// Lock the handle and copy the text to the buffer. 
		lptstrCopy = (LPWSTR)GlobalLock(hglbCopy); 
		memcpy(lptstrCopy, text.c_str(), 
			(text.length() + 1) * sizeof(WCHAR) ); 
		lptstrCopy[(text.length() + 1) * sizeof(WCHAR)] = (WCHAR) 0;    // null character 
		GlobalUnlock(hglbCopy); 

		// Place the handle on the clipboard. 

		SetClipboardData(CF_UNICODETEXT, hglbCopy); 


		// Close the clipboard. 

		CloseClipboard(); 
	}

	std::string WinClipboard::paste()
	{ 
		HGLOBAL   hglb; 
		LPWSTR    lptstr; 

		std::string result;
		std::wstring input;

		// get the clipboard text. 

		if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) 
			return result;

		if (!OpenClipboard(NULL)) 
			return result; 

		hglb = GetClipboardData(CF_UNICODETEXT); 
		if (hglb != NULL) 
		{ 
			lptstr = (LPWSTR)GlobalLock(hglb); 


			if (lptstr != NULL) 
			{ 
				GlobalUnlock(hglb); 
				input = lptstr;
				result = _winUTF16ToUTF8(input);
			} 
			CloseClipboard(); 
		}
		return result;
	}

	std::string WinClipboard::_winUTF16ToUTF8( const std::wstring& input )
	{
		// get length
		int length = WideCharToMultiByte( CP_UTF8, NULL,
			input.c_str(), input.size(),
			NULL, 0,
			NULL, NULL );
		if( !(length > 0) )
			return std::string();
		else
		{
			std::string result;
			result.resize( length );

			if( WideCharToMultiByte( CP_UTF8, NULL,
				input.c_str(), input.size(),
				&result[0], result.size(),
				NULL, NULL ) > 0 )
				return result;
			else
				return std::string();
		}

	}

	std::wstring WinClipboard::_winUTF8ToUTF16( const std::string& input )
	{
		// get length
		int length = MultiByteToWideChar( CP_UTF8, NULL,
			input.c_str(), input.size(),
			NULL, 0 );
		if( !(length > 0) )
			return std::wstring();
		else
		{
			std::wstring result;
			result.resize( length );

			if( MultiByteToWideChar(CP_UTF8, NULL,
				input.c_str(), input.size(),
				&result[0], result.size()) > 0 )
				return result;
			else
				return std::wstring();
		}

	}

}
