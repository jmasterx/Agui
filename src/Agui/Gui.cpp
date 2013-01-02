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

#include "Agui/Gui.hpp"
#include "Agui/TopContainer.hpp"
#include "Agui/Widgets/ToolTip/ToolTip.hpp"
namespace agui
{
	Gui::~Gui(void)
	{
		delete baseWidget;
	}

	//Gui CTOR
	Gui::Gui()
		: timerInterval(0.05), emptyMouse(MouseEvent::MOUSE_DOWN,
		MOUSE_BUTTON_NONE,
		0,0,0,0,0,false,false,false),
		 input(NULL),graphicsContext(NULL),
		 destroyingFlaggedWidgets(true), toolTip(NULL),
		 maxToolTipWidth(300), hasHiddenToolTip(true),
		 lastToolTipTime(0.0), toolTipShowLength(4.0),
		 cursorProvider(NULL), wantWidgetLocationChanged(true)
	{
		
		baseWidget = new TopContainer(this,&focusMan);

		//initialize variables
		widgetUnderMouse = 0;
		previousWidgetUnderMouse = 0;
		lastMouseDownControl = 0;
		controlWithLock = 0;
		hoverInterval = 2.5;
		doubleClickInterval = 0.35;
		doubleClickExpireTime = 0;
		tabbingEnabled = true;
		tabNextKey = KEY_TAB;
		tabNextExtKey = EXT_KEY_NONE;
		tabPreviousKey = KEY_TAB;
		tabPreviousExtKey = EXT_KEY_NONE;
		tabNextShift = false;
		tabNextControl = false;
		tabNextAlt = false;
		tabPreviousShift = true;
		tabPreviousControl = false;
		tabPreviousAlt = false;
		enableExistanceCheck = true;

		lastMouseButton = MOUSE_BUTTON_NONE;
	}


