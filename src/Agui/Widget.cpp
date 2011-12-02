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

#include "Agui/Widget.hpp"
#include <queue>

namespace agui {
	Font* Widget::globalFont = NULL;

	Widget::Widget(void)
	: font(getGlobalFont()),_container(NULL), _focusManager(NULL), parentWidget(NULL),
	   usingGlobalFont(true),previousFontNum(678),
	   isWidgetVisible(true),isWidgetEnabled(true),
	   isWidgetFocusable(false),isWidgetTabable(false), paintingChildren(false),
	   tMargin(1),lMargin(1),bMargin(1),rMargin(1), textLen(0),
	   flaggedForDestruction(false)
	{
		setLocation(Point(0,0));
		setMargins(1,1,1,1);
		setFont(globalFont);
		setOpacity(1.0f);
		isWidgetFocusable = false;
		setBackColor(Color(255,255,255,255));
		setFontColor(Color(0,0,0,255));
	}


	Widget::~Widget(void)
	{
		for(std::vector<WidgetListener*>::iterator it = 
			widgetListeners.begin();
			it != widgetListeners.end(); ++it)
		{
			if((*it))
				(*it)->death(this);
		}

		if(getParent())
		{
			if(getParent()->containsPrivateChild(this))
			{
				getParent()->removePrivateChild(this);
			}
			else
			{
				getParent()->remove(this);
			}
			
		}

		for(std::vector<Widget*>::iterator it = getPrivateChildBegin();
			it != getPrivateChildEnd(); ++it)
		{
			(*it)->parentWidget = NULL;
			(*it)->_container = NULL;
		}

		for(std::vector<Widget*>::iterator it = getChildBegin();
			it != getChildEnd(); ++it)
		{
			(*it)->parentWidget = NULL;
			(*it)->_container = NULL;
		}

	}



	void Widget::paint(const PaintEvent &paintEvent)
	{
		if(isUsingGlobalFont() && globalFontID != previousFontNum)
		{
			previousFontNum = globalFontID;
			setFont(globalFont);
		}
		paintEvent.graphics()->setOffset(getAbsolutePosition());
		paintBackground(paintEvent);

		paintEvent.graphics()->pushClippingRect(Rectangle(getMargin(SIDE_LEFT),
			getMargin(SIDE_TOP),
			getInnerSize().getWidth(),getInnerSize().getHeight()));

		paintEvent.graphics()->setOffset(Point(getAbsolutePosition().getX() + getMargin(SIDE_LEFT),
			getAbsolutePosition().getY() + getMargin(SIDE_TOP)));

		paintComponent(paintEvent);
		
	}



	void Widget::setText(const std::string &text )
	{
		for(std::vector<WidgetListener*>::iterator it = 
			widgetListeners.begin();
			it != widgetListeners.end(); ++it)
		{
			if((*it))
				(*it)->textChanged(this,text);
		}
			this->text = text;
			textLen = unicodeFunctions.length(getText());

	}


	const std::string& Widget::getText() const
	{
		return text;
	}


	void Widget::add( Widget *widget )
	{

		if(widget == NULL)
		{
			throw Exception("Cannot add child widget because it is NULL");
			return;
		}

		if(widget->parentWidget == NULL && !containsChildWidget(widget))
		{
			for(std::vector<WidgetListener*>::iterator it =
				widgetListeners.begin();
				it != widgetListeners.end(); ++it)
			{
				if((*it))
					(*it)->childAdded(this,widget);
			}

			children.push_back(widget);

			widget->parentWidget = this;
			widget->_container = this->_container;
			widget->parentSizeChanged();
			if(getGui())
			{
				getGui()->_widgetLocationChanged();
			}
		}



	}

	Widget* Widget::getParent() const
	{
		return parentWidget;
	}

	void Widget::remove( Widget *widget )
	{


		if(widget == NULL)
		{
			throw Exception("Could not remove child because it is NULL");
			return;
		}


		if(containsChildWidget(widget))
		{
			if(getGui())
			{
				getGui()->_dispatchWidgetDestroyed(widget);
			}


			for(std::vector<WidgetListener*>::iterator it =
				widgetListeners.begin();
				it != widgetListeners.end(); ++it)
			{
				if((*it))
					(*it)->childRemoved(this,widget);
			}

		
			children.erase(children.begin() + getChildWidgetIndex(widget));
			widget->parentWidget = NULL;
			widget->_container = NULL;

		}

		removePrivateChild(widget);
		if(getGui())
		{
			getGui()->_widgetLocationChanged();
		}

	}




	bool Widget::containsChildWidget( Widget *widget ) const
	{
		for(unsigned int i = 0; i < children.size(); ++i)
		{
			if(children[i] == widget)
			{
				return true;
			}
		}
		return false;
	}


	int Widget::getChildWidgetIndex( Widget *widget ) const
	{
		//returns index or -1 if not found
		for(unsigned int i = 0; i < children.size(); ++i)
		{
			if(children[i] == widget)
			{
				return i;
			}
		}
		return -1;
	}

