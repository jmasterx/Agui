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

#include "CalculatorManager.hpp"
#include <stdlib.h>
#include <vector>
namespace cge
{
	CalculatorManager::CalculatorManager(void)
	{
	}

	CalculatorManager::~CalculatorManager(void)
	{
		//clean up
		for(std::vector<Calculator*>::iterator it = m_calcs.begin(); it != m_calcs.end(); ++it)
		{
				delete (*it);
		}
	}

	void CalculatorManager::actionPerformed( const agui::ActionEvent &evt )
	{
		m_deadCalcs.push(evt.getSource());
	}

	void CalculatorManager::createCalc( GuiFontManager* font, GuiImageManager* img, agui::Gui* gui )
	{
		m_calcs.push_back(new Calculator(img,font,gui));
		//hook into its close button
		m_calcs.back()->getCloseButton()->addActionListener(this);
		//set a very lazy "random" location
		m_calcs.back()->setLocation(rand() % gui->getTop()->getWidth(),rand() % gui->getTop()->getHeight());
	}

	void CalculatorManager::destroyDeadCalcs()
	{
		//ALL YOUR CALCULATORS ARE BELONG TO US
		while(!m_deadCalcs.empty())
		{
			for(std::vector<Calculator*>::iterator it = m_calcs.begin(); it != m_calcs.end(); ++it)
			{
				if((*it)->getCloseButton() == m_deadCalcs.front())
				{
					delete (*it);
					m_calcs.erase(it);

					break;
				}
			}

			m_deadCalcs.pop();
		}
	}

}
