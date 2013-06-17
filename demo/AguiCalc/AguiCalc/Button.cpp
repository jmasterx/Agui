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

#include "Button.hpp"

namespace cge
{
	Button::~Button(void)
	{
	}

	Button::Button( agui::Image* defaultImage, agui::Image* hoverImage, 
		agui::Image* clickImage, 
		agui::Image* focusImage, 
		agui::Image* disabledImage,
		agui::Image* centerImage)
		: m_defaultImage(defaultImage),
			m_hoverImage(hoverImage),
			m_clickImage(clickImage),
			m_focusImage(focusImage),
			m_centerImage(centerImage),
			m_disabledImage(disabledImage),
			m_contentResizeScaleX(1.0f),
			m_contentResizeScaleY(1.0f),
			m_textImage(NULL),m_scaleImg(false),m_hideDefault(false),
			m_imgScale(1.0f)
		{
			setTextAlignment(agui::ALIGN_MIDDLE_CENTER);
		}
	

	void Button::paintBackground( const agui::PaintEvent &paintEvent )
	{
		if(!paintEvent.isEnabled() && m_disabledImage)
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_disabledImage,agui::Point(0,0),getSize(),1.0f);
		}
		else
		{
			switch(getButtonState())
			{
			case DEFAULT:
				if(!m_hideDefault)
				{
					if(isFocused() && m_focusImage)
					{
						paintEvent.graphics()->drawNinePatchImage(
							m_focusImage,agui::Point(0,0),getSize(), 1.0f);
					}
					else
					{
						paintEvent.graphics()->drawNinePatchImage(
							m_defaultImage,agui::Point(0,0),getSize(), 1.0f);
					}
				}
			
				break;
			case HOVERED:
				paintEvent.graphics()->drawNinePatchImage(
					m_hoverImage,agui::Point(0,0),getSize(), 1.0f);
				break;
			case CLICKED:
				paintEvent.graphics()->drawNinePatchImage(
					m_clickImage,agui::Point(0,0),getSize(), 1.0f);
				break;
			}
		}
	}

	void Button::paintComponent( const agui::PaintEvent &paintEvent )
	{
		if(getTextLength() > 0 && m_textImage)
		{
			paintEvent.graphics()->drawImage(
				m_textImage,
				createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,getInnerRectangle(),
				agui::Dimension(m_textImage->getWidth(),m_textImage->getHeight())));
		}
		else if(m_centerImage)
		{
			if(!m_scaleImg)
			{
				paintEvent.graphics()->drawImage(
					m_centerImage,
					createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,getInnerRectangle(),
					agui::Dimension(m_centerImage->getWidth(),m_centerImage->getHeight())));
			}
			else
			{
				int w = getInnerWidth() > getInnerHeight() ? getInnerHeight() : getInnerWidth();
				w *= getImageScale();
				float aspect = m_centerImage->getHeight() / (float)m_centerImage->getWidth();
				int h = w * aspect;
				agui::Point p = createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,getInnerRectangle(),
					agui::Dimension(w,h));

				paintEvent.graphics()->drawScaledImage(m_centerImage,p,agui::Point(),agui::Dimension(
					m_centerImage->getWidth(),m_centerImage->getHeight()),agui::Dimension(w,h));
			}
			
		}

		resizableText.drawTextArea(paintEvent.graphics(),getFont(),
			getInnerRectangle(),getFontColor(),getAreaText(),getTextAlignment());
	}

	bool Button::intersectionWithPoint( const agui::Point &p ) const
	{
		return agui::Rectangle(getMargin(agui::SIDE_LEFT),
			getMargin(agui::SIDE_TOP),getInnerWidth(),
			getInnerHeight()).pointInside(p);
	}

	void Button::setContentResizeScale( float w, float h )
	{
		m_contentResizeScaleX = w;
		m_contentResizeScaleY = h;
	}

	void Button::resizeToContents()
	{
		float constX = 35.0f;
		float constY = 27.0f;
		int extraX = (constX * m_contentResizeScaleX) - constX;
		int extraY = (constY * m_contentResizeScaleY) - constY;
		agui::Button::resizeToContents();
		setSize(getWidth() + extraX,
			getHeight() + extraY);
	}

	void Button::setTextImage( agui::Image* img )
	{
		m_textImage = img;
	}

	void Button::setScaleIcon( bool scale )
	{
		m_scaleImg = scale;
	}

	void Button::setHideDefault( bool hide )
	{
		m_hideDefault = hide;
	}

	void Button::setImage( agui::Image* img )
	{
		m_centerImage = img;
	}

	void Button::setImageScale( float scale )
	{
		m_imgScale = scale;
	}

	float Button::getImageScale() const
	{
		return m_imgScale;
	}

}

