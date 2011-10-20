#ifndef AGUI_OSX_CLIPBOARD_HPP
#define AGUI_OSX_CLIPBOARD_HPP
#include <stdlib.h>
#include <string>
#import <Cocoa/Cocoa.h>
namespace agui
{
	class OSXClipboard
	{
	public:
		static void copy(const std::string& input);
		static std::string paste();
		OSXClipboard(void);
		virtual ~OSXClipboard(void);
	};
}
#endif