	void Gui::handleMouseAxes( const MouseInput &mouse, bool isLocationEvent )
	{
		//handle the mouse move and mouse wheel events

		//obtain mouse data
		if(!isLocationEvent)
		setMouseEvent(mouse);
		else
		{
			mouseEvent = MouseEvent(mouseEvent.getPosition(),
				mouseEvent.getMouseWheelChange(),
				mouseEvent.getButton(),mouseEvent.getTimeStamp(),mouseEvent.getPressure(),
				mouseEvent.alt(),mouseEvent.alt(),
				mouseEvent.shift(),mouseEvent.getSourceWidget());
		}

		resetHoverTime();
		
		
		//if the mouse wheel has changed, it is a mouse wheel event
		if(mouseEvent.getMouseWheelChange() != 0 && !isLocationEvent)
		{
			if(widgetUnderMouse && widgetExists(baseWidget,widgetUnderMouse))
			{
				//prevent non modal event
				if(focusMan.getModalWidget() && !widgetIsModalChild(widgetUnderMouse) && !controlWithLock)
				{
					return;
				}

				if(mouseEvent.getMouseWheelChange() > 0)
				{
					
					if( widgetExists(baseWidget,widgetUnderMouse))
					{
						makeRelArgs(widgetUnderMouse);
						widgetUnderMouse->mouseWheelUp(relArgs);
						
					}
					if(widgetExists(baseWidget,widgetUnderMouse))
					{
						widgetUnderMouse->_dispatchMouseListenerEvent(
							MouseEvent::MOUSE_WHEEL_UP,relArgs);
						
					}
				}
				else
				{
					
					if( widgetExists(baseWidget,widgetUnderMouse))
					{
						makeRelArgs(widgetUnderMouse);
						widgetUnderMouse->mouseWheelDown(relArgs);
					}
					if(widgetExists(baseWidget,widgetUnderMouse))
					{	
						widgetUnderMouse->_dispatchMouseListenerEvent(
							MouseEvent::MOUSE_WHEEL_DOWN,relArgs);
					}
				}
					

				return;
			}
		}
		
		//set previous widget
		previousWidgetUnderMouse = widgetUnderMouse;

		//find and set the widget under the mouse
		widgetUnderMouse = recursiveGetWidgetUnderMouse(baseWidget,mouseEvent);
		if(!widgetExists(baseWidget,previousWidgetUnderMouse))
		{
			previousWidgetUnderMouse = NULL;
		}

		if(!widgetExists(baseWidget,widgetUnderMouse))
		{
			widgetUnderMouse = NULL;
		}
		/* the locked control is valid when the mouse
		is down during a mouse move in order to prevent
		other widgets from receiving events until mouse up */
		if(!controlWithLock)
		{

			if(previousWidgetUnderMouse != widgetUnderMouse || (focusMan.getModalWidget() && !widgetIsModalChild(widgetUnderMouse) 
				&& !controlWithLock))
			{
				//invalidate the last hover control
				lastHoveredControl = NULL;

				//prevent non modal event
				if(focusMan.getModalWidget() && !widgetIsModalChild(widgetUnderMouse) 
					&& !controlWithLock)
				{
					//send previous a mouse leave
					if(previousWidgetUnderMouse)
					{
						
						if( widgetExists(baseWidget,previousWidgetUnderMouse))
						{
							makeRelArgs(previousWidgetUnderMouse);
							
							previousWidgetUnderMouse->mouseLeave(relArgs);
							hideToolTip();
							setCursor(getTop()->getEnterCursor());

						}
						if(widgetExists(baseWidget,previousWidgetUnderMouse))
						{
							previousWidgetUnderMouse->_dispatchMouseListenerEvent(
								MouseEvent::MOUSE_LEAVE,relArgs);
						}

					}

					return;
				}

				//send previous a mouse leave
				if(previousWidgetUnderMouse)
				{
					if( widgetExists(baseWidget,previousWidgetUnderMouse))
					{
						
						makeRelArgs(previousWidgetUnderMouse);
						previousWidgetUnderMouse->mouseLeave(relArgs);
						hideToolTip();
					}
					if(widgetExists(baseWidget,previousWidgetUnderMouse))
					{
						previousWidgetUnderMouse->_dispatchMouseListenerEvent(
							MouseEvent::MOUSE_LEAVE,relArgs);
					}
				}
				

				//send the new one an enter
				if(widgetUnderMouse)
				{
					if( widgetExists(baseWidget,widgetUnderMouse))
					{
						makeRelArgs(widgetUnderMouse);
						setCursor(widgetUnderMouse->getEnterCursor());
						widgetUnderMouse->mouseEnter(relArgs);
					}
					if(widgetExists(baseWidget,widgetUnderMouse))
					{
						widgetUnderMouse->_dispatchMouseListenerEvent(
							MouseEvent::MOUSE_ENTER,relArgs);
					}

				}

			}
		
		//prevent non modal event
		if(focusMan.getModalWidget() && !widgetIsModalChild(widgetUnderMouse)
			&& !controlWithLock)
		{
			return;
		}

		//send it a mouse move
		if(widgetUnderMouse && !isLocationEvent)
		{
			if( widgetExists(baseWidget,widgetUnderMouse))
			{
				makeRelArgs(widgetUnderMouse);
				widgetUnderMouse->mouseMove(relArgs);
			}
			if(widgetExists(baseWidget,widgetUnderMouse))
			{
				widgetUnderMouse->_dispatchMouseListenerEvent(
					MouseEvent::MOUSE_MOVE,relArgs);
			}

		}

		}
		else //send a mouse dragged to the locked control
		{
			if(previousWidgetUnderMouse != widgetUnderMouse)
			{
				//invalidate the last hover control
				lastHoveredControl = 0;

				//prevent non modal event
				if(focusMan.getModalWidget() && !widgetIsModalChild(widgetUnderMouse)
					&& !controlWithLock)
				{
					return;
				}

				//send previous a mouse leave
				if(previousWidgetUnderMouse )
					if(previousWidgetUnderMouse == controlWithLock)
					{
						if( widgetExists(baseWidget,controlWithLock))
						{
							makeRelArgs(controlWithLock);
							controlWithLock->mouseLeave(relArgs);
						}
						if(widgetExists(baseWidget,controlWithLock))
						{
							controlWithLock->_dispatchMouseListenerEvent(
								MouseEvent::MOUSE_LEAVE,relArgs);
						}
					}
					

				//send the new one an enter
				if(widgetUnderMouse == controlWithLock)
				{
					if( widgetExists(baseWidget,controlWithLock))
					{
						makeRelArgs(controlWithLock);
						setCursor(widgetUnderMouse->getEnterCursor());
						controlWithLock->mouseEnter(relArgs);
					}

					if(widgetExists(baseWidget,controlWithLock))
					{
						controlWithLock->_dispatchMouseListenerEvent(
							MouseEvent::MOUSE_ENTER,relArgs);
					}
				}

			}

			//prevent non modal event
			if(focusMan.getModalWidget() && !widgetIsModalChild(widgetUnderMouse)
				&& !controlWithLock)
			{
				return;
			}

			if(!isLocationEvent)
			{
				if( widgetExists(baseWidget,controlWithLock))
				{
					makeRelArgs(controlWithLock);
					controlWithLock->mouseDrag(relArgs);
				}

				if(widgetExists(baseWidget,controlWithLock))
				{			
					controlWithLock->_dispatchMouseListenerEvent(
						MouseEvent::MOUSE_DRAG,relArgs);
				}
			}
		}

		}
		

