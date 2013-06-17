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

#ifndef CGE_GUI_FACTORY_HPP
#define CGE_GUI_FACTORY_HPP
#include <vector>
#include <map>
#include "Agui/Agui.hpp"
#include "Agui/Widget.hpp"
#include "Button.hpp"
#include "FlowLayout.hpp"
#include "BorderLayout.hpp"
#include "Frame.hpp"
#include "PopUpMenu.hpp"
#include "TextField.hpp"
#include "ToolTip.hpp"
#include "Agui/Widgets/Label/Label.hpp"
#include "GridLayout.hpp"
#include "GuiFontManager.hpp"
#include "GuiImageManager.hpp"

namespace cge
{
	class GuiFactory
	{
		std::vector<agui::Widget*> m_widgets;
		GuiImageManager* m_imageManager;
		GuiFontManager* m_fontManager;
		void _loadImages();
	public:
		GuiFactory(void);
		void deleteWidget(agui::Widget* w);
		void setImageManager(GuiImageManager* manager);
		void setFontManager(GuiFontManager* manager);
		ToolTip* createToolTip();
		Button* createButton();
		TextField* createTextField();
		PopUpMenu* createPopUpMenu();
		Frame* createFrame(agui::Widget* content);
		BorderLayout* createBorderLayout();
		GridLayout* createGridLayout();
		FlowLayout* createFlowLayout();
		agui::EmptyWidget* createEmptyWidget();
		agui::Label* createLabel();
		Button* createWhiteButton(const std::string& icon = "", const std::string& textIcon = "");
		GuiImageManager* getImageManager() const;
		GuiFontManager* getFontManager() const;
		virtual ~GuiFactory(void);
	};
}
#endif