	int Widget::getIndexInParent() const
	{
		if(!getParent())
			return -1;

		return getParent()->getChildWidgetIndex((Widget*)this);
	}


	void Widget::mouseEnter(MouseEvent &mouseEvent)
	{
	}



	void Widget::mouseLeave(MouseEvent &mouseEvent)
	{
	}


	void Widget::focusGained()
	{
		for(std::vector<FocusListener*>::iterator it = focusListeners.begin();
			it != focusListeners.end(); ++it)
		{
			(*it)->focusGainedCB(this);
		}

	}

	void Widget::focusLost()
	{
		for(std::vector<FocusListener*>::iterator it = focusListeners.begin();
			it != focusListeners.end(); ++it)
		{
			(*it)->focusLostCB(this);
		}

	}

	 Widget* Widget::getTopWidget() const
	{
		if(!getParent())
		{
			return NULL;
		}
		Widget* parent = (Widget*)this;

		while(parent->getParent() != NULL)
		{
			parent = parent->getParent();
		}

		return parent;
	}

	bool Widget::intersectionWithPoint(const Point &p ) const
	{
		return getSizeRectangle().pointInside(p);
	}

	const Rectangle Widget::getAbsoluteRectangle() const
	{
		Point absLocation;

		absLocation = getLocation();

		if(!getParent())
		{
			return Rectangle(absLocation.getX(),absLocation.getY(),
				getSize().getWidth(),getSize().getHeight());
		}
		
		Widget* parent = (Widget*)this;
		int eX = 0;
		int eY = 0;

		while(parent->getParent() != NULL)
		{
			parent = parent->getParent();

			eX = parent->getMargin(SIDE_LEFT);
			eY = parent->getMargin(SIDE_TOP);
		
			absLocation.setX(absLocation.getX() + parent->getLocation().getX() + eX);
			absLocation.setY(absLocation.getY() + parent->getLocation().getY() + eY);
		}

		return Rectangle(absLocation,getSize());
	}

	const Dimension& Widget::getSize() const
	{
		return size;
	}

	void Widget::setSize(const Dimension &size )
	{
		int x = size.getWidth();
		int y = size.getHeight();

		if(x > maxSize.getWidth() && maxSize.getWidth() > 0)
		{
			x = maxSize.getWidth();
		}
		else if(x < minSize.getWidth())
		{
			x = minSize.getWidth();
		}

		if(y > maxSize.getHeight() && maxSize.getHeight() > 0)
		{
			y = maxSize.getHeight();
		}
		else if(y < minSize.getHeight())
		{
			y = minSize.getHeight();
		}

		this->size = Dimension(x,y);
		_setInnerSize();

		if(getGui())
		{
			getGui()->_widgetLocationChanged();
		}

		for(std::vector<Widget*>::iterator it = getPrivateChildBegin();
			it != getPrivateChildEnd(); ++it)
		{
			(*it)->parentSizeChanged();
		}

		for(std::vector<Widget*>::iterator it = getChildBegin();
			it != getChildEnd(); ++it)
		{
			(*it)->parentSizeChanged();
		}
		for(std::vector<WidgetListener*>::iterator it =
			widgetListeners.begin();
			it != widgetListeners.end(); ++it)
		{
			if((*it))
				(*it)->sizeChanged(this,getSize());
		}

	}

	void Widget::setSize( int width, int height )
	{
		setSize(Dimension(width,height));
	}

	const Point& Widget::getLocation() const
	{
		return location;
	}

	void Widget::setLocation(const Point &location )
	{
		this->location = location;

		for(std::vector<WidgetListener*>::iterator it = 
			widgetListeners.begin();
			it != widgetListeners.end(); ++it)
		{
			if((*it))
				(*it)->locationChanged(this,location);
		}

		if(getGui())
		{
			getGui()->_widgetLocationChanged();
		}
	}

	void Widget::setLocation( int x, int y )
	{
		setLocation(Point(x,y));
	}



	void Widget::keyDown( KeyEvent &keyEvent )
	{
	}

	void Widget::keyUp( KeyEvent &keyEvent )
	{
	}


	void Widget::keyRepeat( KeyEvent &keyEvent )
	{
	}

	void Widget::mouseDown( MouseEvent &mouseEvent )
	{
	}

	void Widget::mouseMove( MouseEvent &mouseEvent )
	{
	}

	void Widget::mouseUp( MouseEvent &mouseEvent )
	{
	}

	void Widget::mouseWheelUp( MouseEvent &mouseEvent )
	{
	}

	void Widget::mouseWheelDown( MouseEvent &mouseEvent )
	{
	}

	void Widget::mouseClick( MouseEvent &mouseEvent )
	{
	}



