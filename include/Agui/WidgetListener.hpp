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

#ifndef AGUI_WIDGET_LISTENER_HPP
#define AGUI_WIDGET_LISTENER_HPP

#include "Agui/BaseTypes.hpp"
namespace agui {
	/**
     * Abstract class for Widget Listeners.
	 *
	 * Any derived Widget Listeners should inherit from this class.
     * @author Joshua Larouche
     * @since 0.1.0
     */
	class AGUI_CORE_DECLSPEC WidgetListener
	{
	protected:
		virtual ~WidgetListener(void) {};
	public:
		WidgetListener(void);

		virtual void childAdded(Widget* source,
			Widget* widget) {}

		virtual void childRemoved(Widget* source,
			Widget* widget) {}

		virtual void fontChanged(Widget* source, 
			const Font *font) {}

		virtual void textChanged(Widget* source,
			const std::string &text) {}

		virtual void enabledChanged(Widget* source, bool enabled) {}

		virtual void visibilityChanged(Widget* source, bool visible) {}

		virtual void locationChanged(Widget* source, 
			const Point &location) {}

		virtual void death(Widget* source) {}

		virtual void sizeChanged(Widget* source, 
			const Dimension &size) {}

		virtual void minSizeChanged(Widget* source, 
			const Dimension &minSize) {}

		virtual void maxSizeChanged(Widget* source,
			const Dimension &maxSize) {}

		virtual void parentSizeChanged(Widget* source,
			const Dimension &parentInnerSize) {}

	};
}

#endif