	void Gui::handleMouseDown(const MouseInput &mouse)
	{
		//handle the mouse down event

		//if there is a locked control, leave
		if(controlWithLock && mouse.button != lastMouseButton)
		{
			return;
		}
		
		//obtain mouse data
		setMouseEvent(mouse);
		setMouseButtonDown(mouse.button);

		widgetUnderMouse = recursiveGetWidgetUnderMouse(baseWidget,mouseEvent);
		Widget*wum = widgetUnderMouse;
		//invalidate hovering
		resetHoverTime();

		//prevent hovering on the widget
		lastHoveredControl = wum;

		//tell modal that a mousedown outside of itself has occurred
		if(focusMan.getModalWidget() && !widgetIsModalChild(widgetUnderMouse))
		{
		
			if( widgetExists(baseWidget,focusMan.getModalWidget()))
			{
				makeRelArgs(focusMan.getModalWidget());
				focusMan.getModalWidget()->modalMouseDown(relArgs);
				if(mouseEvent.getButton() == MOUSE_BUTTON_LEFT)
					hideToolTip();
			}
			if(widgetExists(baseWidget,focusMan.getModalWidget()))
			{
				focusMan.getModalWidget()->_dispatchMouseListenerEvent(
					MouseEvent::MOUSE_MODAL_DOWN,relArgs);
			}

			return;
		}

		if(widgetUnderMouse && widgetExists(baseWidget,widgetUnderMouse))
		{
			lastMouseDownControl = widgetUnderMouse;
			if(widgetUnderMouse != focusMan.getFocusedWidget())
			{
				//set focus
				if(!controlWithLock && widgetUnderMouse->isFocusable() && mouseEvent.getButton() == MOUSE_BUTTON_LEFT)
				focusMan.setFocusedWidget(widgetUnderMouse);
			}
			//send a mouse down
			if(controlWithLock && widgetUnderMouse != controlWithLock)
			{
				return;
			}
			
			if( widgetExists(baseWidget,widgetUnderMouse))
			{
				makeRelArgs(widgetUnderMouse);
				widgetUnderMouse->mouseDown(relArgs);
				if(mouseEvent.getButton() == MOUSE_BUTTON_LEFT)
					hideToolTip();
			}
			if(widgetExists(baseWidget,widgetUnderMouse))
			{
				widgetUnderMouse->_dispatchMouseListenerEvent(
					MouseEvent::MOUSE_DOWN,relArgs);
			}
			
		}

		// lock it for dragging purposes
		if(!controlWithLock)
		{
			controlWithLock = widgetUnderMouse;
			mouseUpControl = widgetUnderMouse;
		}


		if(wum != widgetUnderMouse && 
			widgetExists(baseWidget,widgetUnderMouse) && 
			widgetUnderMouse)
		{
			if( widgetExists(baseWidget,widgetUnderMouse))
			{
				makeRelArgs(widgetUnderMouse);
				widgetUnderMouse->mouseDown(relArgs);

				if(mouseEvent.getButton() == MOUSE_BUTTON_LEFT)
					hideToolTip();
			}
			if(widgetExists(baseWidget,widgetUnderMouse))
			{
				widgetUnderMouse->_dispatchMouseListenerEvent(
					MouseEvent::MOUSE_DOWN,relArgs);
			}
		}

	}


	void Gui::handleMouseUp(const MouseInput &mouse)
	{
		//handles the mouse up and click events
		if(mouse.button != lastMouseButton)
		{
			return;
		}

		//obtain mouse arguments
		setMouseEvent(mouse);


		//invalidate the mouse button
		setMouseButtonDown(MOUSE_BUTTON_NONE);

		resetHoverTime();

		widgetUnderMouse = recursiveGetWidgetUnderMouse(baseWidget,mouseEvent);


		//prevent non modal event
		if(focusMan.getModalWidget() && !widgetIsModalChild(widgetUnderMouse) 
			&& !controlWithLock)
		{
			return;
		}

		if(widgetUnderMouse  && widgetExists(baseWidget,widgetUnderMouse))
		{

			if(lastMouseDownControl == widgetUnderMouse && widgetUnderMouse == controlWithLock)
			{
				/* send a click event if the widget hasn't
				changed since mouse down */
				if( widgetExists(baseWidget,widgetUnderMouse))
				{
					makeRelArgs(widgetUnderMouse);

					widgetUnderMouse->mouseClick(relArgs);
				}
				if(widgetExists(baseWidget,widgetUnderMouse))
				{
					widgetUnderMouse->_dispatchMouseListenerEvent(
						MouseEvent::MOUSE_CLICK,relArgs);
				}
			}

			//send mouse up
			Widget* destinationWidget = mouseUpControl;
			if(!widgetExists(baseWidget,mouseUpControl))
			{
				destinationWidget = widgetUnderMouse;
			}
			if(controlWithLock && widgetExists(baseWidget,controlWithLock))
				destinationWidget = controlWithLock;
			if(!canDoubleClick)
			{
				canDoubleClick = true;
				resetDoubleClickTime();
			}
			else
			{
				canDoubleClick = false;
				if(destinationWidget)
				{
					if( widgetExists(baseWidget,destinationWidget))
					{
						makeRelArgs(destinationWidget);
						destinationWidget->mouseDoubleClick(relArgs);
					}
					if(widgetExists(baseWidget,destinationWidget))
					{
						destinationWidget->_dispatchMouseListenerEvent(
							MouseEvent::MOUSE_DOUBLE_CLICK,relArgs);
					}
				}
			
			}

			if(destinationWidget)
			{
				if( widgetExists(baseWidget,destinationWidget))
				{
					makeRelArgs(destinationWidget);
					destinationWidget->mouseUp(relArgs);
				}
				if(widgetExists(baseWidget,destinationWidget))
				{
					destinationWidget->_dispatchMouseListenerEvent(
						MouseEvent::MOUSE_UP,relArgs);
				}
			}
			
			
			
		}

		//unlock the control
		
			if(widgetUnderMouse != controlWithLock)
			{
				if(widgetUnderMouse)
				{
					if(controlWithLock && widgetExists(baseWidget,controlWithLock))
					{
						if( widgetExists(baseWidget,controlWithLock))
						{
							makeRelArgs(controlWithLock);
							controlWithLock->mouseLeave(relArgs);
						
						}
						if(widgetExists(baseWidget,controlWithLock))
						{
							controlWithLock->_dispatchMouseListenerEvent(
								MouseEvent::MOUSE_LEAVE,relArgs);
						}
					}

					if( widgetExists(baseWidget,widgetUnderMouse))
					{
						makeRelArgs(widgetUnderMouse);
						setCursor(widgetUnderMouse->getEnterCursor());
						widgetUnderMouse->mouseEnter(relArgs);
					}
					if(widgetExists(baseWidget,widgetUnderMouse))
					{
						widgetUnderMouse->_dispatchMouseListenerEvent(
							MouseEvent::MOUSE_ENTER,relArgs);
					}
					
				}
			
			}
		
		if(controlWithLock && widgetExists(baseWidget,controlWithLock))
		{
			if(!controlWithLock->keepMouseLock(mouseEvent.getButton()))
			{
				controlWithLock = NULL;
			}
		}
		else
		{
			controlWithLock = NULL;
		}
	}



