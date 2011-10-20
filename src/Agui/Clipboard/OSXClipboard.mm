#include "Agui/Clipboard/OSXClipboard.hpp"

namespace agui
{
	OSXClipboard::OSXClipboard(void)
	{
	}

	OSXClipboard::~OSXClipboard(void)
	{
	}

	void OSXClipboard::copy( const std::string& input )
	{
	
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

		NSString *string = [[NSString alloc] initWithUTF8String: input.c_str()];
		NSPasteboard *pasteBoard = [NSPasteboard generalPasteboard];
		[pasteBoard declareTypes:[NSArray arrayWithObjects:NSStringPboardType, nil] owner:nil];
		[pasteBoard setString:string forType:NSStringPboardType];

		[pool release];

	}

	std::string OSXClipboard::paste()
	{
		std::string retString;
		
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

		NSPasteboard *pasteBoard = [NSPasteboard generalPasteboard];
		NSString* clipboardString = [pasteBoard  stringForType:NSPasteboardTypeString];
		retString = std::string([clipboardString UTF8String]);
			
		[pool release];

		return retString;
	}


}

