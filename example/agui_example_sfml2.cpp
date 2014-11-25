#include <SFML/Graphics.hpp>

#include <Agui/Agui.hpp>
#include <Agui/Backends/SFML2/SFML2.hpp>
#include <Agui/Widgets/Button/Button.hpp>
#include <Agui/Widgets/CheckBox/CheckBox.hpp>
#include <Agui/Widgets/DropDown/DropDown.hpp>
#include <Agui/Widgets/TextField/TextField.hpp>
#include <Agui/Widgets/Frame/Frame.hpp>
#include <Agui/Widgets/RadioButton/RadioButton.hpp>
#include <Agui/Widgets/RadioButton/RadioButtonGroup.hpp>
#include <Agui/Widgets/Slider/Slider.hpp>
#include <Agui/Widgets/TextBox/ExtendedTextBox.hpp>
#include <Agui/Widgets/Tab/TabbedPane.hpp>
#include <Agui/Widgets/ListBox/ListBox.hpp>
#include <Agui/Widgets/ScrollPane/ScrollPane.hpp>
#include <Agui/FlowLayout.hpp>
#include <iostream>

agui::Gui *gui = NULL;
agui::SFML2Input* inputHandler = NULL;
agui::SFML2Graphics* graphicsHandler = NULL;
agui::Font *defaultFont = NULL;


class SimpleActionListener : public agui::ActionListener
{
public:
	virtual void actionPerformed(const agui::ActionEvent &evt)
	{
		agui::Slider* slider = dynamic_cast<agui::Slider*>(evt.getSource());
		if(slider)
		{
			slider->setBackColor(agui::Color(slider->getValue(),slider->getValue(),slider->getValue()));
			return;

		}

		std::cout << "This is an SFML MessageBox" << std::endl;
	}
};