	Widget* Gui::recursiveGetWidgetUnderMouse( Widget* root,
												const MouseEvent &mouse )
	{
		/* sets the current node to the root node
		while a widget passes the hit test
		and the current node has children,
		iterate through the current node's 
		children and set the current node
		to the child who passes the hit test */


		Widget* wMouse = root;
		Widget* currentNode = root;
		q.push(root);

			while(!q.empty())
			{
				currentNode = q.top();
				q.pop();

				if((currentNode->intersectionWithPoint(Point(
					mouse.getPosition().getX() - currentNode->getAbsolutePosition().getX(),
					mouse.getPosition().getY() - currentNode->getAbsolutePosition().getY())))
					&& currentNode->isEnabled() && currentNode->isVisible())
				{
					wMouse = currentNode;
				}


				if(currentNode == root || 
					((currentNode->intersectionWithPoint(Point(
					mouse.getPosition().getX() - currentNode->getAbsolutePosition().getX(),
					mouse.getPosition().getY() - currentNode->getAbsolutePosition().getY())))
					&& currentNode->isEnabled() && currentNode->isVisible()))
				{
					for (WidgetArray::const_reverse_iterator rit = 
						currentNode->getChildRBegin();
						rit != currentNode->getChildREnd(); ++rit) 
					{ 
						q.push(*rit);

					} 

					for (WidgetArray::const_reverse_iterator rit = 
						currentNode->getPrivateChildRBegin();
						rit != currentNode->getPrivateChildREnd(); ++rit) 
					{ 
						q.push(*rit);
					} 
				}

				
			}
			return wMouse;
		

	}

	void Gui::resetHoverTime()
	{
		//invalidates the time before a hover event is fired
		timeUntilNextHover = input->getTime() + hoverInterval;
	}

	double Gui::getHoverInterval() const
	{
		return hoverInterval;
	}

	void Gui::handleKeyDown( const KeyboardInput &keyboard )
	{
		setKeyEvent(keyboard,false);

			if(handleTabbing())
			{
				return;
			}
		

		if(focusMan.getFocusedWidget()  && widgetExists(baseWidget,focusMan.getFocusedWidget()))
		{
			
			if(widgetExists(baseWidget,focusMan.getFocusedWidget()))
			{
				focusMan.getFocusedWidget()->keyDown(keyEvent);
			}

			if(widgetExists(baseWidget,focusMan.getFocusedWidget()))
			{
				focusMan.getFocusedWidget()->_dispatchKeyboardListenerEvent(
					KeyEvent::KEY_DOWN,keyEvent);
			}
		}

	
	}

	void Gui::setKeyEvent( const KeyboardInput &keyboard,bool handled )
	{
		keyEvent = KeyEvent(keyboard.key,keyboard.extKey,(int)keyboard._key,
			(int)keyboard._modifierKey,
			keyboard.unichar,keyboard.timeStamp,keyboard.isAlt,
			keyboard.isControl,keyboard.isShift,keyboard.isMeta);

		if(handled)
		keyEvent.consume();
	}

	void Gui::handleKeyUp( const KeyboardInput &keyboard)
	{
		setKeyEvent(keyboard,false);

		if(focusMan.getFocusedWidget()  && widgetExists(baseWidget,focusMan.getFocusedWidget()))
		{

			if(widgetExists(baseWidget,focusMan.getFocusedWidget()))
			{
				focusMan.getFocusedWidget()->keyUp(keyEvent);
			}

			if(widgetExists(baseWidget,focusMan.getFocusedWidget()))
			{
				focusMan.getFocusedWidget()->_dispatchKeyboardListenerEvent(
					KeyEvent::KEY_UP,keyEvent);
			}
		}

	}

	void Gui::handleKeyRepeat( const KeyboardInput &keyboard)
	{
		setKeyEvent(keyboard,false);

		if(handleTabbing())
		{
			return;
		}

		if(focusMan.getFocusedWidget()  && widgetExists(baseWidget,focusMan.getFocusedWidget()))
		{

			if(widgetExists(baseWidget,focusMan.getFocusedWidget()))
			{
				focusMan.getFocusedWidget()->keyRepeat(keyEvent);
			}

			if(widgetExists(baseWidget,focusMan.getFocusedWidget()))
			{
				focusMan.getFocusedWidget()->_dispatchKeyboardListenerEvent(
					KeyEvent::KEY_REPEAT,keyEvent);
			}
		}


	}



	Widget* Gui::getWidgetUnderMouse() const
	{
		return widgetUnderMouse;
	}


	MouseButtonEnum Gui::getMouseButtonDown() const
	{
		return lastMouseButton;
	}

	void Gui::setMouseButtonDown( MouseButtonEnum button )
	{
		lastMouseButton = button;
	}

