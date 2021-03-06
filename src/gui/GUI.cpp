/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui/GUI.h"

using namespace irr;

class TerminatedScreen : public IGUIScreen
{
public:
    TerminatedScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
        IGUIScreen(configuration, guiEnvironment) {}
    virtual ~TerminatedScreen() {}

    virtual void initialize(s32, s32) override {}
    virtual void reload(s32, s32) override {}
    virtual void terminate() override {}
    virtual void resize(s32, s32) override {}
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override { return {}; }
    virtual void setVisible(bool) override {}
};

GUI::GUI(ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    configuration(configuration), guiEnvironment(guiEnvironment)
{
    addScreen(std::make_unique<TerminatedScreen>(configuration, guiEnvironment), Screen::TERMINATED);
}

GUI::~GUI()
{
    getCurrentScreen().terminate();
}

unsigned GUI::getCurrentScreenIndex() const
{
    return currentScreenIndex;
}

bool GUI::addScreen(std::unique_ptr<IGUIScreen> screen, unsigned index)
{
    if (screens.find(index) != screens.end())
        return false;

    screens[index] = std::move(screen);
    return true;
}

bool GUI::removeScreen(unsigned screenIndex)
{
    if (screens.find(screenIndex) != screens.end())
        return false;

    screens.erase(screenIndex);
    return true;
}

IGUIScreen &GUI::getCurrentScreen()
{
    return *screens[currentScreenIndex];
}

void GUI::recalculateButtonProportions()
{
    // button width calculated from window width
    buttonWidth = configuration.resolution.Width / 6;

    // minimal button width
    if (buttonWidth < 220)
        buttonWidth = 220;

    // height calculated from width
    buttonHeight = buttonWidth * 1/8;
}

void GUI::initialize(unsigned screenIndex)
{
    recalculateButtonProportions();
    getCurrentScreen().terminate();
    currentScreenIndex = screenIndex;
    getCurrentScreen().initialize(buttonWidth, buttonHeight);
    selectableElements = getCurrentScreen().getSelectableElements();

    guiEnvironment.setFocus(selectableElements.empty() ? 0 : selectableElements.front());
    selectedElement = 0;
    updateSelection();
}

void GUI::reload()
{
    getCurrentScreen().reload(buttonWidth, buttonHeight);
}

void GUI::updateSelection()
{
    if (selectableElements.empty())
        return;

    getCurrentScreen().reload(buttonWidth, buttonHeight);

    if (guiEnvironment.getFocus()->getType() != gui::EGUIET_EDIT_BOX &&
        guiEnvironment.getFocus()->getType() != gui::EGUIET_COMBO_BOX &&
        guiEnvironment.getFocus()->getType() != gui::EGUIET_BUTTON)
    {
        core::stringw str = "[";
        str += guiEnvironment.getFocus()->getText();
        str += "]";
        guiEnvironment.getFocus()->setText(str.c_str());
    }
}

void GUI::selectWithTab()
{
    if (selectableElements.empty())
        return;

    for (std::size_t i = 0; i < selectableElements.size(); i++)
        if (guiEnvironment.getFocus() == selectableElements[i]) {
            selectedElement = i;
            break;
        }
    updateSelection();
}

void GUI::selectElement(std::size_t index)
{
    if (selectableElements.empty())
        return;

    guiEnvironment.setFocus(selectableElements[index]);
    selectedElement = index;
    updateSelection();
}

void GUI::selectNextElement()
{
    if (selectableElements.empty())
        return;

    if (selectedElement < selectableElements.size() - 1)
        selectedElement++;
    else
        selectedElement = 0;

    guiEnvironment.setFocus(selectableElements[selectedElement]);
    updateSelection();
}

void GUI::selectPreviousElement()
{
    if (selectableElements.empty())
        return;

    if (selectedElement > 0)
        selectedElement--;
    else
        selectedElement = selectableElements.size() - 1;

    guiEnvironment.setFocus(selectableElements[selectedElement]);
    updateSelection();
}

void GUI::terminate()
{
    initialize(Screen::TERMINATED);
}

void GUI::setVisible(bool visible)
{
    getCurrentScreen().setVisible(visible);
}

void GUI::resize()
{
    //changing gui position and size when window is resizing
    recalculateButtonProportions();
    getCurrentScreen().resize(buttonWidth, buttonHeight);
}
