/*
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/scriptdev/scriptdevzero>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef DEF_GNOMEREGAN_H
#define DEF_GNOMEREGAN_H

enum
{
    MAX_ENCOUNTER               = 2,                        // Only Grubbis and Thermaplugg need treatment
    MAX_GNOME_FACES             = 6,
    MAX_EXPLOSIVES_PER_SIDE     = 2,

    TYPE_GRUBBIS                = 1,
    TYPE_THERMAPLUGG            = 2,
    TYPE_EXPLOSIVE_CHARGE       = 3,

    DATA_EXPLOSIVE_CHARGE_1     = 1,
    DATA_EXPLOSIVE_CHARGE_2     = 2,
    DATA_EXPLOSIVE_CHARGE_3     = 3,
    DATA_EXPLOSIVE_CHARGE_4     = 4,
    DATA_EXPLOSIVE_CHARGE_USE   = 5,

    NPC_BLASTMASTER_SHORTFUSE   = 7998,

    GO_RED_ROCKET               = 103820,
    GO_CAVE_IN_NORTH            = 146085,
    GO_CAVE_IN_SOUTH            = 146086,
    GO_EXPLOSIVE_CHARGE         = 144065,
    GO_THE_FINAL_CHAMBER        = 142207,

    GO_GNOME_FACE_1             = 142211,
    GO_GNOME_FACE_2             = 142210,
    GO_GNOME_FACE_3             = 142209,
    GO_GNOME_FACE_4             = 142208,
    GO_GNOME_FACE_5             = 142213,
    GO_GNOME_FACE_6             = 142212,

    GO_BUTTON_1                 = 142214,
    GO_BUTTON_2                 = 142215,
    GO_BUTTON_3                 = 142216,
    GO_BUTTON_4                 = 142217,
    GO_BUTTON_5                 = 142218,
    GO_BUTTON_6                 = 142219
};

struct sBombFace
{
    uint64 m_uiGnomeFaceGUID;
    bool m_bActivated;
    uint32 m_uiBombTimer;
};

class MANGOS_DLL_DECL instance_gnomeregan : public ScriptedInstance
{
    public:
        instance_gnomeregan(Map* pMap);
        ~instance_gnomeregan() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        sBombFace* GetBombFaces();
        void DoActivateBombFace(uint8 uiIndex);
        void DoDeactivateBombFace(uint8 uiIndex);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        sBombFace m_asBombFaces[MAX_GNOME_FACES];
        uint64 m_auiExplosiveSortedGUIDs[2][MAX_EXPLOSIVES_PER_SIDE];

        uint64 m_uiBlastmasterShortfuseGUID;
        uint64 m_uiCaveInNorthGUID;
        uint64 m_uiCaveInSouthGUID;
        uint64 m_uiDoorFinalChamberGUID;

        std::list<uint64> m_luiExplosiveChargeGUIDs;
        std::list<uint64> m_luiSpawnedExplosiveChargeGUIDs;
        std::list<uint64> m_lRedRocketGUIDs;
};

#endif
