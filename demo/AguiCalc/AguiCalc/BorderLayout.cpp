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

#include "BorderLayout.hpp"


namespace cge
{
	BorderLayout::BorderLayout( agui::Image* horzImg, agui::Image* vertImg, agui::Image* vertFlipImg )
		:m_horzImg(horzImg),m_vertImg(vertImg), m_paint(false), m_vertFlipImg(vertFlipImg)
	{

	}

	BorderLayout::~BorderLayout(void)
	{
	}

	void BorderLayout::paintBackground( const agui::PaintEvent &paintEvent )
	{

	}

	//sample code to draw borders on the border layout
	void BorderLayout::paintComponent( const agui::PaintEvent &paintEvent )
	{
		if(!m_paint)
		{
			return;
		}
		agui::Widget* north = getWidget(agui::BorderLayout::NORTH);
		agui::Widget* south = getWidget(agui::BorderLayout::SOUTH);
		agui::Widget* east = getWidget(agui::BorderLayout::EAST);
		agui::Widget* west = getWidget(agui::BorderLayout::WEST);
		agui::Widget* center = getWidget(agui::BorderLayout::CENTER);

		if(north && north->isVisible())
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_horzImg,agui::Point(
				north->getLocation().getX(), 
				north->getLocation().getY() + north->getHeight() -7),
				agui::Dimension(north->getWidth() - 3,m_horzImg->getHeight() + 3));
		}

		if(south && south->isVisible())
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_horzImg,agui::Point(
				south->getLocation().getX(), 
				south->getLocation().getY() - m_horzImg->getHeight() + 2),
				agui::Dimension(south->getWidth() - 3,m_horzImg->getHeight() + 1));
		}

		if(east && east->isVisible())
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_vertImg,agui::Point(
				east->getLocation().getX() + 1 - m_vertImg->getWidth(), 
				east->getLocation().getY()),
				agui::Dimension(m_vertImg->getWidth(),east->getHeight()));
		}

		if((west && west->isVisible()) && !(center &&  center->isVisible()))
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_vertFlipImg,agui::Point(
				west->getLocation().getX() +
				west->getInnerWidth() + west->getMargin(agui::SIDE_LEFT), 
				west->getLocation().getY()),
				agui::Dimension(m_vertFlipImg->getWidth(),west->getHeight()));
		}
		else if((west && west->isVisible()) && (center &&  center->isVisible()))
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_vertFlipImg,agui::Point(
				west->getLocation().getX() + 1 +
				center->getLocation().getX() - m_vertFlipImg->getWidth(), 
				west->getLocation().getY()),
				agui::Dimension(m_vertFlipImg->getWidth(),west->getHeight()));
		}
	}

	void BorderLayout::setPaintingBorders( bool painting )
	{
		m_paint = painting;
	}

}

