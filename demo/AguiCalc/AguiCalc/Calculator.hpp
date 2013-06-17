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

#ifndef CGE_CALC_HPP
#define CGE_CALC_HPP
#include "GuiFactory.hpp"
#include "Agui/Gui.hpp"
#include <Agui/ActionListener.hpp>
#define  NUM_BTN_SIZE 10
namespace cge
{
	class Calculator : public agui::ActionListener
	{
		GuiFactory m_factory;
		agui::Gui* m_gui;

		enum Operator
		{
			OP_NONE = 0,
			OP_ADD = 1,
			OP_SUB = 2,
			OP_MUL = 3,
			OP_DIV = 4
		};

		enum State
		{
			STATE_ACC1,
			STATE_OP,
			STATE_ACC2,
			STATE_SILLY_USER
		};

		Button* m_numBtn[NUM_BTN_SIZE];
		Button* m_fullStop;
		Button* m_equals;
		Button* m_clear;
		Button* m_back;
		Button* m_sqrt;
		Button* m_add;
		Button* m_subtract;
		Button* m_multiply;
		Button* m_divide;
		Button* m_plusMinus;

		Button* m_close;

		Frame* m_frame;
		BorderLayout* m_border;
		GridLayout* m_grid;
		TextField* m_edit;

		Operator m_op;
		State m_state;
		double m_acc;
		double m_acc2;
		std::string m_accStr;
	
		double getAccVal();
		void doFullStopCheck();
		bool hasFullStop() const;
		void appendAccumulator(const std::string& val);
		void doEmptyCheck();
		void doBack();
		void doClear();
		void doSqrt();
		void doPlusMinus();
		void doEquals();

		void doOpChange(Operator op);

		double doMaths(Operator op);

		void displayResult();
	public:
		agui::Gui& getGui();
		Calculator(GuiImageManager* img, GuiFontManager* font, agui::Gui* gui);
		void makeUI();
		Button* makeButton(const std::string& text = "",bool white = false);
		~Calculator(void);

		Button* getCloseButton();

		void setLocation(int x, int y);

		virtual void actionPerformed( const agui::ActionEvent &evt );

	};
}
#endif
