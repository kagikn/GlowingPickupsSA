#pragma once
#include "plugin.h"
#include "common.h"
#include <unordered_map>
#include <unordered_set>
#include "CPickup.h"

struct Color
{
    Color() : m_red(0), m_green(0), m_blue(0), m_alpha(0)
    {
    }

    Color(const unsigned char red, const unsigned char green, const unsigned char blue) : m_red(red), m_green(green), m_blue(blue), m_alpha(255)
    {
    }

    Color(const unsigned char red, const char green, const unsigned char blue, const unsigned char alpha) : m_red(red), m_green(green), m_blue(blue), m_alpha(255)
    {
    }

    unsigned char m_red;
    unsigned char m_green;
    unsigned char m_blue;
    unsigned char m_alpha;
};

class PickupColorList
{
public:
    // exactly the same colors defined in VC

    inline static Color Default = Color(0x80, 0x80, 0x80);
    inline static Color MeleeWeapons = Color(0x61, 0xC2, 0xF7);
    inline static Color ThrownWeapons = Color(0x1B, 0x59, 0x82); // This value is also used for missiles in VC
    inline static Color Handguns = Color(0x95, 0xC2, 0x18);
    inline static Color Shotguns = Color(0x2D, 0x9B, 0x5A);
    inline static Color SubmachineGuns = Color(0xFF, 0xE3, 0x4F);
    inline static Color AssaultRifles = Color(0xFE, 0x89, 0x00);
    inline static Color SniperRifles = Color(0xF9, 0x83, 0xD7);
    inline static Color HeavyWeapons = Color(0xA4, 0x28, 0xB2);
    inline static Color MiscWeapons = Color(0x45, 0x45, 0x45); // This value seems to be used for detonators and cameras in VC
    inline static Color Health = Color(0xFF, 0x64, 0x64);
    inline static Color BodyArmor = Color(0x80, 0xFF, 0x80);
    inline static Color PropertySale = Color(0x64, 0xFF, 0x64);
    inline static Color PropertyLocked = Color(0x64, 0x64, 0xFF);
    inline static Color Yellow = Color(0xFF, 0xFF, 0x64);
    inline static Color Purple = Color(0xFF, 0x8B, 0xF4); // The original color in VC is #7E4579 (126, 69, 121), which is used for center-glowing pickups

    // completely original colors

    inline static Color Special1 = Color(0xA6, 0xE2, 0xD2);
    inline static Color Gifts = Color(0xF3, 0x44, 0x67);
    inline static Color Special2 = Color(0x3A, 0x3D, 0xD9);
};

class CPickupFla {
public:
    float             m_fRevenueValue;
    CObject* m_pObject;
    unsigned int      m_nAmmo;
    unsigned int      m_nRegenerationTime;
    CompressedVector  m_vecPos;
    unsigned short    m_nMoneyPerDay;
    short             m_nModelIndex;
    short             m_nReferenceIndex;
    unsigned char     m_nPickupType; // see ePickupType
    struct {
        unsigned char   bDisabled : 1; // waiting for regeneration
        unsigned char   bEmpty : 1; // no ammo
        unsigned char   bHelpMessageDisplayed : 1;
        unsigned char   bVisible : 1;
        unsigned char   nPropertyTextIndex : 3; // see enum ePickupPropertyText
    }                 m_nFlags;
    char _pad1E[2];
    int32_t x, y, z;

    static CPickupFla* aPickUps;
};

