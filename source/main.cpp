#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <switch.h>

#include <stdlib.h>

#include <borealis.hpp>

// works
Result pctlDeleteParentalControls(void)
{
	return serviceDispatch(pctlGetServiceSession_Service(), 1043);
}

// works
Result pctlDeletePairing(void)
{
	return serviceDispatch(pctlGetServiceSession_Service(), 1941);
}

void init_app(void)
{
	pctlInitialize();
}

void exit_app(void)
{
	pctlExit();
}

int main(int argc, char *argv[])
{
	// Init the app

	if (!brls::Application::init("Reset Parental Controls"))
	{
		brls::Logger::error("Unable to init Reset Parental Controls");
		return EXIT_FAILURE;
	}

	// Create a sample view
	brls::TabFrame* rootFrame = new brls::TabFrame();
	rootFrame->setTitle("Reset Parental Controls");
	rootFrame->setIcon(BOREALIS_ASSET("icon.jpg"));
	
	brls::List* OptionsList = new brls::List();
	brls::List* AboutList = new brls::List();

	brls::ListItem* RegisterPasscode = new brls::ListItem("Register parental control PIN");
	RegisterPasscode->getClickEvent()->subscribe([](brls::View* view) {
		pctlExit();
		if (R_FAILED(pctlauthRegisterPasscode())) {
			std::string notification = "Process canceled.";
			brls::Application::notify(notification);
		}
		else {
			std::string notification = "PIN registered successfully!";
			brls::Application::notify(notification);
		}
		pctlInitialize();
	});
	
	OptionsList->addView(RegisterPasscode);
	
	brls::ListItem* DeleteParentalControls = new brls::ListItem("Delete parental controls");
	DeleteParentalControls->getClickEvent()->subscribe([](brls::View* view) {
		bool isRestricted = false;
		pctlIsRestrictionEnabled(&isRestricted);
		if (isRestricted == true) {
			if (R_FAILED(pctlDeleteParentalControls())) {
				std::string notification = "Process canceled.";
				brls::Application::notify(notification);
			}
			else {
				std::string notification = "Parental controls removed successfully.";
				brls::Application::notify(notification);
			}
		}
		else {
		 std::string notification = "Parental controls are already disabled.";
		 brls::Application::notify(notification);
		}
	});
	
	OptionsList->addView(DeleteParentalControls);
	
	brls::ListItem* DeletePairing = new brls::ListItem("Delete parental control app pairing");
	DeletePairing->getClickEvent()->subscribe([](brls::View* view) {
		Result rc = pctlDeletePairing();
		if (rc == 0xE401) {
			std::string notification = "Parental control app is not paired.";
			brls::Application::notify(notification);
		}
		else if (R_FAILED(rc)) {
			std::string notification = "Process canceled.";
			brls::Application::notify(notification);
		}
		else {
			std::string notification = "Parental control app pairing was deleted successfully.";
			brls::Application::notify(notification);
		}
	});
	
	OptionsList->addView(DeletePairing);
	
	rootFrame->addTab("Options", OptionsList);
	
    brls::Label* Label1 = new brls::Label(brls::LabelStyle::REGULAR, "Reset Parental Controls 1.1.1", false);
    AboutList->addView(Label1);

    brls::Label* Label2 = new brls::Label(brls::LabelStyle::MEDIUM, "Research & Development: TotalJustice\nPort to Borealis UI: MasaGratoR", true);
    AboutList->addView(Label2);
	
	brls::Label* Label3 = new brls::Label(brls::LabelStyle::DESCRIPTION, "Copyright (C) 2020\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3 of the License. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program.  If not, see https://www.gnu.org/licenses/", true);
    AboutList->addView(Label3);
	
	rootFrame->addTab("About", AboutList);


	// Add the root view to the stack
	brls::Application::pushView(rootFrame);

	// Run the app
	while (brls::Application::mainLoop());

	// Exit
	return EXIT_SUCCESS;
}