	void Widget::setVisibility( bool visible )
	{

		if(visible != this->isWidgetVisible)
		{
			this->isWidgetVisible = visible;

			for(std::vector<WidgetListener*>::iterator it = 
				widgetListeners.begin();
				it != widgetListeners.end(); ++it)
			{
				if((*it))
					(*it)->visibilityChanged(this,visible);
			}

		}

		if(isUsingGlobalFont() && globalFontID != previousFontNum)
		{
			previousFontNum = globalFontID;
			setFont(globalFont);
		}

		if(getGui())
		{
			getGui()->_widgetLocationChanged();
		}

		if(isUsingGlobalFont() && globalFontID != previousFontNum)
		{
			previousFontNum = globalFontID;
			setFont(globalFont);
		}
		
	}

	bool Widget::isVisible() const
	{
		return isWidgetVisible;
	}

	void Widget::setEnabled( bool enabled )
	{


		if(this->isWidgetEnabled != enabled)
		{
			this->isWidgetEnabled = enabled;

			for(std::vector<WidgetListener*>::iterator it = 
				widgetListeners.begin();
				it != widgetListeners.end(); ++it)
			{
				if((*it))
					(*it)->enabledChanged(this,enabled);
			}
		}

		if(isUsingGlobalFont() && globalFontID != previousFontNum)
		{
			previousFontNum = globalFontID;
			setFont(globalFont);
		}
		
	}

	bool Widget::isEnabled() const
	{
		return isWidgetEnabled;
	}

	std::vector<Widget*>::iterator Widget::getChildBegin() 
	{
		return children.begin();
	}

	std::vector<Widget*>::const_iterator
	Widget::getChildBegin() const
	{
		return children.begin();
	}

	std::vector<Widget*>::iterator Widget::getChildEnd() 
	{
		return children.end();
	}

	std::vector<Widget*>::const_iterator 
	Widget::getChildEnd() const
	{
		return children.end();
	}

	std::vector<Widget*>::reverse_iterator 
	Widget::getChildRBegin() 
	{
		return children.rbegin();
	}

	std::vector<Widget*>::const_reverse_iterator 
	Widget::getChildRBegin() const
	{
		return children.rbegin();
	}

	std::vector<Widget*>::reverse_iterator Widget::getChildREnd() 
	{
		return children.rend();
	}

	std::vector<Widget*>::const_reverse_iterator 
	Widget::getChildREnd() const
	{
		return children.rend();
	}

	Widget* Widget::getChildAt( int index ) const
	{
		if(index < 0 || index > (int)children.size() -1) return (Widget*)0;

		return children[index];
	}

	int Widget::getChildCount() const
	{
		return children.size();
	}

	Gui* Widget::getGui() const
	{

		if(_container)
		return _container;

		if(getTopWidget())
		{
		   return getTopWidget()->getGui();
		}


		return (Gui*)NULL;
			
		
	}



	void Widget::setMinSize( const Dimension &size )
	{

		int x = size.getWidth();
		int y = size.getHeight();

		if(x < 0)
		{
			x = 0;
		}
		if(y < 0)
		{
			y = 0;
		}
		if(x > maxSize.getWidth() && maxSize.getWidth() > 0)
		{
			x = maxSize.getWidth();
		}
		if(y > maxSize.getHeight() && maxSize.getHeight() > 0)
		{
			y = maxSize.getHeight();
		}

		minSize = Dimension(x,y);
		setSize(getSize());

		for(std::vector<WidgetListener*>::iterator it = 
			widgetListeners.begin();
			it != widgetListeners.end(); ++it)
		{
			if((*it))
				(*it)->minSizeChanged(this,size);
		}

	}

	void Widget::setMaxSize( const Dimension &size )
	{

		int x = size.getWidth();
		int y = size.getHeight();


		if(x < 0)
		{
			x = 0;
		}
		if(y < 0)
		{
			y = 0;
		}

		if(x < minSize.getWidth() && x != 0)
		{
			x = minSize.getWidth();
		}
		if(y < minSize.getHeight() && y != 0)
		{
			y = minSize.getHeight();
		}

		maxSize = Dimension(x,y);

		setSize(getSize());

		for(std::vector<WidgetListener*>::iterator it = 
			widgetListeners.begin();
			it != widgetListeners.end(); ++it)
		{
			if((*it))
				(*it)->maxSizeChanged(this,size);
		}

	}




	void Widget::mouseDrag( MouseEvent &mouseEvent )
	{
	}


	bool Widget::isFocusable() const
	{
		return isWidgetFocusable;
	}

	void Widget::setFocusable( bool focusable )
	{
		this->isWidgetFocusable = focusable;
	}

	void Widget::focus()
	{
		
		Widget* top = getTopWidget();
		if(top)
		{
			if(top->_focusManager)
			{
				top->_focusManager->setFocusedWidget(this);
			}
		}
		
	}

	void Widget::mouseHover( MouseEvent &mouseEvent )
	{
	}


	void Widget::mouseDoubleClick( MouseEvent &mouseEvent )
	{
	}

	Font* Widget::getGlobalFont()
	{
		return globalFont;
	}

