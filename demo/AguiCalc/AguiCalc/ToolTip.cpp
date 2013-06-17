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

#include "ToolTip.hpp"

namespace cge
{
	ToolTip::~ToolTip(void)
	{
	}

	ToolTip::ToolTip( agui::Image* tooltipBG )
		:m_tooltipBG(tooltipBG), m_opacityRate(0.1f)
	{
	}

	void ToolTip::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(
			m_tooltipBG,agui::Point(0,0),getSize(),getOpacity());
	}

	void ToolTip::showToolTip( const std::string& text, int width, int x, int y, Widget* invoker )
	{
		agui::ToolTip::showToolTip(text,width,x,y, invoker);
		setOpacity(0.0f);
	}

	void ToolTip::logic( double timeElapsed )
	{
		if(getOpacity() < 1.0f)
		{
			setOpacity(getOpacity() + m_opacityRate);
		}
	}

	void ToolTip::paintComponent( const agui::PaintEvent &paintEvent )
	{
		agui::Color c = agui::Color(
			getFontColor().getR(),getFontColor().getG(),
			getFontColor().getB(),getOpacity());
		resizableText.drawTextArea(
			paintEvent.graphics(),getFont(),getInnerRectangle(),
			c,getAreaText(),getTextAlignment());
	}

}
