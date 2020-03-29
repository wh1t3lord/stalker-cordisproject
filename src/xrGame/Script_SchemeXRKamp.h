#pragma once

#include "Script_ISchemeStalker.h"

namespace Cordis
{
    namespace Scripts
    {
        // @ NOT USED YOU MAY COMPLETE BY YOURSELF!
        class Script_EvaluatorKampEnd : public CScriptPropertyEvaluator
        {
        public:
            Script_EvaluatorKampEnd(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
                : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
            {
            }

            virtual _value_type evaluate(void);

        private:
            DataBase::Storage_Scheme* m_p_storage;
        };

        class Script_EvaluatorOnPosition : public CScriptPropertyEvaluator
        {
        public:
            Script_EvaluatorOnPosition(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
                : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
            {
            }

            virtual _value_type evaluate(void);

        private:
            DataBase::Storage_Scheme* m_p_storage;
        };

        class Script_ActionXRKampGoPosition : public CScriptActionBase
        {
        public:
            Script_ActionXRKampGoPosition(CScriptGameObject* const p_object, xr_string& action_name, DataBase::Storage_Scheme& storage) : CScriptActionBase(nullptr, action_name.c_str()), m_p_storage(&storage), m_p_object(p_object)
            {
                
            }

            virtual void initialize(void);
            virtual void execute(void);
            virtual void finalize(void);

        private:
            DataBase::Storage_Scheme* m_p_storage;
            CScriptGameObject* m_p_object;
        };

        // @ action wait
        class Script_SchemeXRKamp : public Script_ISchemeStalker
        {
            
        };

        class Script_XRKamp
        {
        public:
            Script_XRKamp(void) = delete;
            Script_XRKamp(const xr_string& path_name);
            ~Script_XRKamp(void);

            void select_position(const std::uint16_t npc_id);
            void getDestVertex(
                CScriptGameObject* const p_npc, const float radius, std::uint32_t& dest_vertex, std::uint32_t& position);
            void proceedState(CScriptGameObject* const p_npc);
            void proceedRole(CScriptGameObject* const p_npc, const bool is_director, xr_string& state_name, xr_string& sound_name);
            void updateNpc(
                CScriptGameObject* const p_npc, xr_string& state_name, xr_string& sound_name, std::uint32_t& substate);
            void checkNpcAbility(CScriptGameObject* const p_npc);
            void addNpc(CScriptGameObject* const p_npc);
            void removeNpc(CScriptGameObject* const p_npc);
            void increasePops(void);
            void decreasePops(void);
            
            struct NpcData
            {
                public:
                    inline const xr_string& getNpcName(void) const noexcept { return this->m_npc_name; }
                    inline void setNpcName(const xr_string& npc_name) noexcept
                    {
                        if (npc_name.empty())
                        {
                            MESSAGEW("npc_name.empty() == true! Maybe something is wrong");
                        }

                        this->m_npc_name = npc_name;
                    }

                    inline std::uint32_t getBegin(void) const noexcept { return this->m_begin;}
                    inline void setBegin(const std::uint32_t value) noexcept { this->m_begin = value; }

                    inline std::uint32_t getStateIdle(void) const noexcept { return this->m_state_idle; }
                    inline void setStateIdle(const std::uint32_t value) noexcept { this->m_state_idle = value; }

                    inline std::uint32_t getPosition(void) const noexcept { return this->m_position;}
                    inline void setPosition(const std::uint32_t position) noexcept { this->m_position = position; }

                    inline bool isNew(void) const noexcept { return this->m_is_new; }
                    inline void setNew(const bool value) noexcept { this->m_is_new = value; }

                    inline bool isNeedSoundBegin(void) const noexcept { return this->m_is_need_sound_begin; }
                    inline void setNeedSoundBegin(const bool value) noexcept { this->m_is_need_sound_begin = value; }

                    inline const xr_map<xr_string, bool>& getStates(void) const noexcept { return this->m_states;}
                    inline void setState(const xr_string& state_name, const bool value) noexcept
                    {
                        if (state_name.empty())
                        {
                            Msg("[Scripts/Script_XRKamp/NpcData/setState(state_name, value)] WARNING: can't set because a key is null Return ...");
                            return;
                        }

                        this->m_states[state_name] = value;
                    }

                    inline const xr_string& getSelectedStateName(void) const noexcept { return this->m_selected_state_name; }
                    inline void setSelectedStateName(const xr_string& state_name) noexcept 
                    {
                        if (state_name.empty())
                            Msg("[Scripts/Script_XRKamp/NpcData/setSelectedStateName(state_name)] WARNING: state_name.empty() == true!");

                        this->m_selected_state_name = state_name;
                    }

                    inline bool isEmpty(void) const noexcept 
                    {
                        return (!this->m_is_new) && (!this->m_is_need_sound_begin) && (!this->m_begin) && (!this->m_state_idle) && (!this->m_position) && (this->m_states.empty()) && (this->m_selected_state_name.empty()) && (this->m_npc_name.empty());
                    }
            private:
                bool m_is_new = false;
                bool m_is_need_sound_begin = false;
                std::uint32_t m_begin = 0;
                std::uint32_t m_state_idle = 0;
                std::uint32_t m_position = 0;
                xr_map<xr_string, bool> m_states;
                xr_string m_selected_state_name;
                xr_string m_npc_name;
            };

        private:
            std::uint16_t m_director_id;
            std::uint32_t m_population;
            std::uint32_t m_begin;
            CPatrolPathParams* m_p_patrol;
            Script_SoundManager* m_p_sound_manager;
            // @ First - directed | Second - undirected
            xr_map<xr_string, std::pair<xr_vector<xr_string>, xr_vector<xr_string>>> m_avail_states;
            // @ First - directed | Second - undirected
            xr_map<xr_string, std::pair<xr_string, xr_string>> m_avail_sounds;
            xr_map<xr_string, std::pair<std::uint32_t, bool>> m_timeout;
            xr_map<xr_string, bool> m_kamp_states;
            xr_map<xr_string, xr_map<xr_string, std::uint32_t>> m_trans_kamp;
            xr_map<std::uint16_t, NpcData> m_npc;
            xr_vector<std::pair<Fvector, std::uint16_t>> m_position;
            xr_string m_kamp_name;
            xr_string m_kamp_state_name;
        };
    }
}