	void Widget::setFont( const Font *font )
	{
		if(font == globalFont)
		{
			usingGlobalFont = true;
		}
		else
		{
			this->font = (Font*)font;
			usingGlobalFont = false;
		}

		for(std::vector<WidgetListener*>::iterator it = 
			widgetListeners.begin();
			it != widgetListeners.end(); ++it)
		{
			if((*it))
				(*it)->fontChanged(this,font);
		}

	}

	void Widget::setGlobalFont( const Font *font )
	{
		globalFont = (Font*)font;	
		globalFontID = rand() + rand();
	}


	void Widget::modalMouseDown( MouseEvent &mouseEvent )
	{
	}

	void Widget::modalFocusGained()
	{
		for(std::vector<FocusListener*>::iterator it = focusListeners.begin();
			it != focusListeners.end(); ++it)
		{
			(*it)->modalFocusGainedCB(this);
		}

	}

	void Widget::modalFocusLost()
	{
		for(std::vector<FocusListener*>::iterator it = focusListeners.begin();
			it != focusListeners.end(); ++it)
		{
			(*it)->modalFocusLostCB(this);
		}

	}

	bool Widget::requestModalFocus()
	{
		Widget* top = getTopWidget();
		if(top)
		{
			if(top->_focusManager)
			{
				return top->_focusManager->requestModalFocus(this);
			}
		}

		return false;
	}

	bool Widget::releaseModalFocus()
	{
		Widget* top = getTopWidget();
		if(top)
		{
			if(top->_focusManager)
			{
				return top->_focusManager->releaseModalFocus(this);
			}
		}

		return false;
	}



	MouseEvent Widget::addSourceToMouseEvent(
		const MouseEvent &mouseEvent )
	{
	 return MouseEvent(mouseEvent.getPosition(),
		 mouseEvent.getMouseWheelChange(),
			mouseEvent.getButton(),mouseEvent.getTimeStamp(),
			mouseEvent.getPressure(),mouseEvent.alt(),
			mouseEvent.control(),mouseEvent.shift(),
			this,mouseEvent.isConsumed());
	}


	KeyEvent Widget::addSourceToKeyEvent( 
		const KeyEvent &keyEvent )
	{
		return KeyEvent(keyEvent.getKey(),keyEvent.getExtendedKey(),
			keyEvent.getBackendKeycode(),keyEvent.getBackendModifierKeyFlags(),
			keyEvent.getUnichar(),keyEvent.getTimeStamp(),
			keyEvent.alt(),keyEvent.control(),keyEvent.shift(),
			this,keyEvent.isConsumed());
	}




	void Widget::focusNext()
	{
		if (children.empty())
			return;

		std::vector<Widget*>::iterator 
			startWidget = children.begin();

		for(std::vector<Widget*>::iterator it = children.begin();
			it != children.end(); ++it)
		{
			if((*it)->isFocused())
			{
				if(children.size() == 1)
				{
					return;
				}
				startWidget = it;
				break;
			}
		}

		std::vector<Widget*>::iterator
			currentWidget = startWidget;

		do 
		{

			if(currentWidget == children.end())
				currentWidget = children.begin();

			if((*currentWidget)->isFocusable() 
				&& (*currentWidget) != getFocusedWidget())
			{
				(*currentWidget)->focus();
				return;
			}

			if(currentWidget != children.end())
				currentWidget++;
			

		} while (currentWidget != startWidget);

		

	}

	void Widget::focusPrevious()
	{
		if (children.empty())
			return;

		std::vector<Widget*>::reverse_iterator
			startWidget = children.rbegin();

		for(std::vector<Widget*>::reverse_iterator it = children.rbegin();
			it != children.rend(); ++it)
		{
			if((*it)->isFocused())
			{
				if(children.size() == 1)
				{
					return;
				}
				startWidget = it;
				break;
			}
		}

		std::vector<Widget*>::reverse_iterator
			currentWidget = startWidget;

		do 
		{

			if(currentWidget == children.rend())
				currentWidget = children.rbegin();

			if((*currentWidget)->isFocusable() 
				&& (*currentWidget) != getFocusedWidget())
			{
				(*currentWidget)->focus();
				return;
			}

			if(currentWidget != children.rend())
				currentWidget++;


		} while (currentWidget != startWidget);


	}

	Widget* Widget::getFocusedWidget() const
	{
		Widget* top = getTopWidget();
		if(top)
		{
			if(top->_focusManager)
			{
				return top->_focusManager->getFocusedWidget();
			}
		}

		return (Widget*) NULL;
	}

	bool Widget::isFocused() const
	{
		Widget* top = getTopWidget();
		if(top)
		{
			if(top->_focusManager)
			{
				return top->_focusManager->getFocusedWidget() == this;
			}
		}

		return false;
	}

	void Widget::setFontColor( const Color &color )
	{
		fontColor = color;
	}

	const Color& Widget::getFontColor() const
	{
		return fontColor;
	}

	void Widget::setBackColor( const Color &color )
	{
		backColor = color;
	}

