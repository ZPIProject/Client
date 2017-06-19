#include "GuiHandler.h"
#include "ConstantValues.h"

GuiHandler::~GuiHandler()
{
	delete gui;
	delete gm;
	//delete window;
}

GuiHandler::GuiHandler(sf::RenderWindow & win)
{
	window = &win;
	gui = new tgui::Gui(*window);
	network_handler = new NetworkHandler(PORT);

	active_gui = CurrentActiveGUI::LOGIN;
	login();
	did_active_gui_changed = false;
	gm = new GameManager(network_handler, *window);
}

void GuiHandler::handle_event(sf::Event & event)
{
	gui->handleEvent(event);
}

void GuiHandler::character_selection()
{
	std::vector<std::string> character_list;
	sf::Packet character_list_request;
	int current_character = 0;
	int character_count;


	character_list_request << 13 << current_logged_account;
	network_handler->send_packet(character_list_request);

	sf::Packet recived_list = network_handler->recive_packet();
	recived_list >> character_count;
	for (int i = 0; i < character_count; i++)
	{
		std::string character_name;
		recived_list >> character_name;
		character_list.push_back(character_name);
	}


	tgui::Button::Ptr button0 = tgui::Button::create("Enter the world of MAGIC");
	tgui::Button::Ptr button1 = tgui::Button::create("Logout");
	tgui::Button::Ptr button2 = tgui::Button::create("<");
	tgui::Button::Ptr button3 = tgui::Button::create(">");
	tgui::Picture::Ptr characterPicture = tgui::Picture::create("Graphics/tempChar.png");
	tgui::Label::Ptr label = tgui::Label::create();
	label->setPosition(340, 180);
	label->setTextColor("white");
	gui->add(label, "name_label");
	label->setText("Abrakadabra");


	characterPicture->setSize(160, 160);
	characterPicture->setPosition(320, 220);
	gui->add(characterPicture);

	button0->setSize(200, 50);
	button0->setPosition(300, 500);
	button0->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::MAINMENU);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button0);

	button1->setSize(50, 50);
	button1->setPosition(730, 20);
	button1->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::LOGIN);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button1);

	button2->setSize(30, 30);
	button2->setPosition(70, 300);
	button2->connect("pressed", [&]() {
		std::cout << "-->leftPressed() " << std::endl;
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::MAINMENU);
		/*current_character--;
		current_character < 0 ? character_count - 1 : current_character;
	
		std::cout << current_character << "\n";*/
//		label->setText("Alakazam");
		//this->set_active_gui_changed(true);
	});
	gui->add(button2);

	button3->setSize(30, 30);
	button3->setPosition(700, 300);
	button3->connect("pressed", [&current = current_character, count = character_count]() {
		std::cout << "-->rightPressed() " << std::endl;
		current++;
		current >= count ? current = 0 : current;

		std::cout << current << "\n";
		//this->set_active_gui_changed(true);
	});
	gui->add(button3);
}

void GuiHandler::main_menu()
{
	std::cout << "main_menu\n";
	tgui::Button::Ptr button0 = tgui::Button::create("Play");
	tgui::Button::Ptr button1 = tgui::Button::create("Stats");
	tgui::Button::Ptr button2 = tgui::Button::create("How to play?");
	tgui::Button::Ptr button3 = tgui::Button::create("Character Screen");

	tgui::Button::Ptr button4 = tgui::Button::create("Logout");
	tgui::Button::Ptr button5 = tgui::Button::create("Exit");

	button0->setSize(300, 40);
	button0->setPosition(250, 190);
	button0->connect("pressed", [=]() {
		std::cout << "-->playPressed() " << std::endl;
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::GAME);
	});
	gui->add(button0);

	button1->setSize(300, 40);
	button1->setPosition(250, 250);
	button1->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::PLAYERSTATISTIC);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button1);

	button2->setSize(300, 40);
	button2->setPosition(250, 310);
	button2->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::TUTORIAL);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button2);

	button3->setSize(300, 40);
	button3->setPosition(250, 370);
	button3->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::CHARACTERSELECTION);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button3);


	button4->setSize(300, 40);
	button4->setPosition(250, 470);
	button4->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::LOGIN);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button4);

	button5->setSize(300, 40);
	button5->setPosition(250, 530);
	button5->connect("pressed", [&]() { window->close(); });
	gui->add(button5);
}