class WidgetCreator {
private:
	SimpleActionListener simpleAL;
	agui::FlowLayout flow;
	agui::Button button;
	agui::CheckBox checkBox;
	agui::DropDown dropDown;
	agui::TextField textField;
	agui::Frame frame;
	agui::Gui* mGui;
	agui::RadioButton rButton[3];
	agui::RadioButtonGroup rGroup;
	agui::Slider slider;
	agui::ExtendedTextBox exTextBox;
	agui::TabbedPane tabbedPane;
	agui::Tab tab[3];
	agui::ListBox listBox;
	agui::ScrollPane scrollPane;
	agui::Button scrollButtons[15];

public:
	WidgetCreator(agui::Gui *guiInstance)
	{
		mGui = guiInstance;
		mGui->add(&flow);
		flow.add(&button);
		button.setSize(80,40);
		button.setText("Push Me");
		button.addActionListener(&simpleAL);

		flow.add(&checkBox);
		checkBox.setAutosizing(true);
		checkBox.setText("Show me a message box");
		checkBox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
		checkBox.addActionListener(&simpleAL);

		flow.add(&dropDown);
		dropDown.setText("Select Item");
		dropDown.setSize(120,30);
		dropDown.addItem("Apples");
		dropDown.addItem("Oranges");
		dropDown.addItem("Grapes");
		dropDown.addItem("Peaches");
		dropDown.addActionListener(&simpleAL);

		flow.add(&textField);
		textField.setText("Press Enter!");
		textField.resizeToContents();
		textField.addActionListener(&simpleAL);

		gui->add(&frame);
		frame.setSize(220,120);
		frame.setLocation(60,60);
		frame.setText("Example Frame");
		std::stringstream ss;
		for(int i = 0; i < 3; ++i)
		{
			ss.str("");
			ss.clear();
			ss << "Sample Radio Button ";
			ss << i;

			rGroup.add(&rButton[i]);
			rButton[i].setAutosizing(true);
			rButton[i].setText(ss.str());
			frame.add(&rButton[i]);
			rButton[i].setLocation(0,30 * i);
			rButton[i].addActionListener(&simpleAL);
		}

		flow.add(&slider);
		slider.setSize(100,36);
		slider.setMaxValue(255);
		slider.setMarkerSize(agui::Dimension(10,30));
		slider.addActionListener(&simpleAL);

		exTextBox.setWordWrap(true);
		exTextBox.setText("Welcome \n");
		exTextBox.setCurrentColor(agui::Color(255,0,0));
		exTextBox.appendText("WARNING!!!\n\n");
		exTextBox.setCurrentColor(agui::Color(255,0,255));
		exTextBox.appendText("Viewing text in magenta is not recommended\n\n");
		exTextBox.setCurrentColor(agui::Color(100,0,200));
	exTextBox.appendText("Copyright (c) 2011 Joshua Larouche \n \
\
License: (BSD) \n \
Redistribution and use in source and binary forms, with or without \
modification, are permitted provided that the following conditions \
are met: \n \
1. Redistributions of source code must retain the above copyright \
notice, this list of conditions and the following disclaimer. \n \
2. Redistributions in binary form must reproduce the above copyright \
notice, this list of conditions and the following disclaimer in \
the documentation and/or other materials provided with the distribution. \n \
3. Neither the name of Agui nor the names of its contributors may \
be used to endorse or promote products derived from this software \
without specific prior written permission. \n\n \
");

	exTextBox.setCurrentColor(agui::Color(255,128,0));
exTextBox.appendText("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \
\"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT \
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR \
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT \
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, \
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED \
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR \
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF \
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING \
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS \
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. \
");
		exTextBox.setSelection(84,100);
		exTextBox.setSelectionColor(agui::Color(255,0,0));
		exTextBox.setSelection(0,0);
		//exTextBox.setTextAlignment(agui::ALIGN_CENTER);
		exTextBox.setSize(600,300);
		exTextBox.positionCaret(0,0);

		tabbedPane.setSize(agui::Dimension(300,400));
		flow.add(&tabbedPane);
		tab[0].setText("Text Box");
		tab[1].setText("List Box");
		tab[2].setText("Scroll Pane");
		tabbedPane.addTab(&tab[0],&exTextBox);
		tabbedPane.addTab(&tab[1],&listBox);
		tabbedPane.addTab(&tab[2],&scrollPane);

		tabbedPane.setResizeTabContent(true);

		for(int i = 0; i < 250; ++i)
		{
			ss.str("");
			ss.clear();
			ss << "Item ";
			ss << i;

			listBox.addItem(ss.str());
		}

		listBox.setMultiselectExtended(true);

		int count = 0;
		for(int i = 0; i < 3; ++i)
		{
			for(int j = 0; j < 5; ++j)
			{

				ss.str("");
				ss.clear();
				ss << "Scroll Button ";
				ss << i + 1;
				ss << " , ";
				ss << j + 1;

				scrollPane.add(&scrollButtons[count]);
				scrollButtons[count].setSize(200,100);
				scrollButtons[count].setLocation(210 * i,110 * j);
				scrollButtons[count].setText(ss.str());

				count++;
			}
		}
	}
};

WidgetCreator* creator;

void initializeAgui(sf::RenderTarget& target)
{

	//Set the image loader
	agui::Image::setImageLoader(new agui::SFML2ImageLoader());

	//Set the font loader
	agui::Font::setFontLoader(new agui::SFML2FontLoader());

	//Instance the input handler
	inputHandler = new agui::SFML2Input();

	//Instance the graphics handler
	graphicsHandler = new agui::SFML2Graphics(target);

	agui::Color::setPremultiplyAlpha(false);

	//Instance the gui
	gui = new agui::Gui();

	//Set the input handler
	gui->setInput(inputHandler);

	//Set the graphics handler
	gui->setGraphics(graphicsHandler);

	defaultFont = agui::Font::load("data/DejaVuSans.ttf",16);

	//Setting a global font is required and failure to do so will crash.
	agui::Widget::setGlobalFont(defaultFont);
}

void addWidgets()
{
	creator = new WidgetCreator(gui);
}

void cleanUp()
{
	gui->getTop()->clear();
	delete creator;
	creator = NULL;
	delete gui;
	gui = NULL;
	delete inputHandler;
	delete graphicsHandler;
	inputHandler = NULL;
	graphicsHandler = NULL;

	delete defaultFont;
	defaultFont = NULL;
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

	initializeAgui(window);
	addWidgets();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				//window.setView(sf::View(sf::FloatRect(0, 0, event.size.width,event.size.height)));

				gui->resizeToDisplay();
			}

			inputHandler->processEvent(event);
		}

		gui->logic();
		window.clear(sf::Color(240,240,240));
		gui->render();
		window.display();
	}

	return 0;
}