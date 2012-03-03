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

#include "Agui/Layout.hpp"

namespace agui
{
	Layout::Layout(void)
	:isLayingOut(false), resizeToParent(true)
	{
	}

	Layout::~Layout(void)
	{
		for(std::list<Widget*>::iterator it = getPrivateChildBegin(); it != 
			getPrivateChildEnd(); ++it)
		{
			(*it)->removeWidgetListener(this);
		}
		for(std::list<Widget*>::iterator it = getChildBegin(); it != 
			getChildEnd(); ++it)
		{
			(*it)->removeWidgetListener(this);
		}
	}

	void Layout::setSize( const Dimension &size )
	{
		Widget::setSize(size);
		updateLayout();
	}

	void Layout::setSize( int width, int height )
	{
		Widget::setSize(width,height);
	}

	void Layout::add( Widget *widget )
	{
		Widget::add(widget);
		widget->addWidgetListener(this);
		updateLayout();
	}

	void Layout::remove( Widget *widget )
	{
		Widget::remove(widget);
		widget->removeWidgetListener(this);
		updateLayout();
	}

	void Layout::paintBackground( const PaintEvent &paintEvent )
	{
	}

	void Layout::paintComponent( const PaintEvent &paintEvent )
	{

	}

	void Layout::locationChanged( Widget *source, const Point &location )
	{
		if(!isLayingOut)
		{
			isLayingOut = true;
			layoutChildren();
			isLayingOut = false;
		}
		
	}

	void Layout::sizeChanged( Widget* source, const Dimension &size )
	{
		if(!isLayingOut)
		{
			updateLayout();
		}
		
	}

	void Layout::updateLayout()
	{
		isLayingOut = true;
		layoutChildren();
		isLayingOut = false;
	}

	void Layout::parentSizeChanged()
	{
		if( resizeToParent && getParent())
		{
			Layout* layout = dynamic_cast<Layout*>(getParent());

			if(!layout)
			setSize(getParent()->getInnerSize());
		}
	}

	void Layout::setResizeToParent( bool resize )
	{
		resizeToParent = resize;
		parentSizeChanged();
	}

	bool Layout::isResizingToParent() const
	{
		return resizeToParent;
	}

	void Layout::visibilityChanged( Widget* source, bool visible )
	{
		if(!isLayingOut)
		updateLayout();
	}

}