void GuiHandler::statistics()
{
	int hpValue_base = 303;
	int mpValue_base = 123;

	int knoValue_base = 5;
	int wisValue_base = 5;
	int vitValue_base = 5;

	int apValue_base = 3;

	tgui::Button::Ptr button0 = tgui::Button::create("Save");
	tgui::Button::Ptr button1 = tgui::Button::create("Reset");
	tgui::Button::Ptr button2 = tgui::Button::create("Back");
	tgui::Label::Ptr label0 = tgui::Label::create("Player's stats:");

	tgui::Label::Ptr label1 = tgui::Label::create("HP:");
	tgui::Label::Ptr label11 = tgui::Label::create(std::to_string(hpValue_base));
	tgui::Label::Ptr label2 = tgui::Label::create("MP:");
	tgui::Label::Ptr label21 = tgui::Label::create(std::to_string(mpValue_base));

	tgui::Label::Ptr label3 = tgui::Label::create("Player's atributes:");
	tgui::Label::Ptr label4 = tgui::Label::create("Knowledge:");
	tgui::Label::Ptr label41 = tgui::Label::create(std::to_string(knoValue_base));
	tgui::Label::Ptr label5 = tgui::Label::create("Wisdom:");
	tgui::Label::Ptr label51 = tgui::Label::create(std::to_string(wisValue_base));
	tgui::Label::Ptr label6 = tgui::Label::create("Vitality:");
	tgui::Label::Ptr label61 = tgui::Label::create(std::to_string(vitValue_base));

	tgui::Label::Ptr label7 = tgui::Label::create("Points to spend:");
	tgui::Label::Ptr label71 = tgui::Label::create(std::to_string(apValue_base));

	button0->setSize(80, 30);
	button0->setPosition(20, 300);
	button0->connect("pressed", []() {
		std::cout << "-->savePressed() " << std::endl;
	});
	gui->add(button0);

	button1->setSize(80, 30);
	button1->setPosition(120, 300);
	button1->connect("pressed", []() {
		std::cout << "-->resetPressed() " << std::endl;
	});
	gui->add(button1);

	button2->setSize(80, 30);
	button2->setPosition(20, 550);
	button2->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::MAINMENU);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button2);


	label0->setPosition(20, 20);
	label0->setTextColor("white");
	gui->add(label0);

	label1->setPosition(20, 50);
	label1->setTextColor("white");
	gui->add(label1);
	label11->setPosition(170, 50);
	label11->setTextColor("white");
	gui->add(label11);


	label2->setPosition(20, 80);
	label2->setTextColor("white");
	gui->add(label2);
	label21->setPosition(170, 80);
	label21->setTextColor("white");
	gui->add(label21);

	label3->setPosition(20, 130);
	label3->setTextColor("white");
	gui->add(label3);

	label6->setPosition(20, 160);
	label6->setTextColor("white");
	gui->add(label6);
	label61->setPosition(170, 160);
	label61->setTextColor("white");
	gui->add(label61);

	label4->setPosition(20, 190);
	label4->setTextColor("white");
	gui->add(label4);
	label41->setPosition(170, 190);
	label41->setTextColor("white");
	gui->add(label41);

	label5->setPosition(20, 220);
	label5->setTextColor("white");
	gui->add(label5);
	label51->setPosition(170, 220);
	label51->setTextColor("white");
	gui->add(label51);



	label7->setPosition(20, 270);
	label7->setTextColor("white");
	gui->add(label7);
	label71->setPosition(170, 270);
	label71->setTextColor("white");
	gui->add(label71);
}

