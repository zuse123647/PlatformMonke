#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "main.hpp"
#include "UnityEngine/Vector3.hpp"
#include "custom-types/shared/register.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/Physics.hpp"
#include "UnityEngine/RaycastHit.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/PrimitiveType.hpp"
#include "GorillaLocomotion/Player.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/utils-functions.h"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "config.hpp"
#include "PlatformMonkeWatchView.hpp"
#include "monkecomputer/shared/GorillaUI.hpp"
#include "monkecomputer/shared/Register.hpp"
#include "custom-types/shared/register.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "UnityEngine/Vector3.hpp"

ModInfo modInfo;

#define INFO(value...) getLogger().info(value)
#define ERROR(value...) getLogger().error(value)

using namespace UnityEngine;
using namespace UnityEngine::XR;
using namespace GorillaLocomotion;

bool rightGrip = false;
bool leftGrip = false;
bool isButton = false;
bool isButton2 = false;
Transform* leftPlatformT;
Transform* rightPlatformT;
Vector3 leftHandP;
GameObject* leftPlatform;
GameObject* rightPlatform;
Renderer* renderer2;
Renderer* renderer;

Logger& getLogger()
{
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

bool moddedRoom = false;

MAKE_HOOK_OFFSETLESS(PhotonNetworkController_OnJoinedRoom, void, Il2CppObject* self)
{
    PhotonNetworkController_OnJoinedRoom(self);

    Il2CppObject* currentRoom = (il2cpp_utils::RunMethod("Photon.Pun", "PhotonNetwork"

    if
    {
        moddedRoom = !CRASH_UNLESS(il2cpp_utils::RunMethod<bool>(currentRoom, "get_IsVisible"));
    }
    else moddedRoom = true;

}

void spawnLeftPlatform()
{
    using namespace GlobalNamespace;
    leftGrip = OVRInput::Get(OVRInput::Button::PrimaryHandTrigger, OVRInput::Controller::LTouch);
    Player* playerInstance = Player::get_Instance();
    bool disabled = true;
    if(playerInstance == nullptr) return;
    GameObject* go = playerInstance->get_gameObject();
    auto* player = go->GetComponent<GorillaLocomotion::Player*>();
    Vector3 out2 = Vector3(0.0, 0.0, 0.0);
    Vector3 size = Vector3(0.025, 0.25, 0.32);
    if (leftGrip && !leftPlatformT)
    {
        GameObject* leftPlatform = GameObject::CreatePrimitive(PrimitiveType::Cube);
        leftPlatformT = leftPlatform->get_transform();
        Renderer* renderer = leftPlatformT->GetComponentInChildren<Renderer*>();
        renderer->get_material()->set_color(Color::get_black());
    }
    if (leftGrip)
    {
        if (!leftPlatformT) return;
        Transform* leftHandT = player->leftHandTransform;
        Vector3 leftHandP = leftHandT->get_position();
        Quaternion leftHandR = leftHandT->get_rotation();
        leftPlatformT->set_localScale(size);
        leftPlatformT->set_position(leftHandP);
        leftPlatformT->set_rotation(leftHandR);
        disabled = false;
    }
    if (!leftGrip)
    {
        if (!leftPlatformT) return;
        leftPlatformT->set_position(out2);
        disabled = true;
    }
}

void spawnRightPlatform()
{
    using namespace GlobalNamespace;
    rightGrip = OVRInput::Get(OVRInput::Button::PrimaryHandTrigger, OVRInput::Controller::RTouch);
    Player* playerInstance = Player::get_Instance();
    bool disabled2 = true;
    if(playerInstance == nullptr) return;
    Vector3 out = Vector3(0.0, 0.0, 0.0);
    Vector3 size2 = Vector3(0.025, 0.25, 0.32);
    GameObject* go = playerInstance->get_gameObject();
    auto* player = go->GetComponent<GorillaLocomotion::Player*>();
    if (rightGrip && !rightPlatformT)
    {
        GameObject* rightPlatform = GameObject::CreatePrimitive(PrimitiveType::Cube);
        rightPlatformT = rightPlatform->get_transform();
        Renderer* renderer = rightPlatformT->GetComponentInChildren<Renderer*>();
        renderer->get_material()->set_color(Color::get_black());
    }
    if (rightGrip)
    {
        if (!rightPlatformT) return;
        Transform* rightHandT = player->rightHandTransform;
        Vector3 rightHandP = rightHandT->get_position();
        Quaternion rightHandR = rightHandT->get_rotation();
        rightPlatformT->set_localScale(size2);
        rightPlatformT->set_position(rightHandP);
        rightPlatformT->set_rotation(rightHandR);
        disabled2 = false;
    }
    if (!rightGrip)
    {
        if (!rightPlatformT) return;
        rightPlatformT->set_position(out);
        disabled2 = true;
    }
}


void Buttn()
{
    using namespace GlobalNamespace;	
    if (moddedRoom && config.enabled)
	{
        leftGrip = OVRInput::Get(OVRInput::Button::PrimaryHandTrigger, OVRInput::Controller::LTouch);
        rightGrip = OVRInput::Get(OVRInput::Button::PrimaryHandTrigger, OVRInput::Controller::RTouch);
        
        if (!isButton)
        {
            spawnLeftPlatform();
            isButton = true; 
        }
        if (isButton)
        {
            if (!leftGrip)
            {
                isButton = false;
            }
        }
        
        if (!isButton2)
        {
            spawnRightPlatform();
            isButton2 = true;

        }
        if (isButton2)
        {
            if (!rightGrip)
            {
                isButton2 = false;
            }
        }
    }
}

MAKE_HOOK_OFFSETLESS(Player_Update, void, Il2CppObject* self)
{
    using namespace UnityEngine;
    using namespace GlobalNamespace;
    Player_Update(self);
    if (moddedRoom)
    {
        Buttn();
    }
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;

    modInfo = info;
}

extern "C" void load()
{
    getLogger().info("Loading mod...");

    GorillaUI::Init();

    INSTALL_HOOK_OFFSETLESS(getLogger(), PhotonNetworkController_OnJoinedRoom, il2cpp_utils::FindMethodUnsafe("", "PhotonNetworkController", "OnJoinedRoom", 0));
	INSTALL_HOOK_OFFSETLESS(getLogger(), Player_Update, il2cpp_utils::FindMethodUnsafe("GorillaLocomotion", "Player", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(getLogger(), Player_Awake, il2cpp_utils::FindMethodUnsafe("GorillaLocomotion", "Player", "Awake", 0));
    
    custom_types::Register::RegisterType<PlatformMonke::PlatformMonkeWatchView>(); 
    GorillaUI::Register::RegisterWatchView<PlatformMonke::PlatformMonkeWatchView*>("Platform Monke", VERSION);
    getLogger().info("Mod loaded!");
}