	void Gui::setMouseEvent( const MouseInput &mouse)
	{
		MouseButtonEnum button = mouse.button;
		//create mouse data
		if(controlWithLock && mouse.button == MOUSE_BUTTON_NONE)
		{
			button = lastMouseButton;
		}
		mouseEvent = MouseEvent(Point
			(mouse.x,mouse.y),mouse.wheel,button,mouse.timeStamp,
			mouse.pressure,
			mouse.isAlt, mouse.isControl, mouse.isShift);
	}




	MouseEvent Gui::getMouseEvent() const
	{
		return mouseEvent;
	}

	void Gui::setLastMouseDownControl( Widget* control )
	{
		lastMouseDownControl = control;
	}

	Widget* Gui::getLastMouseDownControl() const
	{
		return lastMouseDownControl;
	}

	void Gui::handleHover()
	{
		if(controlWithLock != NULL && !focusMan.getModalWidget())
		{
			return;
		}

		//dispatches a hover event
		if(input->getTime() > timeUntilNextHover)
		{
			resetHoverTime();

			if(widgetUnderMouse != lastHoveredControl)
			{
				lastHoveredControl = widgetUnderMouse;
				if(widgetUnderMouse && widgetExists(baseWidget,widgetUnderMouse))
				{
					if((focusMan.getModalWidget() && widgetIsModalChild(widgetUnderMouse))
						|| !focusMan.getModalWidget())
					{
						if( widgetExists(baseWidget,widgetUnderMouse))
						{
							makeRelArgs(widgetUnderMouse);
							widgetUnderMouse->_dispatchMouseListenerEvent(
								MouseEvent::MOUSE_HOVER,relArgs);
						}
						if(widgetExists(baseWidget,widgetUnderMouse))
						{
							showToolTip(widgetUnderMouse,
								mouseEvent.getX(),mouseEvent.getY());
							widgetUnderMouse->mouseHover(relArgs);
						}
					}
					
				}
			}
		}
	}

	void Gui::setHoverInterval(double time)
	{
		//set the amount of time before a hover is fired
		hoverInterval = time;
	}

	void Gui::resetDoubleClickTime()
	{
		doubleClickExpireTime = input->getTime() + doubleClickInterval;
	}

	void Gui::handleDoubleClick()
	{
		//prevent non modal event

		if(canDoubleClick)
		{
			if( input->getTime() > doubleClickExpireTime)
			{
				canDoubleClick = false;
			}
		}
	}


	double Gui::getDoubleClickInterval() const
	{
		return doubleClickInterval;
	}

	void Gui::setDoubleClickInterval( double time )
	{
		doubleClickInterval = time;
	}

	bool Gui::widgetIsModalChild(Widget* widget) const
	{
		if(!focusMan.getModalWidget())
		{
			return false;
		}

		if(widget == focusMan.getModalWidget())
		{
			return true;
		}

		if(widget == toolTip)
		{
			return true;
		}
		Widget* currentParent = widget;
		if(widget)
		{
			while (currentParent)
			{
				if(currentParent == focusMan.getModalWidget())
				{
					return true;
				}
				if(!currentParent->getParent())
				{
					return false;
				}
				currentParent = currentParent->getParent();
			}
		
		}
		return false;
	}


	bool Gui::widgetExists(const Widget* root, const Widget* target ) const
	{
		if(!enableExistanceCheck)
		{
			return true;
		}

		if(root == target)
		{
			return true;
		}

		for(WidgetArray::const_iterator it
			= root->getPrivateChildBegin(); 
			it != root->getPrivateChildEnd(); ++it)
		{
			if(widgetExists((*it),target)) {return true;}
		}

		for(WidgetArray::const_iterator it 
			= root->getChildBegin(); 
			it != root->getChildEnd(); ++it)
		{
			if(widgetExists(*it,target)) {return true;}
		}

		return false;

	}

	bool Gui::widgetExists( Widget* target )
	{
		bool exch = enableExistanceCheck;
		enableExistanceCheck = true;
		bool r = widgetExists(baseWidget,target);
		enableExistanceCheck = exch;
		return r;
	}

	Widget* Gui::getFocusedWidget() const 
	{
		return focusMan.getFocusedWidget();
	}

	bool Gui::recursiveFocusNext( Widget* target, 
											  Widget* focused)
	{
		if(focused == NULL && !passedFocus)
		{
			passedFocus = true;
		}

		if(target)
		if(target != focused && target->isFocusable() 
			&& target->isTabable() && passedFocus)
		{
			target->focus();
			return true;
		}

		if(target->isFocused() && target != focused)
		{
			return true;
		}

		if(target == focused)
		{
			passedFocus = true;
		}


		if(target && target->isVisible() && target->isEnabled())
		{

		
			for(WidgetArray::const_iterator it = 
				target->getPrivateChildBegin(); 
				it != target->getPrivateChildEnd(); ++it)
			{
				if(recursiveFocusNext((*it),focused))
				{
					return true;
				}
			}

			for(WidgetArray::const_iterator it = 
				target->getChildBegin(); 
				it != target->getChildEnd(); ++it)
			{
				if(recursiveFocusNext((*it),focused))
				{
					return true;
				}
			}

		}

		return false;

	}

	void Gui::focusNextTabableWidget()
	{
		if(focusMan.getModalWidget())
		{
			if(focusMan.getModalWidget()->isFocusable() && focusMan.getModalWidget()->isTabable()
				&& !focusMan.getModalWidget()->isFocused())
			{
				focusMan.getModalWidget()->focus();
			}
			return;
		}

		passedFocus = false;

		if(!recursiveFocusNext(baseWidget,focusMan.getFocusedWidget()))
		{
			passedFocus = true;
			recursiveFocusNext(baseWidget,focusMan.getFocusedWidget());
		}
		


	}