void GuiHandler::login()
{
	tgui::Button::Ptr button0 = tgui::Button::create("login");
	tgui::Button::Ptr button1 = tgui::Button::create("exit");
	tgui::EditBox::Ptr editbox0 = tgui::EditBox::create();
	tgui::EditBox::Ptr editbox1 = tgui::EditBox::create();
	tgui::EditBox::Ptr editbox2 = tgui::EditBox::create();
	
	editbox0->setSize(200, 30);
	editbox0->setPosition(550, 370);
	editbox0->setDefaultText("IP address");
	gui->add(editbox0);

	editbox1->setSize(200, 30);
	editbox1->setPosition(550, 420);
	editbox1->setDefaultText("username");
	gui->add(editbox1);

	editbox2->setSize(200, 30);
	editbox2->setPosition(550, 470);
	editbox2->setDefaultText("password");
	editbox2->setPasswordCharacter('*');
	gui->add(editbox2);

	button0->setSize(100, 30);
	button0->setPosition(600, 520);
	button0->connect("pressed", [=]()
	{
		NetworkHandler* nh = this->get_network_handler();
		nh->connect(editbox0->getText().toAnsiString());

		sf::Packet packet;
		packet << 0 << editbox1->getText().toAnsiString() << editbox2->getText().toAnsiString();
		nh->send_packet(packet);
		packet.clear();

		packet = nh->recive_packet();
		bool logged;
		packet >> logged;

		//if (logged)
		//{
			set_active_gui_changed(logged);
			change_active_gui(CurrentActiveGUI::CHARACTERSELECTION);
			this->set_account_name(editbox1->getText().toAnsiString());
		//}


	});
/*	button0->connect("pressed", [](tgui::EditBox::Ptr editbox0, tgui::EditBox::Ptr editbox1) {
		std::cout << "-->login get: " << std::endl;
		std::cout << "username: " << editbox0->getText().toAnsiString() << std::endl;
		std::cout << "password: " << editbox1->getText().toAnsiString() << std::endl;
	});*/

	/*button0->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::CHARACTERSELECTION);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});*/
	gui->add(button0);

	button1->setSize(50, 50);
	button1->setPosition(730, 20);
	button1->connect("pressed", [&]() { window->close(); });
	gui->add(button1);
}

void GuiHandler::tutorial()
{
	tgui::Picture::Ptr tutorialPicture = tgui::Picture::create("Graphics/tutorial.png");
	tgui::Button::Ptr button0 = tgui::Button::create("Back");

	tutorialPicture->setSize(800, 600);
	tutorialPicture->setPosition(0, 0);
	gui->add(tutorialPicture);

	button0->setSize(300, 40);
	button0->setPosition(250, 550);
	button0->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::MAINMENU);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button0);
}

void GuiHandler::game()
{
	gm->run();
}

void GuiHandler::draw()
{
	change_status();
	gui->draw();
}

void GuiHandler::main_loop()
{
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			handle_event(event);

		}

		window->clear();
		draw();
		window->display();
	}
}

void GuiHandler::change_active_gui(CurrentActiveGUI gui)
{
	//change_status();
	active_gui = gui;
	std::cout << "change_active_gui" << (int)gui << "\n";
}

void GuiHandler::on_character_select_button()
{
	std::cout << "-->characterPressed() " << std::endl;
	did_active_gui_changed = true;
	active_gui = CurrentActiveGUI::MAINMENU;
}

void GuiHandler::change_status()
{
	if (did_active_gui_changed)
	{
		switch (active_gui)
		{
		case MAINMENU:
			std::cout << "main_menu init\n";
			gui->removeAllWidgets();
			main_menu();
			break;
		case LOGIN:
			gui->removeAllWidgets();
			login();
			break;
		case CHARACTERSELECTION:
			gui->removeAllWidgets();
			character_selection();
			break;
		case PLAYERSTATISTIC:
			gui->removeAllWidgets();
			statistics();
			break;
		case TUTORIAL:
			gui->removeAllWidgets();
			tutorial();
			break;
		case GAME:
			gui->removeAllWidgets();
			game();
			break;
		default:
			break;
		}


		did_active_gui_changed = false;
	}
}