	const Color& Widget::getBackColor() const
	{
		return backColor;
	}

	void Widget::setOpacity( float opacity )
	{
		this->opacity = opacity;
	}

	float Widget::getOpacity() const
	{
		return opacity;
	}


	bool Widget::_dispatchKeyboardListenerEvent(
										KeyEvent::KeyboardEventEnum event,
										const KeyEvent & keyEvent )
	{
		if(keyboardListeners.empty())
			return false;

		if(keyEvent.isConsumed())
			return true;

		KeyEvent kArgs = addSourceToKeyEvent(keyEvent);

		for(std::vector<KeyboardListener*>::iterator it 
			= keyboardListeners.begin(); it != keyboardListeners.end(); ++it)
		{
			switch (event)
			{
			case KeyEvent::KEY_DOWN:
				(*it)->keyDownCB(kArgs);
				break;
			case KeyEvent::KEY_UP:
				(*it)->keyUpCB(kArgs);
				break;
			case KeyEvent::KEY_REPEAT:
				(*it)->keyRepeatCB(kArgs);
				break;
			default:
				break;
			}
		}

		return kArgs.isConsumed();
	}

	bool Widget::_dispatchMouseListenerEvent( 
										MouseEvent::MouseEventEnum event,
										MouseEvent & mouseEvent )
	{
		if(mouseListeners.empty())
			return false;


		MouseEvent mArgs = addSourceToMouseEvent(mouseEvent);

		for(std::vector<MouseListener*>::iterator it 
			= mouseListeners.begin(); it != mouseListeners.end(); ++it)
		{

			switch (event)
			{
			case MouseEvent::MOUSE_DOWN:
				(*it)->mouseDownCB(mArgs);
				break;
			case MouseEvent::MOUSE_UP:
				(*it)->mouseUpCB(mArgs);
				break;
			case MouseEvent::MOUSE_MOVE:
				(*it)->mouseMoveCB(mArgs);
				break;
			case MouseEvent::MOUSE_CLICK:
				(*it)->mouseClickCB(mArgs);
				break;
			case MouseEvent::MOUSE_DOUBLE_CLICK:
				(*it)->mouseDoubleClickCB(mArgs);
				break;
			case MouseEvent::MOUSE_WHEEL_UP:
				(*it)->mouseWheelUpCB(mArgs);
				break;
			case MouseEvent::MOUSE_WHEEL_DOWN:
				(*it)->mouseWheelDownCB(mArgs);
				break;
			case MouseEvent::MOUSE_ENTER:
				(*it)->mouseEnterCB(mArgs);
				break;
			case MouseEvent::MOUSE_LEAVE:

				(*it)->mouseLeaveCB(mArgs);
				break;
			case MouseEvent::MOUSE_HOVER:
				(*it)->mouseHoverCB(mArgs);
				break;
			case MouseEvent::MOUSE_DRAG:
				(*it)->mouseDragCB(mArgs);
				break;
			case MouseEvent::MOUSE_MODAL_DOWN:
				(*it)->modalMouseDownCB(mArgs);
				break;
			case MouseEvent::MOUSE_MODAL_UP:
				(*it)->modalMouseUpCB(mArgs);
				break;
			default:
				break;
			}
		}
		if(mArgs.isConsumed())
		{
			mouseEvent.consume();
		}
		return mArgs.isConsumed();
	}

	void Widget::addMouseListener( MouseListener* listener )
	{
		if(!listener)
		{
			return;
		}
		for (std::vector<MouseListener*>::iterator it = mouseListeners.begin();
			it != mouseListeners.end(); ++it)
		{
			if((*it) == listener)
				return;
		}

		if(listener)
			mouseListeners.push_back(listener);
	}

	void Widget::removeMouseListener( MouseListener* listener )
	{
		mouseListeners.erase(std::remove(mouseListeners.begin(),
			mouseListeners.end(), listener), mouseListeners.end());

	}

	void Widget::addKeyboardListener( KeyboardListener* listener )
	{
		if(!listener)
		{
			return;
		}
		for (std::vector<KeyboardListener*>::iterator it = 
			keyboardListeners.begin();
			it != keyboardListeners.end(); ++it)
		{
			if((*it) == listener)
				return;
		}

		if(listener)
			keyboardListeners.push_back(listener);
	}

	void Widget::removeKeyboardListener( KeyboardListener* listener )
	{
		keyboardListeners.erase(std::remove(keyboardListeners.begin(),
			keyboardListeners.end(), listener), keyboardListeners.end());
	}

	void Widget::addWidgetListener( 
										WidgetListener *listener )
	{
		if(!listener)
		{
			return;
		}
		for(std::vector<WidgetListener*>::iterator it = 
			widgetListeners.begin();
			it != widgetListeners.end(); ++it)
		{
			if((*it) == listener)
				return;
		}

		widgetListeners.push_back(listener);
	}

