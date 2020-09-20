#include <switch.h>

#include <borealis.hpp>

#include "About_tab.hpp"

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

int main(int argc, char *argv[])
{
	// Init the app

	if (!brls::Application::init("Reset Parental Controls"))
	{
		brls::Logger::error("Unable to init Reset Parental Controls");
		return EXIT_FAILURE;
	}

	// Create a sample view
	pctlInitialize();
	brls::TabFrame* rootFrame = new brls::TabFrame();
	rootFrame->setTitle("Reset Parental Controls");
	rootFrame->setIcon(BOREALIS_ASSET("icon.jpg"));
	
	brls::List* OptionsList = new brls::List();

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
		if (R_FAILED(rc)) {
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
	
	rootFrame->addTab("About", new AboutTab());


	// Add the root view to the stack
	brls::Application::pushView(rootFrame);

	// Run the app
	while (brls::Application::mainLoop());

	// Exit
	pctlExit();
	return EXIT_SUCCESS;
}