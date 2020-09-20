#include "about_tab.hpp"

AboutTab::AboutTab() {		
	brls::Label* Label1 = new brls::Label(brls::LabelStyle::REGULAR, "Reset Parental Controls 1.1.1-borealis", false);
	this->addView(Label1);

	brls::Label* Label2 = new brls::Label(brls::LabelStyle::MEDIUM, "Research & Development: TotalJustice\nPort to Borealis UI: MasaGratoR", true);
	this->addView(Label2);
	
	brls::Label* Label3 = new brls::Label(brls::LabelStyle::DESCRIPTION, "Copyright (C) 2020\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3 of the License. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program.  If not, see https://www.gnu.org/licenses/", true);
	this->addView(Label3);
}