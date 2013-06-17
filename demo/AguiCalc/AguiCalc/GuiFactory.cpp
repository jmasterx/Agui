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

#include "GuiFactory.hpp"
#include "Button.hpp"
namespace cge
{
	GuiFactory::GuiFactory(void)
		:
		m_imageManager(NULL),
		m_fontManager(NULL)
	{
	}

	GuiFactory::~GuiFactory(void)
	{
		for(std::vector<agui::Widget*>::reverse_iterator it = m_widgets.rbegin();
			it != m_widgets.rend(); ++it)
		{
			delete *it;
		}

		m_widgets.clear();
	}

	Button* GuiFactory::createButton()
	{
		Button* button = new Button(
			m_imageManager->getImage("button.default"),
			m_imageManager->getImage("button.hover"),
			m_imageManager->getImage("button.click"), 
			m_imageManager->getImage("button.focus"),
			m_imageManager->getImage("button.disabled"),
			NULL
			);

		button->setMargins(0,1,3,4);
		button->setMinSize(agui::Dimension(33,12));
		button->setFontColor(agui::Color(255,255,255));
		button->setFocusable(true);

		m_widgets.push_back(button);

		return button;
	}

	void GuiFactory::setImageManager( GuiImageManager* manager )
	{
		m_imageManager = manager;
	}

	TextField* GuiFactory::createTextField()
	{
		TextField* textfield = new TextField(
			m_imageManager->getImage("textfield.bg"),
			m_imageManager->getImage("textfield.focus"),
			createPopUpMenu());

		textfield->setSelectionBackColor(agui::Color(179,203,224));
		textfield->setMargins(3,4,4,4);
		textfield->setFontColor(agui::Color(24,76,108));
		textfield->setPasswordCharacter(9679);
		textfield->setHideSelection(false);

		m_widgets.push_back(textfield);
		return textfield;
	}


	ToolTip* GuiFactory::createToolTip()
	{
		ToolTip* tooltip = new ToolTip(
			m_imageManager->getImage("tooltip.bg"));
		tooltip->setMargins(6,7,11,10);
		tooltip->setFontColor(agui::Color(16,68,102));
		m_widgets.push_back(tooltip);
		return tooltip;

	}

	PopUpMenu* GuiFactory::createPopUpMenu()
	{
		PopUpMenu* popup = new PopUpMenu(	
			m_imageManager->getImage("popup.bg"),
			m_imageManager->getImage("popup.highlight"),
			m_imageManager->getImage("popup.highlight"));

		popup->setMargins(4,4,7,7);
		popup->setFontColor(agui::Color(4,56,90));
		popup->setChildOffset(agui::Point(-4,0));
		popup->setFont(m_fontManager->getFont(0.9f));

		m_widgets.push_back(popup);

		return popup;
	}

	Frame* GuiFactory::createFrame( agui::Widget* content )
	{
		Frame* frame = new Frame(m_imageManager->getImage("frame.bg"),
			content);

		frame->setMargins(9,17,42,33);
		frame->setTopMargin(10);
		frame->setLeftMargin(5);
		frame->setBottomMargin(3);
		frame->setRightMargin(6);
		frame->getContentPane()->setMargins(0,0,0,0);
		frame->setBackColor(agui::Color(184,203,218));
		frame->setMinSize(agui::Dimension(150,150));
		m_widgets.push_back(frame);
		return frame;
	}

	BorderLayout* GuiFactory::createBorderLayout()
	{
		BorderLayout* border = new BorderLayout(
			NULL,NULL,NULL
			);
		border->setVerticalSpacing(1);
		border->setHorizontalSpacing(2);

		m_widgets.push_back(border);

		return border;
	}

	void GuiFactory::setFontManager( GuiFontManager* manager )
	{
		m_fontManager = manager;
	}

	GridLayout* GuiFactory::createGridLayout()
	{
		GridLayout* grid = new GridLayout();
		grid->setMargins(3,3,5,3);
		m_widgets.push_back(grid);

		return grid;
	}

	FlowLayout* GuiFactory::createFlowLayout()
	{
		FlowLayout* flow = new FlowLayout();
		flow->setMargins(0,0,0,0);

		m_widgets.push_back(flow);

		return flow;
	}

	agui::EmptyWidget* GuiFactory::createEmptyWidget()
	{
		agui::EmptyWidget* empty = new agui::EmptyWidget();
		m_widgets.push_back(empty);
		return empty;
	}

	agui::Label* GuiFactory::createLabel()
	{
		agui::Label* label = new agui::Label();
		label->setFontColor(agui::Color(3,65,142));
		m_widgets.push_back(label);
		return label;
	}

	Button* GuiFactory::createWhiteButton( 
		const std::string& icon /*= ""*/, const std::string& textIcon /*= ""*/ )
	{
		agui::Image* ico = icon == "" ? NULL : m_imageManager->getImage(icon);
		Button* button = new Button(
			m_imageManager->getImage("whitebutton.default"),
			m_imageManager->getImage("whitebutton.hover"),
			m_imageManager->getImage("whitebutton.click"), 
			NULL,
			NULL,
			ico
			);

		button->setMargins(2,2,6,6);

		button->setContentResizeScale(1.2f,1.2f);
		button->setMinSize(agui::Dimension(33,12));
		button->setFontColor(agui::Color(3,65,142));

		if(textIcon != "")
		{
			button->setMargins(2,3,6,6);
			button->setTextImage(m_imageManager->getImage(textIcon));
		}
		m_widgets.push_back(button);

		return button;
	}
	void GuiFactory::deleteWidget( agui::Widget* w )
	{
		for(size_t i = 0; i < m_widgets.size(); ++i)
		{
			if(m_widgets[i] == w)
			{
				delete w;
				m_widgets.erase(m_widgets.begin() + i);
				return;
			}
		}
	}

	GuiImageManager* GuiFactory::getImageManager() const
	{
		return m_imageManager;
	}

	GuiFontManager* GuiFactory::getFontManager() const
	{
		return m_fontManager;
	}
}
