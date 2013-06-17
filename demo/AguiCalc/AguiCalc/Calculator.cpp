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

#include "Calculator.hpp"
#include <sstream>
#define MAX_DECIMALS 12
namespace cge
{
	Calculator::~Calculator(void)
	{
	}

	Calculator::Calculator( GuiImageManager* img, GuiFontManager* font, agui::Gui* gui )
		: m_gui(gui),m_acc(0),m_acc2(0),m_op(OP_NONE),m_state(STATE_ACC1)
	{
		m_factory.setImageManager(img);
		m_factory.setFontManager(font);
		makeUI();
		doClear();
		displayResult();
	}

	agui::Gui& Calculator::getGui()
	{
		return *m_gui;
	}

	void Calculator::makeUI()
	{
		std::stringstream ss;

		m_edit = m_factory.createTextField();
		m_edit->setReadOnly(true);
		m_edit->setTextAlignment(agui::ALIGN_RIGHT);
		m_edit->setFont(m_factory.getFontManager()->getFont(0.95f));

		m_border = m_factory.createBorderLayout();
		m_border->setResizable(false);
	
		float relativeFontRatio = m_factory.getFontManager()->getDefaultFont()->getLineHeight() / 19.0f;
		m_frame = m_factory.createFrame(m_border);
		m_frame->setSize(260 * relativeFontRatio,325 * relativeFontRatio);
		m_frame->setMinSize(agui::Dimension(260 * relativeFontRatio,325 * relativeFontRatio));
		m_frame->setMaxSize(agui::Dimension(260 * 1.25f * relativeFontRatio,325 * 1.25f * relativeFontRatio));

		m_grid = m_factory.createGridLayout();
	    m_grid->setMargins(2,8,4,4);

	    m_grid->setNumberOfColumns(4);
		m_grid->setNumberOfRows(0);

		for(int i = 0; i < NUM_BTN_SIZE; ++i)
		{
			ss.str("");
			ss.clear();
			ss << i;
			m_numBtn[i] = makeButton(ss.str());
		}

		m_fullStop = makeButton(".");
		m_equals = makeButton("=",true);
		m_clear = makeButton("C",true);
		std::string backuni = "";
		//back symbol
		backuni += (char)0xE2; backuni +=  + (char)0x86; backuni += (char)0x90;
		m_back = makeButton(backuni,true);
		std::string sqrtuni = "";
		//sqrt symbol
		sqrtuni += (char)0xE2l; sqrtuni +=  + (char)0x88; sqrtuni += (char)0x9A;
		m_sqrt = makeButton(sqrtuni,true);
	    m_add = makeButton("+",true);
		m_subtract = makeButton("-",true);
		m_multiply = makeButton("*",true);
		m_divide = makeButton("/",true);
		std::string pmuni = "";
		//sqrt symbol
		pmuni += (char)0xC2; pmuni +=  + (char)0xB1;
	    m_plusMinus = makeButton(pmuni,true);

		m_grid->add(m_back);
		m_grid->add(m_clear);
		m_grid->add(m_plusMinus);
		m_grid->add(m_sqrt);

		m_grid->add(m_numBtn[7]);
		m_grid->add(m_numBtn[8]);
		m_grid->add(m_numBtn[9]);
		m_grid->add(m_divide);

		m_grid->add(m_numBtn[4]);
		m_grid->add(m_numBtn[5]);
		m_grid->add(m_numBtn[6]);
		m_grid->add(m_multiply);

		m_grid->add(m_numBtn[1]);
		m_grid->add(m_numBtn[2]);
		m_grid->add(m_numBtn[3]);
		m_grid->add(m_subtract);

		m_grid->add(m_numBtn[0]);
		m_grid->add(m_fullStop);
		m_grid->add(m_equals);
		m_grid->add(m_add);

		m_border->add(m_edit,agui::BorderLayout::NORTH);
		m_border->add(m_grid,agui::BorderLayout::CENTER);
		getGui().add(m_frame);

		m_close = makeButton("x",false);
		m_close->setFont(m_factory.getFontManager()->getFont(0.75f));
		m_close->resizeToContents();
		m_close->setSize(m_close->getWidth(),m_edit->getInnerHeight());
		m_edit->add(m_close);
	}

	void Calculator::actionPerformed( const agui::ActionEvent &evt )
	{
		for(int i = 0; i < NUM_BTN_SIZE; ++i)
		{
			if(m_numBtn[i] == evt.getSource())
			{
				appendAccumulator(m_numBtn[i]->getText());
			}
		}

		if(m_fullStop == evt.getSource())
		{
			appendAccumulator(".");
		}

		if(m_back == evt.getSource())
		{
			doBack();
		}

		if(m_clear == evt.getSource())
		{
			doClear();
		}
		if(m_sqrt == evt.getSource())
		{
		   doSqrt();
		}

		if(m_plusMinus == evt.getSource())
		{
			doPlusMinus();
		}

		if(m_add == evt.getSource())
		{
			doOpChange(OP_ADD);
		}

		if(m_subtract == evt.getSource())
		{
			doOpChange(OP_SUB);
		}

		if(m_multiply == evt.getSource())
		{
			doOpChange(OP_MUL);
		}

		if(m_divide == evt.getSource())
		{
			doOpChange(OP_DIV);
		}

		if(m_equals == evt.getSource())
		{
			doEquals();
		}

		doFullStopCheck();
		displayResult();
	}

