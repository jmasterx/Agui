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

#include <Agui/Agui.hpp>
#include <Agui/Backends/Allegro5/Allegro5.hpp>
#include <Agui/Widgets/Button/Button.hpp>
#include <Agui/Widgets/TextField/TextField.hpp>
#include <Agui/Widgets/Frame/Frame.hpp>
#include <Agui/FlowLayout.hpp>
#include <stdlib.h>
#include <vector> 
#include <ctime>
#include <iostream>
#include "GuiImageManager.hpp"
#include "GuiFontManager.hpp"
#include "Calculator.hpp"
#include <Agui/Backends/Allegro5/Allegro5CursorProvider.hpp>
#include "CalculatorManager.hpp"
#include <ctime>

using namespace cge;

#define FRAME_RATE 60

	//Globals
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT event;
	ALLEGRO_EVENT_QUEUE *queue = NULL;
	bool done = false;
	agui::Gui *gui = NULL;
	agui::Allegro5Input* inputHandler = NULL;
	agui::Allegro5Graphics* graphicsHandler = NULL;
	GuiFontManager* fontMan = NULL;
	GuiImageManager* imgMan = NULL;
	GuiFactory m_globalFactory;
	agui::Allegro5CursorProvider cursor;

	class CalculatorCallback : public agui::ActionListener
	{
		CalculatorManager m_calcMan;
		Button* m_btnCreate;
		GuiFactory m_factory;
		FlowLayout* m_flow;
	public:
		CalculatorCallback()
		{
			m_factory.setFontManager(fontMan);
			m_factory.setImageManager(imgMan);
			m_flow = m_factory.createFlowLayout();
			m_btnCreate = m_factory.createWhiteButton();
			m_btnCreate->setText("New Calculator");
			m_btnCreate->resizeToContents();
			m_btnCreate->addActionListener(this);
			m_flow->add(m_btnCreate);
			m_flow->setLeftToRight(false);
			gui->add(m_flow);

			srand(time(NULL));
		}

		virtual void actionPerformed( const agui::ActionEvent &evt ) 
		{
			m_calcMan.createCalc(fontMan,imgMan,gui);
		}

		void logic()
		{
			m_calcMan.destroyDeadCalcs();
		}

	};

	CalculatorCallback* calcCallback = NULL;

	bool initializeAllegro() {
		//Initialize Allegro
		if(!al_init())
		{
			return false;
		}

		if(!al_init_image_addon())
		{
			return false;
		}

		al_init_font_addon();

		if(!al_init_ttf_addon())
		{
			return false;
		}

		if(!al_init_primitives_addon())
		{
			return false;
		}
		if(!al_install_mouse())
		{
			return false;
		}
		if(!al_install_keyboard())
		{
			return false;
		}

		// Start a timer to regulate speed
		timer = al_create_timer(1.0/FRAME_RATE);
		al_start_timer(timer);

		//show screen

		al_set_new_display_flags(ALLEGRO_RESIZABLE);

		display = al_create_display(640,480);

		if(!display)
		{

			return false;
		}
		//show the mouse
		al_show_mouse_cursor(display); 

		al_set_new_bitmap_flags(
			ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | 
			ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);

		//Window Title
		al_set_window_title(display,"Agui - Calculators Demo");

		queue = al_create_event_queue();

		return true;
	}

	void initializeAgui()
	{
		//Set the image loader
		agui::Image::setImageLoader(new agui::Allegro5ImageLoader);

		//Set the font loader
		agui::Font::setFontLoader(new agui::Allegro5FontLoader);

		//Instance the input handler
		inputHandler = new agui::Allegro5Input();

		//Instance the graphics handler
		graphicsHandler = new agui::Allegro5Graphics();

		//Allegro does not automatically premultiply alpha so let Agui do it
		agui::Color::setPremultiplyAlpha(true);

		//Instance the gui
		gui = new agui::Gui();

		//Set the input handler
		gui->setInput(inputHandler);

		//Set the graphics handler
		gui->setGraphics(graphicsHandler);

		gui->setCursorProvider(&cursor);

		fontMan = new GuiFontManager("data/DejaVuSans.ttf",6,40,20);

		//Setting a global font is required and failure to do so will crash.
		agui::Widget::setGlobalFont(fontMan->getDefaultFont());

		imgMan = new GuiImageManager("data/gui_image_mapping.conf","data/");

		m_globalFactory.setImageManager(imgMan);
		m_globalFactory.setFontManager(fontMan);
		//set the global ToolTip
		gui->setToolTip(m_globalFactory.createToolTip());

		calcCallback = new CalculatorCallback();
	}

	void cleanUp()
	{
		gui->getTop()->clear();
		delete gui;
		gui = NULL;
		delete inputHandler;
		delete graphicsHandler;
		delete fontMan;
		delete imgMan;
		delete calcCallback;
		fontMan = NULL;
		imgMan = NULL;
		inputHandler = NULL;
		graphicsHandler = NULL;
	}
	void render()
	{
		al_clear_to_color(al_map_rgb(240,240,240));

		//render the widgets
		gui->render();

		al_flip_display();

	}

int main(int argc, char *argv[])
{
	if (!initializeAllegro())
	{
		return 1;
	}

	initializeAgui();
	bool needRedraw = true;
	// Start the event queue to handle keyboard input, mouse and our timer
	al_register_event_source(queue, (ALLEGRO_EVENT_SOURCE*)al_get_keyboard_event_source());
	al_register_event_source(queue, (ALLEGRO_EVENT_SOURCE*)al_get_mouse_event_source());
	al_register_event_source(queue, (ALLEGRO_EVENT_SOURCE*)timer);
	al_register_event_source(queue, (ALLEGRO_EVENT_SOURCE*)display);

	while(!done) {

		// Block until an event enters the queue
		al_wait_for_event(queue, &event);

		//Let Agui process the event
		inputHandler->processEvent(event);

		//Handle rendering and logic
		if (needRedraw && al_event_queue_is_empty(queue)) {
			gui->logic();
			calcCallback->logic();
			render();

			needRedraw = false;
		}


		switch(event.type) {

		case ALLEGRO_EVENT_TIMER:
			if(event.timer.source == timer)
			{

				needRedraw = true;
			}

			break;
		case ALLEGRO_EVENT_DISPLAY_RESIZE:

			al_acknowledge_resize(event.display.source);

			//Resize Agui
			gui->resizeToDisplay();

			break;
		case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
			//Resize Agui
			gui->resizeToDisplay();
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			return 0;
			break;
		}
	}

	cleanUp();

	return 0;
}
