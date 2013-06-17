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

#include "TextField.hpp"

namespace cge
{
	TextField::~TextField(void)
	{
	}

	TextField::TextField( agui::Image* bgImage, agui::Image* focusImage, PopUpMenu* menu )
		:m_bgImage(bgImage), m_focusImage(focusImage), m_menu(menu),
		m_cutItem("Cut", "Ctrl + X"), m_copyItem("Copy","Ctrl + C"), 
		m_pasteItem("Paste","Ctrl + V"), m_selectAllItem("Select All","Ctrl + A"),
		m_deleteItem("Delete"),
		m_separatorItem(agui::PopUpMenuItem::SEPARATOR)
	{
		if(menu)
		{
			menu->addItem(&m_cutItem);
			menu->addItem(&m_copyItem);
			menu->addItem(&m_pasteItem);
			menu->addItem(&m_deleteItem);
			menu->addItem(&m_separatorItem);
			menu->addItem(&m_selectAllItem);
			menu->addActionListener(this);
		}
	}

	void TextField::paintBackground( const agui::PaintEvent &paintEvent )
	{
		agui::Image* img = NULL;
		if(isFocused())
		{
			img = m_focusImage;
		}
		else
		{
			img = m_bgImage;
		}

		paintEvent.graphics()->drawNinePatchImage(img,agui::Point(0,0),getSize());
	}

	void TextField::paintComponent( const agui::PaintEvent &paintEvent )
	{
		int caretLoc = getCaretLocation();
		int textLoc = getTextOffset();

		agui::Rectangle sideclip = getInnerRectangle();
		sideclip = agui::Rectangle(sideclip.getX() + getLeftPadding() ,
			sideclip.getY() + 2,sideclip.getSize().getWidth() - getLeftPadding()
			- getRightPadding() + 1, sideclip.getHeight() - 4);

		paintEvent.graphics()->pushClippingRect(sideclip);

		if(getSelectionStart() != getSelectionEnd() && (isFocused() || !isHidingSelection()) )
		{
			agui::Rectangle selRect = agui::Rectangle(
				getSelectionLocation(),
				(getInnerHeight() / 2) - 
				(getFont()->getLineHeight() / 2),
				getSelectionWidth(),
				getFont()->getLineHeight());

			paintEvent.graphics()->drawFilledRectangle(
				selRect,getSelectionBackColor());
		}


		paintEvent.graphics()->drawText(agui::Point(textLoc, +
			((getInnerSize().getHeight() - getFont()->getLineHeight()) / 2)),getText().c_str(),
			getFontColor(),getFont());


		if(isFocused())
		{
			if(isBlinking())
				paintEvent.graphics()->drawLine(agui::Point(caretLoc + 1,
				((getInnerSize().getHeight() / 2) + (getFont()->getLineHeight() / 2))),
				agui::Point(caretLoc + 1, ((getInnerSize().getHeight() / 2) - 
				(getFont()->getLineHeight() / 2))),
				agui::Color(0,0,0));
		}

		paintEvent.graphics()->popClippingRect();

	}

	void TextField::actionPerformed( const agui::ActionEvent &evt )
	{
		if(evt.getSource() == &m_selectAllItem)
		{
			selectAll();
			focus();
		}
		else if(evt.getSource() == &m_copyItem)
		{
			copy();
			focus();
		}
		else if(evt.getSource() == &m_pasteItem)
		{
			if(!isReadOnly())
			{
				paste();
				focus();
			}
		}
		else if(evt.getSource() == &m_cutItem)
		{
			cut();
			focus();
		}
		else if(evt.getSource() == &m_deleteItem)
		{
			if(!isReadOnly())
			{
				deleteSelection();
				focus();
			}
		}
	}

	void TextField::mouseDown( agui::MouseEvent &mouseEvent )
	{
		agui::TextField::mouseDown(mouseEvent);
		if(mouseEvent.getButton() == agui::MOUSE_BUTTON_RIGHT)
		{
			if(m_menu)
			{
				m_pasteItem.setEnabled(!isReadOnly());
				m_cutItem.setEnabled(!isReadOnly() && !(getSelectionStart() == getSelectionEnd()));
				m_copyItem.setEnabled(!(getSelectionStart() == getSelectionEnd()));
				m_deleteItem.setEnabled(!isReadOnly() && !(getSelectionStart() == getSelectionEnd()));
				m_menu->showPopUp(this,mouseEvent.getX(),mouseEvent.getY());
			}
		}
	}

}
