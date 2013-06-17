/*   _____                           
* /\  _  \                     __    
* \ \ \_\ \      __    __  __ /\_\   
*  \ \  __ \   /'_ `\ /\ \/\ \\/\ \  
*   \ \ \/\ \ /\ \_\ \\ \ \_\ \\ \ \ 
*    \ \_\ \_\\ \____ \\ \____/ \ \_\
*     \/_/\/_/ \/____\ \\/___/   \/_/
*                /\____/             
*                \_/__/              
*
* Copyright (c) 2011 Joshua Larouche
* 
*
* License: (BSD)
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name of Agui nor the names of its contributors may
*    be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "GuiFontManager.hpp"
#include "Agui/Backends/Allegro5/Allegro5.hpp"
namespace cge
{
	GuiFontManager::~GuiFontManager(void)
	{
		for(size_t i = 0; i < m_fonts.size(); ++i)
		{
			delete m_fonts[i];
		}
	}

	agui::Font* GuiFontManager::getDefaultFont() const
	{
		return getFont((int)m_defaultSize);
	}

	agui::Font* GuiFontManager::getFont( int size ) const
	{
		if(m_fonts.empty())
		{
			return NULL;
		}

		int pos = size - m_beginRange;

		if(pos < 0)
		{
			return m_fonts[0];
		}
		else if(pos >= (int)m_fonts.size())
		{
			return m_fonts.back();
		}
		
		return m_fonts[pos];

	}

	agui::Font* GuiFontManager::getFont( float scale ) const
	{
		int size = (int)((float)m_defaultSize * scale);
		return getFont(size);
	}

	agui::Font* GuiFontManager::getFont( agui::Font* ref, float scale ) const
	{
		int fontSz = -1;
		for(size_t i = 0; i < m_fonts.size(); ++i)
		{
			if(m_fonts[i] == ref)
			{
				fontSz = i;
				break;
			}
		}

		if(fontSz == -1)
		{
			return NULL;
		}

		fontSz += m_beginRange;

		int size = (int)((float)fontSz * scale);

		return getFont(size);
	}

	GuiFontManager::GuiFontManager( 
		const std::string& fontPath, int beginRange, int endRange, int defaultSize )
		: m_beginRange(beginRange), m_endRange(endRange), m_defaultSize(defaultSize)
	{
		for(int i = beginRange; i <= endRange; ++i)
		{
			m_fonts.push_back(agui::Font::load(fontPath,i));
		}
	}

}
