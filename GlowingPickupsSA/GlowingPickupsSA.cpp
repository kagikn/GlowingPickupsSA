#include <cmath>
#include "GlowingPickupsSA.h"
#include "CPickups.h"
#include "CTimer.h"
#include "CCoronas.h"
#include "CShadows.h"
#include "CPointLights.h"
#include "CCamera.h"
#include "CModelInfo.h"

void GlowingPickup::GlowNormalPickup(CEntity* pickupEntity, Color color)
{
    const auto grobalTimer = CTimer::m_snTimeInMilliseconds;    
    const auto glowingStage = static_cast<unsigned char>((grobalTimer >> 9) & 7);

    bool glowOuterRing = false;
    bool glowInnerRing = false;
    
    if (glowingStage >= 3)
    {
        if(glowingStage == 3)
        {
            //Probably the same random function that is used in DoPickUpEffects(0x43F050 in v1.0) in VC 
            glowOuterRing = (rand() & 3) != 0;
        }
        else
        {
            glowOuterRing = true;
        }
    }

    const auto glowingStage2 = (static_cast<unsigned char>(glowingStage - 1) & 7);

    if (glowingStage2 >= 3)
    {
        if (glowingStage2 == 3)
        {
            //Probably the same random function that is used in DoPickUpEffects(0x43F050 in v1.0) in VC 
            glowInnerRing = (rand() & 3) != 0;
        }
        else
        {
            glowInnerRing = true;
        }
    }

    CVector center = pickupEntity->TransformFromObjectSpace(CVector(0.0f, 0.0f, 0.0f));
    auto& cent = center;

    constexpr float INNER_CORONA_RADIUS = 0.6f;
    constexpr float OUTER_CORONA_RADIUS = 0.76f;

    if (glowOuterRing)
    {
        
        CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(pickupEntity), nullptr, static_cast<unsigned char>(static_cast<double>(color.m_red) * 0.45f), static_cast<unsigned char>(static_cast<double>(color.m_green) * 0.45f), static_cast<unsigned char>(static_cast<double>(color.m_blue) * 0.45f),
               255, cent, OUTER_CORONA_RADIUS, 65.0f, (eCoronaType)9, FLARETYPE_NONE, false, false, 0, 0.0f, false, -0.4f, 0, 15.0f, false, true);
        CShadows::StoreStaticShadow(reinterpret_cast<unsigned int>(pickupEntity), 2, gpShadowExplosionTex, &center, 2.0f, 0, 0, -2.0f, 0x10,
            static_cast<unsigned char>(color.m_red * 0.30f), static_cast<unsigned char>(color.m_green * 0.30f), static_cast<unsigned char>(color.m_blue * 0.30f),
            4.0f, 1.0f, 40.0f, false, 0.0f);
        
        const float lightRange = static_cast<float>(static_cast<double>(rand() % 0xF) * 0.1f + 3.0f);
        constexpr float lightIntensity = 0.00390625f;
        const float lightColorRed = static_cast<float>(static_cast<double>(color.m_red) * lightIntensity);
        const float lightColorGreen = static_cast<float>(static_cast<double>(color.m_green) * lightIntensity);
        const float lightColorBlue = static_cast<float>(static_cast<double>(color.m_blue) * lightIntensity);

        CPointLights::AddLight(0, cent, CVector(), lightRange, lightColorRed, lightColorGreen, lightColorBlue, 0, true, nullptr);
    }
    else
    {
        CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(pickupEntity), nullptr, 0, 0, 0, 255, cent,
            OUTER_CORONA_RADIUS, 65.0f, (eCoronaType)9, FLARETYPE_NONE, false, false, 0, 0.0f, false, 1.5f, 0, 15.0f, true, true);
    }

    if (glowInnerRing)
    {
        CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(pickupEntity) + 1, nullptr, static_cast<unsigned char>(color.m_red * 0.44999999), static_cast<unsigned char>(color.m_green * 0.44999999), static_cast<unsigned char>(color.m_blue * 0.44999999),
            255, cent, INNER_CORONA_RADIUS, 65.0f, (eCoronaType)9, FLARETYPE_NONE, false, false, 0, 0.0f, false, -0.4f, 0, 15.0f, false, true);
        CShadows::StoreStaticShadow(reinterpret_cast<unsigned int>(pickupEntity), 2, gpShadowExplosionTex, &center, 2.0f, 0, 0, -2.0f, 0x10,
            static_cast<unsigned char>(color.m_red * 0.30f), static_cast<unsigned char>(color.m_green * 0.30f), static_cast<unsigned char>(color.m_blue * 0.30f),
            4.0f, 1.0f, 40.0f, false, 0.0f);
    }
    else
    {
        CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(pickupEntity) + 1, nullptr, 0, 0, 0, 255, cent,
            INNER_CORONA_RADIUS, 65.0f, (eCoronaType)9, FLARETYPE_NONE, false, false, 0, 0.0f, false, 1.5f, 0, 15.0f, false, true);
    }

    if (!CModelInfo::ms_modelInfoPtrs[pickupEntity->m_nModelIndex])
    {
        return;
    }
    for(int i = 0; i < 4; i++)
    {
        const auto boundBox = CModelInfo::ms_modelInfoPtrs[pickupEntity->m_nModelIndex]->m_pColModel->m_boundBox;
        const auto vecDifferenceX = boundBox.m_vecMax.x - boundBox.m_vecMin.x;
        const auto vecDifferenceY = boundBox.m_vecMax.y - boundBox.m_vecMin.y;
        const auto vecDifferenceZ = boundBox.m_vecMax.z - boundBox.m_vecMin.z;
        
        enum MaxcomponentOfVector{ x, y, z };
        
        auto maxDifference = vecDifferenceZ;
        MaxcomponentOfVector maxComp = z;
        if (vecDifferenceY >= maxDifference)
        {
            maxDifference = vecDifferenceY;
            maxComp = y;
        }
        if (vecDifferenceX >= maxDifference)
        {
            maxDifference = vecDifferenceX;
            maxComp = x;
        }

        constexpr double oneThird = 1.0 / 3.0;
        CVector vectorOffset;
        const auto maxOffset = static_cast<float>(i * oneThird * maxDifference);

        switch (maxComp)
        {
            case x:
                pickupEntity->TransformFromObjectSpace(vectorOffset, CVector(maxOffset, 0.0f, 0.0f));
                break;
            case y:
                pickupEntity->TransformFromObjectSpace(vectorOffset, CVector(0.0f, maxOffset, 0.0f));
                break;
            case z:
                pickupEntity->TransformFromObjectSpace(vectorOffset, CVector(0.0f, 0.0f, maxOffset));
                break;
        }

        CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(pickupEntity) + 8 + i, nullptr, static_cast<unsigned char>(color.m_red * 0.15f), static_cast<unsigned char>(color.m_green * 0.15f), static_cast<unsigned char>(color.m_blue * 0.15f),
            255, vectorOffset, 1.0f, 65.0f, CORONATYPE_SHINYSTAR, FLARETYPE_NONE, false, false, 0, 0.0f, false, -0.5f, 0, 15.0f, false, true);
    }
}