	bool Gui::recursiveFocusPrevious( Widget* target, 
												  Widget* focused )
	{
		if(focused == NULL && !passedFocus)
		{
			passedFocus = true;
		}

			if(target && target->isVisible() && target->isEnabled())
			{
				for(WidgetArray::const_reverse_iterator it = 
					target->getPrivateChildRBegin(); 
					it != target->getPrivateChildREnd(); ++it)
				{
					if(recursiveFocusPrevious((*it),focused))
					{
						return true;
					}
				}
				for(WidgetArray::const_reverse_iterator it = 
					target->getChildRBegin(); 
					it != target->getChildREnd(); ++it)
				{
					if(recursiveFocusPrevious((*it),focused))
					{
						return true;
					}
				}
			}

				if(target)
					if(target != focused && target->isFocusable() 
						&& target->isTabable() && passedFocus)
					{
						//tabbed panes should not be previously tabbed
						//to avoid circular dependency

						if(target->isReverseTabable())
						{
							target->focus();
							return true;
						}
					
					}

					if(target->isFocused() && target != focused)
					{
						return true;
					}

					if(target == focused)
					{
						passedFocus = true;
					}
				return false;
	}

	void Gui::focusPreviousTabableWidget()
	{
		if(focusMan.getModalWidget())
		{
			if(focusMan.getModalWidget()->isFocusable() && focusMan.getModalWidget()->isTabable()
				&& !focusMan.getModalWidget()->isFocused())
			{
				focusMan.getModalWidget()->focus();
			}
			return;
		}

		passedFocus = false;

		if(!recursiveFocusPrevious(baseWidget,focusMan.getFocusedWidget()))
		{
			passedFocus = true;
			recursiveFocusPrevious(baseWidget,focusMan.getFocusedWidget());
		}

	}

	bool Gui::isTabbingEnabled() const
	{
		return tabbingEnabled;
	}

	void Gui::setTabbingEnabled( bool tabbing )
	{
		tabbingEnabled = tabbing;
	}


	void Gui::_widgetLocationChanged()
	{
		if(wantWidgetLocationChanged)
		handleMouseAxes(emptyMouse,true);
	}

	void Gui::_removeWidget( Widget *widget )
	{
		_widgetLocationChanged();

		if(isToolTipVisible())
		{
			if(toolTip->getInvoker() == widget)
			{
				hideToolTip();
			}
		}

		if(widget == widgetUnderMouse)
			widgetUnderMouse = NULL;

		if(widget == lastMouseDownControl)
			lastMouseDownControl = NULL;

		if(widget == previousWidgetUnderMouse)
			previousWidgetUnderMouse = NULL;

		if(widget == focusMan.getFocusedWidget())
			focusMan.focusWidgetDestroyed();

		if(widget == controlWithLock)
			controlWithLock = NULL;

		if(widget == lastHoveredControl)
			lastHoveredControl = NULL;

		if(widget == focusMan.getModalWidget())
			focusMan.releaseModalFocus(widget);
	}


	void Gui::makeRelArgs( Widget *source )
	{
		relArgs = MouseEvent(Point(
			mouseEvent.getX() - source->getAbsolutePosition().getX(),
			mouseEvent.getY() - source->getAbsolutePosition().getY()),
			mouseEvent.getMouseWheelChange(),
			mouseEvent.getButton(),mouseEvent.getTimeStamp(),
			mouseEvent.getPressure(),
			mouseEvent.alt(),
			mouseEvent.control(),
			mouseEvent.shift(),
			source);
	}

	void Gui::logic()
	{
		input->pollInput();
		_dispatchKeyboardEvents();
		_dispatchMouseEvents();
		handleTimedEvents();
		currentTime = input->getTime();
		recursiveDoLogic(baseWidget);
		if(destroyingFlaggedWidgets)
		{
			destroyFlaggedWidgets();
		}
	}

	void Gui::recursiveDoLogic( Widget* baseWidget )
	{
		baseWidget->logic(currentTime);

		for(WidgetArray::iterator it = 
			baseWidget->getPrivateChildBegin();
			it != baseWidget->getPrivateChildEnd(); ++it)
		{
			recursiveDoLogic(*it);
		}

		if(!baseWidget->isChildlogicHandled())
		for(WidgetArray::iterator it = 
			baseWidget->getChildBegin();
			it != baseWidget->getChildEnd(); ++it)
		{
			recursiveDoLogic(*it);
		}
	}

	void Gui::setGraphics( Graphics *context )
	{
		graphicsContext = context;
		if(input)
		{
			baseWidget->setSize(graphicsContext->getDisplaySize());
		}
		
	}

	void Gui::setInput( Input* input )
	{
		this->input = input;
		if(graphicsContext)
		{
			baseWidget->setSize(graphicsContext->getDisplaySize());
		}
	}

	void Gui::setSize( int width, int height )
	{
		baseWidget->setSize(width,height);
	}

	void Gui::render()
	{
		graphicsContext->clearClippingStack();

		baseWidget->_recursivePaintChildren(baseWidget,true,graphicsContext);
		graphicsContext->clearClippingStack();
		graphicsContext->setOffset(Point(0,0));
		graphicsContext->pushClippingRect(Rectangle(Point(0,0),baseWidget->getSize()));

	}

