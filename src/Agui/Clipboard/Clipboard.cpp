#include "Agui/Clipboard/Clipboard.hpp"

#if defined(_WIN32)
#include "Agui/Clipboard/WinClipboard.hpp"
#elif defined(__APPLE__)
#include "Agui/Clipboard/OSXClipboard.hpp"
#else
#endif

namespace agui
{
	Clipboard::Clipboard(void)
	{
	}

	Clipboard::~Clipboard(void)
	{
	}

	void Clipboard::copy( const std::string& input )
	{

#if defined(_WIN32)
		WinClipboard::copy(input);
#elif defined(__APPLE__)
		OSXClipboard::copy(input);
#else
#endif
	}

	std::string Clipboard::paste()
	{
#if defined(_WIN32)
		return WinClipboard::paste();
#elif defined(__APPLE__)
		return OSXClipboard::paste();
#else
		return std::string("Failed");
#endif

	}

}