	void Widget::removeWidgetListener( 
										WidgetListener *listener )
	{
		widgetListeners.erase(
			std::remove(widgetListeners.begin(),
			widgetListeners.end(), listener),
			widgetListeners.end());
	}


	void Widget::addFocusListener( FocusListener* listener )
	{
		if(!listener)
		{
			return;
		}
		for(std::vector<FocusListener*>::iterator it = focusListeners.begin();
			it != focusListeners.end(); ++it)
		{
			if((*it) == listener)
				return;
		}

		focusListeners.push_back(listener);
	}

	void Widget::removeFocusListener( FocusListener* listener )
	{
		focusListeners.erase(std::remove(focusListeners.begin(),
			focusListeners.end(), listener), focusListeners.end());
	}

	void Widget::setBackWidget( Widget* widget )
	{
		if(containsChildWidget(widget))
		{
			if(children.front() == widget)
				return;

			children.erase(std::remove(children.begin(), 
				children.end(), widget), children.end());


			children.insert(children.begin(),widget);

		}
		else if(containsPrivateChild(widget))
		{
			if(privateChildren.front() == widget)
				return;

			privateChildren.erase(std::remove(privateChildren.begin(), 
				privateChildren.end(), widget), privateChildren.end());


			privateChildren.insert(privateChildren.begin(),widget);
		}
	}

	void Widget::setFrontWidget( Widget* widget )
	{

		if(containsChildWidget(widget))
		{
			if(children.back() == widget)
				return;

			children.erase(std::remove(children.begin(), 
				children.end(), widget), children.end());

			children.push_back(widget);

		}
		else if(containsPrivateChild(widget))
		{
			if(privateChildren.back() == widget)
				return;

			privateChildren.erase(std::remove(privateChildren.begin(), 
				privateChildren.end(), widget), privateChildren.end());

			privateChildren.push_back(widget);
		}
	}

	void Widget::bringToFront()
	{
		if(getParent())
		{
			getParent()->setFrontWidget(this);
		}
	}

	void Widget::sendToBack()
	{
		if(getParent())
		{
			getParent()->setBackWidget(this);
		}
	}

	void Widget::show()
	{
		setVisibility(true);
	}

	void Widget::hide()
	{
		setVisibility(false);
	}

	Point Widget::getAbsolutePosition() const
	{
		return getAbsoluteRectangle().getTopLeft();
	}

	

	bool Widget::isTabable() const
	{
		return isWidgetTabable;
	}

	void Widget::setTabable( bool tabable )
	{
		isWidgetTabable = tabable;
	}

	const Font* Widget::getFont() const
	{
		if(usingGlobalFont)
		{
			return globalFont;
		}
		else
		{
			return font;
		}
	}

	bool Widget::isUsingGlobalFont() const
	{
		return usingGlobalFont;
	}


	Point Widget::createAlignedPosition( AreaAlignmentEnum alignment,
												const Rectangle &parentRect,
												const Dimension &childSize ) const
	{

		switch (alignment)
		{
		case ALIGN_TOP_LEFT:
			return parentRect.getTopLeft();
			break;
		case ALIGN_TOP_CENTER:
			return Point(parentRect.getTopLeft().getX()
				+ ((parentRect.getWidth() / 2) - childSize.getWidth() / 2)
				, parentRect.getTopLeft().getY());
			break;
		case ALIGN_TOP_RIGHT:
			return Point(parentRect.getTopLeft().getX()
				+ (parentRect.getWidth() - childSize.getWidth())
				, parentRect.getTopLeft().getY());
			break;
		case ALIGN_MIDDLE_LEFT:
			return Point(parentRect.getTopLeft().getX()
				, parentRect.getTopLeft().getY() + 
				((parentRect.getHeight() / 2) - childSize.getHeight() / 2));
			break;
		case ALIGN_MIDDLE_CENTER:
			return Point(parentRect.getTopLeft().getX()
				+ ((parentRect.getWidth() / 2) - childSize.getWidth() / 2)
				, parentRect.getTopLeft().getY() + 
				((parentRect.getHeight() / 2) - childSize.getHeight() / 2));
			break;
		case ALIGN_MIDDLE_RIGHT:
			return Point(parentRect.getTopLeft().getX()
				+ (parentRect.getWidth() - childSize.getWidth())
				, parentRect.getTopLeft().getY() + 
				((parentRect.getHeight() / 2) - childSize.getHeight() / 2));
			break;

		case ALIGN_BOTTOM_LEFT:
			return Point(parentRect.getTopLeft().getX()
				, parentRect.getTopLeft().getY() + 
				(parentRect.getHeight()  - childSize.getHeight()));
			break;
		case ALIGN_BOTTOM_CENTER:
			return Point(parentRect.getTopLeft().getX()
				+ ((parentRect.getWidth() / 2) - childSize.getWidth() / 2)
				, parentRect.getTopLeft().getY() + 
				(parentRect.getHeight() - childSize.getHeight()));
			break;
		case ALIGN_BOTTOM_RIGHT:
			return Point(parentRect.getTopLeft().getX()
				+ (parentRect.getWidth() - childSize.getWidth())
				, parentRect.getTopLeft().getY() + 
				(parentRect.getHeight() - childSize.getHeight()));
			break;
		default:
			return Point();
			break;
		}
	}