class GlowingPickup {
public:
    // pickup colors in VC are defined for model IDs. Changing the weapon slot of a specific weapon doesn't change the pickup color. 
    inline static const std::unordered_map<unsigned int, Color> pickupWeaponModelAndColorMap
    {  {MODEL_BRASSKNUCKLE, PickupColorList::Default},
       {MODEL_GOLFCLUB, PickupColorList::MeleeWeapons}, {MODEL_NITESTICK, PickupColorList::MeleeWeapons}, {MODEL_KNIFECUR, PickupColorList::MeleeWeapons},
        {MODEL_BAT, PickupColorList::MeleeWeapons}, {MODEL_SHOVEL, PickupColorList::MeleeWeapons}, {MODEL_POOLCUE, PickupColorList::MeleeWeapons},
        {MODEL_KATANA, PickupColorList::MeleeWeapons}, {MODEL_CHNSAW, PickupColorList::MeleeWeapons},
       {MODEL_GRENADE, PickupColorList::ThrownWeapons}, {MODEL_TEARGAS, PickupColorList::ThrownWeapons}, {MODEL_MOLOTOV, PickupColorList::ThrownWeapons},
        {MODEL_MISSILE, PickupColorList::ThrownWeapons}, {MODEL_SATCHEL, PickupColorList::ThrownWeapons},
       {MODEL_COLT45, PickupColorList::Handguns}, {MODEL_SILENCED, PickupColorList::Handguns}, {MODEL_DESERT_EAGLE, PickupColorList::Handguns},
       {MODEL_CHROMEGUN, PickupColorList::Shotguns}, {MODEL_SAWNOFF, PickupColorList::Shotguns}, {MODEL_SHOTGSPA, PickupColorList::Shotguns},
       {MODEL_MICRO_UZI, PickupColorList::SubmachineGuns}, {MODEL_MP5LNG, PickupColorList::SubmachineGuns}, {MODEL_TEC9, PickupColorList::SubmachineGuns},
       {MODEL_AK47, PickupColorList::AssaultRifles}, {MODEL_M4, PickupColorList::AssaultRifles},
       {MODEL_CUNTGUN, PickupColorList::SniperRifles}, {MODEL_SNIPER, PickupColorList::SniperRifles},
       {MODEL_ROCKETLA, PickupColorList::HeavyWeapons}, {MODEL_HEATSEEK, PickupColorList::HeavyWeapons}, {MODEL_FLAME, PickupColorList::HeavyWeapons},
        {MODEL_MINIGUN, PickupColorList::HeavyWeapons},
       {MODEL_SPRAYCAN, PickupColorList::Special1}, {MODEL_FIRE_EX, PickupColorList::Special1}, {MODEL_CAMERA, PickupColorList::Special1},
       {MODEL_GUN_DILDO1, PickupColorList::Gifts}, {MODEL_GUN_DILDO2, PickupColorList::Gifts}, {MODEL_GUN_VIBE1, PickupColorList::Gifts},
        {MODEL_GUN_VIBE2, PickupColorList::Gifts}, {MODEL_FLOWERA, PickupColorList::Gifts}, {MODEL_GUN_CANE, PickupColorList::Gifts},
       {MODEL_NVGOGGLES, PickupColorList::Special2}, {MODEL_IRGOGGLES, PickupColorList::Special2}, {MODEL_GUN_PARA, PickupColorList::Special2},
       {MODEL_JETPACK, PickupColorList::MiscWeapons}, {MODEL_HEALTH, PickupColorList::Health}, {MODEL_BODYARMOUR, PickupColorList::BodyArmor},
       {MODEL_PROPERTY_FSALE, PickupColorList::PropertySale}, {MODEL_PROPERTY_LOCKED, PickupColorList::PropertyLocked}, 
       {MODEL_BRIBE, PickupColorList::Yellow}, {MODEL_BIGDOLLAR, PickupColorList::Yellow}, {MODEL_INFO, PickupColorList::Yellow},
       {MODEL_PICKUPSAVE, PickupColorList::PropertyLocked}, {MODEL_CLOTHESP, PickupColorList::PropertyLocked},
       {MODEL_CAMERAPICKUP, PickupColorList::Purple},

       {MODEL_BONUS, PickupColorList::Health},
    };

    inline static const std::unordered_set<unsigned int> ModelsToIgnore
    { MODEL_BRIEFCASE,
    };

    inline static const std::unordered_set<unsigned int> centerGlowingOnlyModels
    { MODEL_INFO, MODEL_HEALTH, MODEL_ADRENALINE, MODEL_BRIBE, MODEL_BONUS, MODEL_CAMERAPICKUP, MODEL_KILLFRENZY, MODEL_PROPERTY_LOCKED, MODEL_PROPERTY_FSALE,
        MODEL_BIGDOLLAR, MODEL_CLOTHESP, MODEL_PICKUPSAVE, MODEL_KILLFRENZY2PLYR, MODEL_TWOPLAYER,
    };

    GlowingPickup();
    static void Main();
    static void GlowPickup(CPickup* pickup);
    static void GlowNormalPickup(CEntity* pickupEntity, Color color);
    static void GlowNormalPickupCenter(CEntity* pickupEntity, Color color);
    static void GlowMoneyPickup(CEntity* moneyPickupEntity);
    static void GlowCollectiblePickup(CEntity* pickupEntity);

    static double deg2rad(double value)
    {
        return value * static_cast<double>(3.141592653589793116) / static_cast<double>(180.0);
    }
} glowingPickup;
