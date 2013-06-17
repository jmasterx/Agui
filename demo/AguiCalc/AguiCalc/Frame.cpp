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

#include "Frame.hpp"

namespace cge
{
	Frame::~Frame(void)
	{
	}

	Frame::Frame( agui::Image* bg, Widget* content )
		: agui::Frame(content), m_bg(bg)
	{

	}

	void Frame::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawFilledRectangle(
			agui::Rectangle(getMargin(agui::SIDE_LEFT) + getLeftMargin(),
			getMargin(agui::SIDE_TOP) + getTopMargin(),
			getInnerWidth() - getRightMargin() - getLeftMargin(),getInnerHeight() - getBottomMargin() - getTopMargin())
			,getBackColor());

		paintEvent.graphics()->drawNinePatchImage(m_bg,
			agui::Point(0,0),getSize(),getOpacity());
	}

	void Frame::paintComponent( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawText(agui::Point(getLeftMargin() + 1,
			(getTopMargin() - getFont()->getLineHeight()) / 2),getText().c_str(),
			getFontColor(),getFont());
	}

	void Frame::setFont( const agui::Font *font )
	{
		agui::Widget::setFont(font);
	}

	bool Frame::intersectionWithPoint( const agui::Point &p ) const
	{
		return agui::Rectangle(getMargin(agui::SIDE_LEFT),getMargin(agui::SIDE_TOP)
			,getInnerWidth(),getInnerHeight()).pointInside(p);
	}

	void Frame::setLocation( const agui::Point &location )
	{
		agui::Point l = location;
		if(l.getX() + (int)getMargin(agui::SIDE_LEFT) < 0)
			l.setX(-(int)getMargin(agui::SIDE_LEFT));
		if(l.getY() + (int)getMargin(agui::SIDE_TOP) < 0)
			l.setY(-(int)getMargin(agui::SIDE_TOP));
		if(getParent())
		{
			if(l.getX() + getInnerWidth() + getMargin(agui::SIDE_LEFT) > 
				getParent()->getInnerWidth())
				l.setX(getParent()->getInnerWidth() - getInnerWidth() - 
				getMargin(agui::SIDE_LEFT));

			if(l.getY() + getInnerHeight() + getMargin(agui::SIDE_TOP) > 
				getParent()->getInnerHeight())
				l.setY(getParent()->getInnerHeight() - getInnerHeight() - 
				getMargin(agui::SIDE_TOP));
		}
		
		agui::Widget::setLocation(l);
	}

	void Frame::setLocation( int x, int y )
	{
		agui::Widget::setLocation(x,y);
	}

	void Frame::mouseMove( agui::MouseEvent &mouseEvent )
	{
		agui::Frame::mouseMove(mouseEvent);

		agui::Point relativeMouse = agui::Point(
			mouseEvent.getX(),
			mouseEvent.getY());

		mouseEvent.consume();

		if(isResizable())
		{
			if(getBRResizeRect().pointInside(relativeMouse))
			{
				getGui()->setCursor(agui::CursorProvider::RESIZE_NW_CURSOR);
			}
			else
			{
				getGui()->setCursor(agui::CursorProvider::DEFAULT_CURSOR);
			}
		}
	}

	void Frame::mouseUp( agui::MouseEvent &mouseEvent )
	{
		agui::Frame::mouseUp(mouseEvent);
		getGui()->setCursor(agui::CursorProvider::DEFAULT_CURSOR);
	}

	void Frame::mouseDrag( agui::MouseEvent &mouseEvent )
	{
		agui::Frame::mouseDrag(mouseEvent);

		agui::Point relativeMouse = agui::Point(
			mouseEvent.getX(),
			mouseEvent.getY());

		mouseEvent.consume();

		if(isResizable())
		{
			if(getBRResizeRect().pointInside(relativeMouse))
			{
				getGui()->setCursor(agui::CursorProvider::RESIZE_NW_CURSOR);
			}
			else
			{
				getGui()->setCursor(agui::CursorProvider::DEFAULT_CURSOR);
			}
		}
	}

}