	Button* Calculator::makeButton( const std::string& text /*= ""*/, bool white)
	{
		Button* b = white ? m_factory.createWhiteButton() : m_factory.createButton();
		b->setFont(m_factory.getFontManager()->getFont(1.1f));
		b->setText(text);
		b->addActionListener(this);

		return b;
	}

	void Calculator::doFullStopCheck()
	{
		m_fullStop->setEnabled(!hasFullStop());
	}

	void Calculator::appendAccumulator( const std::string& val )
	{
		if(m_state == STATE_SILLY_USER || m_accStr.length() == MAX_DECIMALS)
		{
			return;
		}

		if(m_state == STATE_OP)
		{
			m_state = STATE_ACC2;
		}

		if(hasFullStop() && val == ".")
		{
			doFullStopCheck();
		}
		else
		{
			if(m_accStr == "0")
			{
				m_accStr = "";
			}

			if((m_accStr.length() == 0 || 
				(m_accStr.length() == 1 && m_accStr[0] == '-')) && val == ".")
			{
			   m_accStr += "0";
			}

			m_accStr += val;
		}
	}

	bool Calculator::hasFullStop() const
	{
		return m_accStr.find('.') != std::string::npos;
	}

	void Calculator::displayResult()
	{
		std::string result = "";
		std::stringstream ss;
		ss.precision(MAX_DECIMALS);
		switch(m_state)
		{
		case STATE_SILLY_USER:
			result = "Inf";
			break;
		case STATE_ACC1:
		case STATE_ACC2:
			result = m_accStr;
			break;
		case STATE_OP:
			ss << m_acc;
			result = ss.str();
		}

		m_edit->setText(result);
	}

	void Calculator::doEmptyCheck()
	{
		if(m_accStr.length() == 0 || m_accStr == "-")
		{
			m_accStr = "0";
		}
	}

	void Calculator::doBack()
	{
		if(m_state == STATE_ACC1 || m_state == STATE_ACC2)
		{
			m_accStr =  m_accStr.substr(0, m_accStr.size()-1);
			doEmptyCheck();
		}
	}

	void Calculator::doClear()
	{
		m_state = STATE_ACC1;
		m_op = OP_NONE;
		m_acc = 0;
		m_acc2 = 0;
		m_accStr = "";
		doEmptyCheck();
		doFullStopCheck();
	}

	double Calculator::getAccVal()
	{
		std::stringstream ss;
		ss.precision(MAX_DECIMALS);
		ss << m_accStr;
		double d;
		ss >> d;
		return d;
	}

	void Calculator::doSqrt()
	{
		double d = 0.0;

		if(m_state == STATE_OP)
		{
			d = m_acc;
		}
		else
		{
			d = getAccVal();
		}

		if(d > 0)
			d = sqrt(d);
		else
		{
			return;
		}

		if (m_state == STATE_OP)
		{
			m_acc2 = d;
			m_state = STATE_ACC2;


			std::stringstream ss;
			ss.precision(MAX_DECIMALS);
			ss << d;
			m_accStr = ss.str();
		}
		else 
		{
     		std::stringstream ss;
			ss.precision(MAX_DECIMALS);
			ss << d;
			m_accStr = ss.str();
		}
	
	}

	void Calculator::doPlusMinus()
	{
		double d = 0.0;

		if(m_state == STATE_OP)
		{
			d = m_acc;
		}
		else
		{
			d = getAccVal();
		}

		if(abs(d) != 0)
			d = -d;
		else
		{
			return;
		}

		if (m_state == STATE_OP)
		{
			m_acc2 = d;
			m_state = STATE_ACC2;


			std::stringstream ss;
			ss.precision(MAX_DECIMALS);
			ss << d;
			m_accStr = ss.str();
		}
		else 
		{
			std::stringstream ss;
			ss.precision(MAX_DECIMALS);
			ss << d;
			m_accStr = ss.str();
		}
	
	}

	void Calculator::doOpChange( Operator op )
	{
		if(m_state == STATE_SILLY_USER)
			return;

		if(m_state != STATE_OP)
		{
			if(m_state == STATE_ACC1)
			{
				m_acc = getAccVal();
				m_acc2 = m_acc;
				m_accStr = "";
			}
			else if(m_state == STATE_ACC2)
			{
				m_acc2 = getAccVal();
				m_acc = doMaths(m_op);
				if(m_acc2 == 0 && m_op == OP_DIV)
				{
					m_state = STATE_SILLY_USER;
					return;
				}
				m_acc2 = m_acc;
				m_accStr = "";
			}

		    m_state = STATE_OP;
		}
		else
		{
			m_acc2 = m_acc;
		}

		m_op = op;
	}

	double Calculator::doMaths( Operator op )
	{
		switch(op)
		{
		case OP_DIV:
			if(m_acc2 == 0)
			{
				return 0;
			}
			return m_acc / m_acc2;
		case OP_MUL:
			return m_acc * m_acc2;
		case OP_SUB:
			return m_acc - m_acc2;
		case OP_ADD:
			return m_acc + m_acc2;
		}

		return 0;
	}

	void Calculator::doEquals()
	{
		if(m_op == OP_NONE)
			return;

		if(m_state == STATE_ACC2)
		m_acc2 = getAccVal();

		m_acc = doMaths(m_op);
		if(m_acc2 == 0 && m_op == OP_DIV)
		{
			m_state = STATE_SILLY_USER;
			return;
		}

		m_accStr = "";

		m_state = STATE_OP;
	}

	Button* Calculator::getCloseButton()
	{
		return m_close;
	}

	void Calculator::setLocation( int x, int y )
	{
		m_frame->setLocation(x,y);
	}

}