void GlowingPickup::GlowNormalPickupCenter(CEntity* pickupEntity, Color color)
{
    CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(pickupEntity) + 9, nullptr, static_cast<unsigned char>(color.m_red * 0.495f), static_cast<unsigned char>(color.m_green * 0.495f), static_cast<unsigned char>(color.m_blue * 0.495f),
        255, pickupEntity->GetPosition(), 1.2f, 50.0f, CORONATYPE_SHINYSTAR, FLARETYPE_NONE, true, false, 1, 0.0f, false, 1.5f, 0, 15.0f, false, true);
}

void GlowingPickup::GlowMoneyPickup(CEntity* moneyPickupEntity)
{
    const auto theCam = reinterpret_cast<CCamera*>(0xB6F028); //The camera address is the same among v1.0 and v1.01, but plugin-sdk supports TheCamera only for 1.0US  
    const float distance = DistanceBetweenPoints(theCam->m_matrix->pos, moneyPickupEntity->m_matrix->pos);
    
    if (distance < 20.0f)
    {
        const auto grobalTimer = CTimer::m_snTimeInMilliseconds;

#pragma warning(push)
#pragma warning(disable : 4302)
        const short seed = (static_cast<unsigned short>(grobalTimer) + reinterpret_cast<unsigned short>(moneyPickupEntity)) & 0x3FF;
#pragma warning(pop) 
        double sine = std::sin(static_cast<double>(seed) * 0.0061328127f);
        if (seed & 0x400)
        {
            sine = deg2rad(distance);
        }

        const auto greenIntensity = static_cast<unsigned char>((20.0f - distance) * (0.2f * static_cast<float>(sine) + 0.3) * 0.05f * 64.0f);

        CVector& center = moneyPickupEntity->m_matrix->pos;

        constexpr float moneyCoronaRadius = 0.25f; // originally 0.4f in VC

        CShadows::StoreStaticShadow(reinterpret_cast<unsigned int>(moneyPickupEntity), 2, gpShadowExplosionTex, &center, 2.0f, 0, 0, -2.0f, 0x10,
            0, greenIntensity, 0, 4.0f, 1.0f, 40.0f, false, 0.0f);
        CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(moneyPickupEntity), nullptr, 0, greenIntensity, 0, 255, center,
            moneyCoronaRadius, 40.0f, (eCoronaType)9, FLARETYPE_NONE, false, false, 0, 0.0f, false, 1.5f, 0, 15.0f, false, true);
    }
}

