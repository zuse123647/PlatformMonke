#include "PlatformMonkeWatchView.hpp"
#include "config.hpp"
#include "monkecomputer/shared/ViewLib/MonkeWatch.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "UnityEngine/Vector3.hpp"

DEFINE_TYPE(PlatformMonke::PlatformMonkeWatchView);

using namespace GorillaUI;
using namespace UnityEngine;

extern bool moddedRoom;

namespace PlatformMonke
{
    void PlatformMonkeWatchView::Awake()
    {
        toggleHandler = new UIToggleInputHandler(EKeyboardKey::Enter, EKeyboardKey::Enter, true);
    }

    void PlatformMonkeWatchView::DidActivate(bool firstActivation)
    {
        std::function<void(bool)> fun = std::bind(&PlatformMonkeWatchView::OnToggle, this, std::placeholders::_1);
        toggleHandler->toggleCallback = fun;
        Redraw();
    }

    void PlatformMonkeWatchView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawBody();

        watch->Redraw();
    }

    void PlatformMonkeWatchView::DrawHeader()
    {
        text += "<color=#136CFF>== <color=#FF2F3C>Platform Monke</color> ==</color>\n";
    }

    void PlatformMonkeWatchView::DrawBody()
    {
        text += "\nPlatform Monke is:\n\n  ";
        text += config.enabled ? "<color=#00ff00>> Enabled</color>" : "<color=#ff0000>> Disabled</color>";

        if (config.enabled && !moddedRoom)
        {
            text += "\n\nBut is disabled\ndue to not being in\na private room\n";
        }
    }

    void PlatformMonkeWatchView::OnToggle(bool value)
    {
        config.enabled = value;

        SaveConfig();
    }

    void PlatformMonkeWatchView::OnKeyPressed(int key)
    {
        toggleHandler->HandleKey((EKeyboardKey)key);
        Redraw();
    }
}