	std::vector<Widget*>::iterator Widget::getPrivateChildBegin()
	{
		return privateChildren.begin();
	}

	std::vector<Widget*>::const_iterator 
	Widget::getPrivateChildBegin() const
	{
		return privateChildren.begin();
	}

	std::vector<Widget*>::reverse_iterator
	Widget::getPrivateChildRBegin()
	{
		return privateChildren.rbegin();
	}

	std::vector<Widget*>::iterator Widget::getPrivateChildEnd()
	{
		return privateChildren.end();
	}

	std::vector<Widget*>::const_iterator
	Widget::getPrivateChildEnd() const
	{
		return privateChildren.end();
	}

	std::vector<Widget*>::reverse_iterator 
	Widget::getPrivateChildREnd()
	{
		return privateChildren.rend();
	}

	std::vector<Widget*>::const_reverse_iterator 
	Widget::getPrvateChildRBegin() const
	{
		return privateChildren.rbegin();
	}

	std::vector<Widget*>::const_reverse_iterator 
	Widget::getPrivateChildREnd() const
	{
		return privateChildren.rend();
	}


	void Widget::addPrivateChild( Widget *widget )
	{
		if(widget == NULL)
		{
			throw Exception("Cannot add child control because it is NULL");
			return;
		}

		if(widget->parentWidget == NULL && !containsPrivateChild(widget))
		{
			privateChildren.push_back(widget);

			widget->parentWidget = this;
			widget->_container = this->_container;
		}
	}


	bool Widget::containsPrivateChild( Widget *widget ) const
	{
		for(unsigned int i = 0; i < privateChildren.size(); ++i)
		{
			if(privateChildren[i] == widget)
			{
				return true;
			}
		}
		return false;
	}

	void Widget::removePrivateChild( Widget *widget )
	{
		if(widget == NULL)
		{
			return;
		}

		if(containsPrivateChild(widget))
		{
			privateChildren.erase(privateChildren.begin() +
				getPrivateChildIndex(widget));
			widget->parentWidget = NULL;
			widget->_container = NULL;
		}

	}


	int Widget::getPrivateChildIndex( Widget *widget ) const
	{
		//returns index or -1 if not found
		for(unsigned int i = 0; i < privateChildren.size(); ++i)
		{
			if(privateChildren[i] == widget)
			{
				return i;
			}
		}
		return -1;
	}


	void Widget::addActionListener( ActionListener *listener)
	{
		if(!listener)
		{
			return;
		}
		for(std::vector<ActionListener*>::iterator it = 
			actionListeners.begin();
			it != actionListeners.end(); ++it)
		{
			if((*it) == listener)
				return;
		}

		actionListeners.push_back(listener);
	}

	void Widget::removeActionListener( ActionListener *listener )
	{
		actionListeners.erase(
			std::remove(actionListeners.begin(),
			actionListeners.end(), listener),
			actionListeners.end());
	}

	void Widget::dispatchActionEvent( const ActionEvent &evt )
	{
		for(std::vector<ActionListener*>::iterator it = actionListeners.begin();
			it != actionListeners.end(); ++it)
		{
			(*it)->actionPerformed(evt);
		}
	}

	const Rectangle Widget::getRelativeRectangle() const
	{
		return Rectangle(getLocation(),getSize());
	}

	int Widget::getTextLength() const
	{
		return textLen;
	}

	void Widget::setPaintingChildren( bool painting )
	{
		paintingChildren = painting;
	}

	bool Widget::isPaintingChildren() const
	{
		return paintingChildren;
	}

	void Widget::clip(const PaintEvent &paintEvent )
	{
		paintEvent.graphics()->clearClippingStack();

		if(getParent())
		{
			Widget* pWidget = this;
			do
			{
			pWidget = pWidget->getParent();
					rects.push_back(pWidget);
				


			} while(pWidget->getParent());

			while (!rects.empty())
			{
				pWidget = rects.front();

				paintEvent.graphics()->setOffset(
					Point(pWidget->getAbsolutePosition().getX() +
					pWidget->getMargin(SIDE_LEFT),
					pWidget->getAbsolutePosition().getY() +
					pWidget->getMargin(SIDE_TOP)));

				paintEvent.graphics()->pushClippingRect
					(pWidget->getInnerRectangle());
				rects.erase(rects.begin());
			}


			paintEvent.graphics()->setOffset(getAbsolutePosition());

			paintEvent.graphics()->pushClippingRect
				(getSizeRectangle());

		}
	}

	void Widget::paintChildren(const PaintEvent &paintEvent )
	{
		stackOffset = paintEvent.graphics()->getOffset();
		stackRects = paintEvent.graphics()->getClippingStack();
		_recursivePaintChildren(this,isEnabled(),paintEvent.graphics());
		paintEvent.graphics()->setClippingStack(stackRects,stackOffset);
	}

