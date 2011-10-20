#ifndef AGUI_WIN_CLIPBOARD_HPP
#define AGUI_WIN_CLIPBOARD_HPP
#include <stdlib.h>
#include <string>
namespace agui
{
	class WinClipboard
	{	
		static std::string _winUTF16ToUTF8(const std::wstring& input);
		static std::wstring _winUTF8ToUTF16(const std::string& input);
	public:

		static void copy(const std::string& input);
		static std::string paste();
		WinClipboard(void);
		virtual ~WinClipboard(void);
	};
}

#endif