	const double Gui::getElapsedTime() const
	{
		return input->getTime();
	}

	void Gui::addKeyPreviewListener( KeyboardListener* listener )
	{
		keyPreviewListeners.push_back(listener);
	}

	void Gui::removeKeyPreviewListener( KeyboardListener* listener )
	{
		keyPreviewListeners.erase(std::remove(keyPreviewListeners.begin(),
			keyPreviewListeners.end(), listener), keyPreviewListeners.end());
	}

	void Gui::_dispatchKeyPreview( KeyEvent &keyEvent, 
		KeyEvent::KeyboardEventEnum type )
	{
		for(std::vector<KeyboardListener*>::iterator it =
			keyPreviewListeners.begin();
			it != keyPreviewListeners.end(); ++it)
		{
			switch (type)
			{
			case KeyEvent::KEY_DOWN:
				(*it)->keyDownCB(keyEvent);
				break;
			case KeyEvent::KEY_UP:
				(*it)->keyUpCB(keyEvent);
				break;
			case KeyEvent::KEY_REPEAT:
				(*it)->keyRepeatCB(keyEvent);
				break;
			default:
				break;
			}
		}
	}

	void Gui::_dispatchKeyboardEvents()
	{
		while(!input->isKeyboardQueueEmpty())
		{
			KeyboardInput kb = input->dequeueKeyboardInput();
			setKeyEvent(kb,false);
			_dispatchKeyPreview(keyEvent,kb.type);
			if(keyEvent.isConsumed())
			{
				continue;
			}

			switch(kb.type)
			{
			case KeyEvent::KEY_DOWN:

 				handleKeyDown(kb);
				break;
			case KeyEvent::KEY_REPEAT:
				handleKeyRepeat(kb);
				break;
			case KeyEvent::KEY_UP:
				handleKeyUp(kb);

				break;
			default:
				break;
			}
		}
	}

	void Gui::_dispatchMouseEvents()
	{
		while(!queuedMouseDown.empty())
		{
			MouseInput mi = queuedMouseDown.back();
			queuedMouseDown.pop();

			if(mi.type == MouseEvent::MOUSE_DOWN)
			{
				_dispatchMousePreview(mi,mi.type);
				if(!mouseEvent.isConsumed())
				handleMouseDown(mi);
			}
		}

		while(!input->isMouseQueueEmpty())
		{
			MouseInput mi = input->dequeueMouseInput();
			if(
				mi.type == MouseEvent::MOUSE_MOVE ||
				mi.type == MouseEvent::MOUSE_WHEEL_DOWN ||
				mi.type == MouseEvent::MOUSE_WHEEL_UP)
			{
				_dispatchMousePreview(mi,mi.type);
				if(!mouseEvent.isConsumed())
				handleMouseAxes(mi,false);
			}
			else if(mi.type == MouseEvent::MOUSE_DOWN)
			{
				//queue it for later to fix click bug
				if(!queuedMouseDown.empty())
				{
					queuedMouseDown.pop();
				}
				queuedMouseDown.push(mi);
				return;
			}
			else if(mi.type == MouseEvent::MOUSE_UP)
			{
				_dispatchMousePreview(mi,mi.type);
				if(!mouseEvent.isConsumed())
				handleMouseUp(mi);
			}
		}

		
	}

	void Gui::_dispatchWidgetDestroyed( Widget* widget )
	{
			_removeWidget(widget);
	}


	void Gui::add( Widget* widget )
	{
		baseWidget->add(widget);
	}

	void Gui::remove( Widget* widget )
	{
		baseWidget->remove(widget);
	}

	void Gui::resizeToDisplay()
	{
		if(graphicsContext)
		baseWidget->setSize(graphicsContext->getDisplaySize());
	}

	void Gui::handleTimedEvents()
	{
		handleHover();
		handleDoubleClick();
		handleToolTip();
	}

	void Gui::setTabNextKey( KeyEnum key,
		ExtendedKeyEnum extKey,
		bool shift /*= false*/,
		bool control /*= false*/, 
		bool alt /*= false*/ )
	{
		tabNextKey = key;
		tabNextShift = shift;
		tabNextControl = control;
		tabNextAlt = alt;
		tabNextExtKey = extKey;
	}

	void Gui::setTabPreviousKey( KeyEnum key,
		ExtendedKeyEnum extKey,
		bool shift /*= false*/,
		bool control /*= false*/,
		bool alt /*= false*/ )
	{
		tabPreviousKey = key;
		tabPreviousShift = shift;
		tabPreviousControl = control;
		tabPreviousAlt = alt;
		tabPreviousExtKey = extKey;
	}

	bool Gui::handleTabbing()
	{
		if((tabNextKey != KEY_NONE && keyEvent.getKey() == tabNextKey ||
			tabNextExtKey != EXT_KEY_NONE && 
			keyEvent.getExtendedKey() == tabNextExtKey) &&
			keyEvent.shift() == tabNextShift &&
			keyEvent.control() == tabNextControl &&
			keyEvent.alt() == tabNextAlt)
		{
			focusNextTabableWidget();
			return true;
		}
		else if((tabPreviousKey != KEY_NONE && keyEvent.getKey() == tabPreviousKey ||
			tabPreviousExtKey != EXT_KEY_NONE && 
			keyEvent.getExtendedKey() == tabPreviousExtKey) &&
			keyEvent.shift() == tabPreviousShift &&
			keyEvent.control() == tabPreviousControl &&
			keyEvent.alt() == tabPreviousAlt)
		{
			focusPreviousTabableWidget();
			return true;
		}

		return false;
	}