	void Widget::_recursivePaintChildren( Widget *root, bool enabled,
										   Graphics *graphicsContext )
	{
		//recursively calls itself to render widgets from back to front

		if(!root->isVisible())
		{
			return;
		}

		bool widgetEnabled = root->isEnabled();

		if(enabled == false)
		{
			widgetEnabled = false;
		}

		if(root != this)
		{
			root->clip(PaintEvent(widgetEnabled,graphicsContext));

			root->paint(PaintEvent(widgetEnabled,graphicsContext));

			if(root->isPaintingChildren())
			{
				return;
			}
		}

		for(std::vector<Widget*>::iterator it = 
			root->getPrivateChildBegin();
			it != root->getPrivateChildEnd(); ++it)
		{
			_recursivePaintChildren(*it,widgetEnabled,graphicsContext);
		}
		for(std::vector<Widget*>::iterator it = 
			root->getChildBegin();
			it != root->getChildEnd(); ++it)
		{
			_recursivePaintChildren(*it,widgetEnabled,graphicsContext);
		}

	}

	const Rectangle Widget::getSizeRectangle() const
	{
		return Rectangle(0,0,getSize().getWidth(),getSize().getHeight());
	}

	const Dimension& Widget::getMinSize() const
	{
		return minSize;
	}

	const Dimension& Widget::getMaxSize() const
	{
		return maxSize;
	}

	void Widget::setMargins( size_t t, size_t l, size_t b, size_t r )
	{
		tMargin = t;
		lMargin = l;
		bMargin = b;
		rMargin = r;
		_setInnerSize();
	}

	size_t Widget::getMargin( SideEnum side ) const
	{
		switch (side)
		{
		case SIDE_TOP:
			return tMargin;
			break;
		case SIDE_LEFT:
			return lMargin;
			break;
		case SIDE_BOTTOM:
			return bMargin;
			break;
		case SIDE_RIGHT:
			return rMargin;
			break;
		default:
			return 0;
			break;
		}
	}

	void Widget::_setInnerSize()
	{
		innerSize = Dimension(getSize().getWidth() - lMargin - rMargin,
			getSize().getHeight() - tMargin - bMargin);
	}

	const Dimension& Widget::getInnerSize() const
	{
		return innerSize;
	}

	const Rectangle Widget::getInnerRectangle() const
	{
		return Rectangle(0,0,getInnerSize().getWidth(),getInnerSize().getHeight());
	}

	int Widget::getWidth() const
	{
		return getSize().getWidth();
	}

	int Widget::getHeight() const
	{
		return getSize().getHeight();
	}

	int Widget::getInnerWidth() const
	{
		return getInnerSize().getWidth();
	}

	int Widget::getInnerHeight() const
	{
		return getInnerSize().getHeight();
	}

	void Widget::clear()
	{
		std::vector<Widget*> removeWidgets = children;
		for(size_t i = 0; i < removeWidgets.size(); ++i)
		{
			remove(removeWidgets[i]);
		}
	}

	bool Widget::keepMouseLock( MouseButtonEnum button ) const
	{
		return false;
	}

	void Widget::sendToTop()
	{
		if(getTopWidget())
		{
			Widget* top = getTopWidget();
			if(getParent())
			{
				getParent()->remove(this);
			}
			top->add(this);
		}
	}

	void Widget::modalMouseUp( MouseEvent &mouseEvent )
	{
		(void)mouseEvent;
	}

	void Widget::alignToParent( AreaAlignmentEnum alignment )
	{
		if(!getParent())
		{
			return;
		}
		setLocation(createAlignedPosition(alignment,getParent()->getInnerRectangle(),
			getSize()));
	}

	void Widget::logic( double timeElapsed )
	{

	}


	void Widget::flagForDestruction()
	{
		if(isFlaggedForDestruction())
		{
			return;
		}

		flaggedForDestruction = true;
		if(getGui())
		{
			getGui()->flagWidget(this);
		}
	}

	bool Widget::isFlaggedForDestruction() const
	{
		return flaggedForDestruction;
	}

	void Widget::flagChildrenForDestruction()
	{
		for(std::vector<Widget*>::iterator it = getChildBegin();
			it != getChildEnd(); ++it)
		{
			(*it)->flagForDestruction();
		}
	}

	void Widget::flagAllChildrenForDestruction()
	{
		for(std::vector<Widget*>::iterator it = getChildBegin();
			it != getChildEnd(); ++it)
		{
			(*it)->flagForDestruction();
			(*it)->flagAllChildrenForDestruction();
		}
	}

	void Widget::parentSizeChanged()
	{

	}

	int Widget::_round( float val ) const
	{
		if( (val + 0.5) >= (int(val) + 1) )
			return (int)(val) + 1;
		else
			return (int)val;
	}


	int Widget::globalFontID = 789;

}