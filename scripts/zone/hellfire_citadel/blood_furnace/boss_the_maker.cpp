/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "../../../sc_defines.h"

#define SPELL_ACID_SPRAY		38153 // heroic 38973 ??? 38153
#define SPELL_EXPLODING_BREAKER	30925 
#define SPELL_KNOCKDOWN			20276
#define SPELL_DOMINATION		25772 // ???


#define SAY_KILL_1				"Let's see what I can make of you." 
#define SOUND_KILL_1			10289
#define SAY_KILL_2				"It is pointless to resist."
#define SOUND_KILL_2			10290

#define SAY_AGGRO_1				"My work must not be interrupted."
#define SOUND_AGGRO_1			10286
#define SAY_AGGRO_2				"Perhaps I can find a use for you." 
#define SOUND_AGGRO_2			10287
#define SAY_AGGRO_3				"Anger... Hate... These are tools I can use."	
#define SOUND_AGGRO_3			10288			

#define SAY_DIE					"Stay away from... me." 
#define SOUND_DIE				10291				

struct MANGOS_DLL_DECL boss_the_makerAI : public ScriptedAI
{
    boss_the_makerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}   

    uint32 AcidSpray_Timer;
    uint32 ExplodingBreaker_Timer;
    uint32 Domination_Timer;
    uint32 Knockdown_Timer;

    bool InCombat;

    void EnterEvadeMode()
    {   
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);

        AcidSpray_Timer = 2000;
        ExplodingBreaker_Timer = 4000;
        Domination_Timer = 120000;
        Knockdown_Timer	= 6000;

        InCombat = false;

    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                switch(rand()%3)
                {
                case 0:
                    DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                    break;

                case 1:
                    DoYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
                    break;  

                case 2:
                    DoYell(SAY_AGGRO_3, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_3);
                    break;  
                }
                InCombat = true;
            }
        }
    }

    void KilledUnit(Unit* victim)
    {
        if (rand()%5)
            return;

        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_KILL_1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_1);
            break;

        case 1:
            DoYell(SAY_KILL_2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_2);
            break;               
        }
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DIE,LANG_UNIVERSAL,NULL);  
        DoPlaySoundToSet(m_creature,SOUND_DIE);
    }

    void MoveInLineOfSight(Unit *who)
    {  
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH); 
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    switch(rand()%3)
                    {
                    case 0:
                        DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                        break;

                    case 1:
                        DoYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
                        break;  

                    case 2:
                        DoYell(SAY_AGGRO_3, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_3);
                        break;  
                    }
                    InCombat = true;
                }
            }
        }
    }  

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(AcidSpray_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_ACID_SPRAY);
            AcidSpray_Timer = 4000+rand()%8000;
        }else AcidSpray_Timer -=diff;

        if(ExplodingBreaker_Timer < diff)
        {
            Unit* target;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            DoCast(target,SPELL_EXPLODING_BREAKER);
            ExplodingBreaker_Timer = 4000+rand()%8000;
        }else ExplodingBreaker_Timer -=diff;

        /* // Disabled until Core Support for mind control
        if(domination_timer_timer < diff)
        {
        Unit* target;
        target = SelectUnit(SELECT_TARGET_RANDOM,0);

        DoCast(target,SPELL_DOMINATION);

        domination_timer = 120000;
        }else domination_timer -=diff;
        */

        if(Knockdown_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_KNOCKDOWN);
            Knockdown_Timer = 4000+rand()%8000;
        }else Knockdown_Timer -=diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_the_makerAI(Creature *_Creature)
{
    return new boss_the_makerAI (_Creature);
}


void AddSC_boss_the_maker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_the_maker";
    newscript->GetAI = GetAI_boss_the_makerAI;
    m_scripts[nrscripts++] = newscript;
}