	TopContainer* Gui::getTop() const
	{
		return baseWidget;
	}

	std::stack<Widget*>& Gui::getFlaggedWidgets()
	{
		return flaggedWidgets;
	}

	void Gui::destroyFlaggedWidgets()
	{
		while(!flaggedWidgets.empty())
		{
			Widget* c = flaggedWidgets.top();

			delete flaggedWidgets.top();
			flaggedWidgets.pop();
		}
	}

	void Gui::flagWidget( Widget *widget )
	{
		flaggedWidgets.push(widget);
	}

	void Gui::setDestroyFlaggedWidgets( bool destroying )
	{
		destroyingFlaggedWidgets = destroying;
	}

	bool Gui::isDestroyingFlaggedWidgets() const
	{
		return destroyingFlaggedWidgets;
	}

	void Gui::setToolTip( ToolTip* toolTip )
	{
		this->toolTip = toolTip;
		getTop()->add(toolTip);
	}

	ToolTip* Gui::getToolTip() const
	{
		return toolTip;
	}

	void Gui::setMaxToolTipWidth( int width )
	{
		maxToolTipWidth = width;
	}

	int Gui::getMaxToolTipWidth() const
	{
		return maxToolTipWidth;
	}

	void Gui::hideToolTip()
	{
		if(toolTip)
		{
			toolTip->hideToolTip();
			hasHiddenToolTip = true;
		}
	}

	void Gui::showToolTip( Widget* widget, int x, int y)
	{
		if(toolTip && widget && widget->getToolTipText().length() > 0)
		{
			if(toolTip->getParent() == NULL)
			{
				getTop()->add(toolTip);
			}
			toolTip->bringToFront();
			toolTip->showToolTip(
				widget->getToolTipText(),
				getMaxToolTipWidth(),
				x + toolTip->getPreferredOffset().getX(),
				y + toolTip->getPreferredOffset().getY(),
				widget);

			hasHiddenToolTip = false;
			lastToolTipTime = getElapsedTime();
		}
	}

	double Gui::getToolTipShowLength() const
	{
		return toolTipShowLength;
	}

	void Gui::setToolTipShowLength( double val )
	{
		toolTipShowLength = val;
	}

	void Gui::handleToolTip()
	{
		if(getElapsedTime() - lastToolTipTime > toolTipShowLength 
			&& !hasHiddenToolTip)
		{
			hasHiddenToolTip = true;
			hideToolTip();
		}
	}

	void Gui::_dispatchMousePreview( const MouseInput& input, MouseEvent::MouseEventEnum type )
	{
		setMouseEvent(input);

		if(type == MouseEvent::MOUSE_MOVE)
		{
			if(mouseEvent.getMouseWheelChange() > 0)
			{
				type = MouseEvent::MOUSE_WHEEL_UP;
			}
			else if(mouseEvent.getMouseWheelChange() < 0)
			{
				type = MouseEvent::MOUSE_WHEEL_DOWN;
			}
		}

		for(std::vector<MouseListener*>::iterator it =
			mousePreviewListeners.begin();
			it != mousePreviewListeners.end(); ++it)
		{
			switch (type)
			{
			case MouseEvent::MOUSE_DOWN:
				(*it)->mouseDownCB(mouseEvent);
				break;
			case MouseEvent::MOUSE_UP:
				(*it)->mouseUpCB(mouseEvent);
				break;
			case MouseEvent::MOUSE_WHEEL_DOWN:
				(*it)->mouseWheelDownCB(mouseEvent);
				break;
			case MouseEvent::MOUSE_WHEEL_UP:
				(*it)->mouseWheelUpCB(mouseEvent);
				break;
			case MouseEvent::MOUSE_MOVE:
				(*it)->mouseMoveCB(mouseEvent);
				break;
			default:
				break;
			}
		}
	}

	void Gui::addMousePreviewListener( MouseListener* listener )
	{
		mousePreviewListeners.push_back(listener);
	}

	void Gui::removeMousePreviewListener( MouseListener* listener )
	{
		mousePreviewListeners.erase(std::remove(mousePreviewListeners.begin(),
			mousePreviewListeners.end(), listener), mousePreviewListeners.end());
	}

	void Gui::invalidateToolTip()
	{
		lastHoveredControl = NULL;
		hideToolTip();
	}

	bool Gui::setCursor( CursorProvider::CursorEnum cursor )
	{
		if(cursorProvider)
		{
			return cursorProvider->setCursor(cursor);
		}

		return false;
	}

	void Gui::setCursorProvider( CursorProvider* provider )
	{
		cursorProvider = provider;
	}

	Widget* Gui::getLockWidget()
	{
		return controlWithLock;
	}

	void Gui::toggleWidgetLocationChanged( bool on )
	{
		wantWidgetLocationChanged = on;
	}

	void Gui::setExistanceCheck( bool check )
	{
		enableExistanceCheck = check;
	}

	int Gui::isDoingExistanceCheck() const
	{
		return enableExistanceCheck;
	}

	void Gui::_modalChanged()
	{
		_widgetLocationChanged();
	}

	bool Gui::isToolTipVisible() const
	{
		if(toolTip)
		{
			return toolTip->isVisible();
		}

		return false;
	}

}
