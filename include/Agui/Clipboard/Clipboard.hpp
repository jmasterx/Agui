#ifndef AGUI_CLIPBOARD_HPP
#define AGUI_CLIPBOARD_HPP
#include <stdlib.h>
#include <string>
namespace agui
{
	class Clipboard
	{

	public:
		static void copy(const std::string& input);
		static std::string paste();
		Clipboard(void);
		virtual ~Clipboard(void);
	};
}

#endif