void GlowingPickup::GlowCollectiblePickup(CEntity* pickupEntity)
{
    const auto theCam = reinterpret_cast<CCamera*>(0xB6F028); //The camera address is the same among v1.0 and v1.01, but plugin-sdk supports TheCamera only for 1.0US  
    const float distance = DistanceBetweenPoints(theCam->m_matrix->pos, pickupEntity->m_matrix->pos);

    if (distance <= 14.0f)
    {
        const auto grobalTimer = CTimer::m_snTimeInMilliseconds;

#pragma warning(push)
#pragma warning(disable : 4302)
        const short seed = (static_cast<unsigned short>(grobalTimer) + reinterpret_cast<unsigned short>(pickupEntity)) & 0x7FF;
#pragma warning(pop)
        double sine = std::sin(static_cast<double>(seed * 0.00306640635));
        if (static_cast<unsigned short>(sine) & 0x400)
        {
            sine = deg2rad(distance);
        }

        const auto colorIntensity = static_cast<unsigned char>((14.0f - distance) * (0.5f * static_cast<float>(sine) + 0.5f) * 0.0714285746f * 255.0f);

        CVector& center = pickupEntity->m_matrix->pos;

        CShadows::StoreStaticShadow(reinterpret_cast<unsigned int>(pickupEntity), 2, gpShadowExplosionTex, &center, 2.0f, 0, 0, -2.0f, 0x10,
            colorIntensity, colorIntensity, colorIntensity, 4.0f, 1.0f, 40.0f, false, 0.0f);
        CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(pickupEntity), nullptr, colorIntensity, colorIntensity, colorIntensity, 255, center,
            0.6f, 40.0f, (eCoronaType)9, FLARETYPE_NONE, false, false, 0, 0.0f, false, 1.5f, 0, 15.0f, false, true);
    }
}


void GlowingPickup::Main()
{
    constexpr int PICKUP_LIMIT = 620;

    for (int i = 0; i < PICKUP_LIMIT; i++)
    {
        CPickup& pickup = CPickups::aPickUps[i];

        if (pickup.m_nFlags.bDisabled || !pickup.m_nFlags.bVisible || !pickup.m_pObject)
        {
            continue;
        }

        const auto pickupType = pickup.m_nPickupType;
        
        if (pickup.m_nModelIndex == MODEL_CJ_HORSE_SHOE || pickup.m_nModelIndex == MODEL_CJ_OYSTER || pickup.m_nModelIndex == MODEL_CJ_OYSTER_2)
        {
            GlowCollectiblePickup(pickup.m_pObject);
        }
        else if (pickup.m_nModelIndex == MODEL_MONEY)
        {
            GlowMoneyPickup(pickup.m_pObject);
        }
        else if (pickupType != PICKUP_NONE)
        {
            auto modelIndex = pickup.m_pObject->m_nModelIndex;

            auto iteratorForIgnoredModels = ModelsToIgnore.find(modelIndex);
            const int& WeaponIDPlayerIsAiming = *(int*)0xB6F1A8;
                 
            if (iteratorForIgnoredModels != ModelsToIgnore.end()
                || (pickup.m_nModelIndex == MODEL_CAMERAPICKUP && WeaponIDPlayerIsAiming != 46))
            {
                continue;
            }

            auto it1 = centerGlowingOnlyModels.find(modelIndex);

            auto it2 = pickupWeaponModelAndColorMap.find(modelIndex);
            const Color color = it2 == pickupWeaponModelAndColorMap.end() ? PickupColorList::Default : it2->second;

            if (it1 == centerGlowingOnlyModels.end())
            {
                GlowNormalPickup(pickup.m_pObject, color);
            }
            else
            {
                GlowNormalPickupCenter(pickup.m_pObject, color);
            }
        }
    }
}

GlowingPickup::GlowingPickup() {
    plugin::Events::gameProcessEvent += Main;
}
