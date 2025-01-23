#pragma once

#include "Skyrim_Cursor.h"

// Logo Level
#include "Logo_Camera.h"
#include "BackGround.h"
#include "Skyrim_LogoObj.h"

// GamePlay Level
#include "Navigation_Client.h"
#include "Terrain.h"
#include "Sky.h"
#include "ParticleRect.h"
#include "Camera_Player.h"

#include "SceneChangeCol.h"

// Tool Level
#include "Tool_Camera.h"
#include "Terrain_Grid.h"

/* Player */
#include "Player.h"
#include "Player_CameraPart.h"
#include "Player_Body.h"
#include "Player_Head.h"
#include "Player_Hand.h"
#include "Player_Foot.h"
#include "Player_Weapon.h"

#pragma region NPC
/* Carlotta */
#include "NPC_Carlotta.h"

#include "NPC_GuardCaptain.h"
#include "GuardCaptain_Weapon.h"

#include "NPC_GuardSupply.h"

#include "NPC_Guard.h"
#include "Guard_Weapon.h"

// ´õ¹Ì
#include "Carlotta_Body.h"
#include "Carlotta_Head.h"
#include "Carlotta_Hand.h"
#include "Carlotta_Foot.h"
#pragma endregion

#pragma region Monster 

/* Skeever */
#include "Skeever.h"
#include "Skeever_Weapon.h"

/* FalmerUE */
#include "Falmer_UnEquip.h"
#include "FalmerUE_Weapon.h"

/* FalmerOH */
#include "Falmer_OneHand.h"
#include "FalmerOH_Weapon.h"

/* Spider */
#include "Spider.h"
#include "Spider_Mouth.h"
#include "Projectile_Web.h"

/* Boss Spider */
#include "BossSpider.h"
#include "BossSpider_Mouth.h"
#include "BossSpider_Left.h"
#include "BossSpider_Right.h"

#pragma endregion

#pragma region Light
#include "Light_Point.h"
#pragma endregion

#pragma region Effect
#include "Effect_BloodFlare.h"
#include "Effect_BloodSpot.h"
#include "Effect_FadeBlack.h"
#include "Effect_IntroFade.h"
#include "Effect_ScreenBlur.h"
#include "Effect_CombatSpark.h"
#pragma endregion

#pragma region Placeable Object
#include "SkyrimRock.h"
#include "SkyrimTerrain.h"
#include "Building.h"
#include "StoneWork.h"
#include "DGPlaceableObj.h"
#include "CastleObject.h"
#pragma endregion

#pragma region Collisible Object 
#include "CaveRock.h"
#include "SpiderEgg.h"
#include "FalmerHouse.h"
#include "FalmerFence.h"
#include "CavePillars.h"
#pragma endregion 

#pragma region Weapon 
#include "Weapon_FalmerAxe.h"

#include "Weapon_AkaviriSword.h"
#pragma endregion 

#pragma region Armor 
#include "Nude_Female.h"

#include "Armor_Glass.h"
#include "Helmet_Glass.h"

#include "TorsoF_Merchant01.h"
#include "BootsF_Merchant01.h"

#include "TorsoM_Blades.h"
#include "HelmetM_Blades.h"
#include "GlovesM_Blades.h"
#include "BootsM_Blades.h"
#pragma endregion

#pragma region UI
#include "SkyrimUI_SceneChange.h"
#include "SkyrimUI_HpBar.h"
#include "SkyrimUI_SpBar.h"
#include "SkyrimUI_MonsterHpBar.h"
#include "SkyrimUI_BossHpBar.h"
#include "SkyrimUI_Letter.h"
#include "SkyrimUI_Talk.h"
#include "SkyrimUI_TalkIcon.h"

#include "Inventory.h"
#include "Inventory_ItemCategory.h"
#include "Inventory_ItemList.h"
#include "Inventory_UnderBar.h"

#pragma endregion
