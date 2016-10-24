#ifndef HUDSCREEN_H
#define HUDSCREEN_H

#include <memory>
#include <irrlicht.h>
#include <IGUIScreen.h>
#include "Config.h"
#include "util.h"

class HUDScreen : public IGUIScreen
{
public:
    HUDScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment);
    ~HUDScreen();

    virtual void initialize(s32 buttonWidth, s32 buttonHeight) override;
    virtual void terminate() override;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) override;
    virtual std::vector<gui::IGUIElement *> getSelectableElements() override;
    virtual void setVisible(bool visible) override;

    void setInfoVisible(bool visible);
    bool getInfoVisible() const;

    gui::IGUIStaticText *textCameraPosition;
    gui::IGUIStaticText *textObstaclesCount;
    gui::IGUIStaticText *textFPS;
    gui::IGUIStaticText *textVelocity;
    gui::IGUIStaticText *textScore;

private:
    bool infoVisible = true;
};

#endif // HUDSCREEN_H
