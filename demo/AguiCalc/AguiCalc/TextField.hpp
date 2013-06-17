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

#ifndef CGE_TEXTFIELD_HPP
#define CGE_TEXTFIELD_HPP
#include "Agui/Widgets/TextField/TextField.hpp"
#include "PopUpMenu.hpp"
#include "Agui/ActionListener.hpp"
namespace cge
{
	class TextField : public agui::TextField, agui::ActionListener
	{
		agui::Image* m_bgImage;
		agui::Image* m_focusImage;
		PopUpMenu* m_menu;
		agui::PopUpMenuItem m_cutItem;
		agui::PopUpMenuItem m_copyItem;
		agui::PopUpMenuItem m_pasteItem;
		agui::PopUpMenuItem m_deleteItem;
		agui::PopUpMenuItem m_selectAllItem;
		agui::PopUpMenuItem m_separatorItem;
	public:
		TextField(	agui::Image* bgImage,
		agui::Image* focusImage,
		PopUpMenu* menu);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void mouseDown(agui::MouseEvent &mouseEvent);
		virtual ~TextField(void);
	};
}

#endif
