#pragma once


#include "Common/LevelGameDef.h"
#include "xrServerEntities/xrServer_Object_Base.h"
#include "xrServerEntities/xrServer_Objects_Abstract.h"

constexpr unsigned int SPAWNPOINT_VERSION = 0x0017;

class CSE_Visual;
class CSE_Motion;
class CObjectAnimator;
class IServerEntity;
class CPhysicsShellHolderEditorBase;
class SDK_CustomObject;
 
namespace Cordis
{
	namespace SDK
	{
		class SDK_ObjectSpawnElement : public SDK_CustomObject, public CPhysicsShellHolderEditorBase
		{
			using inherited = SDK_CustomObject;

		public:
#pragma region CLE_Visual
			// @ Lord: подумать над системой анимаций и найти нормальное решение
			class CLE_Visual
			{
			public:

				CLE_Visual(void) = delete;
				CLE_Visual(CSE_Visual* source);
				virtual ~CLE_Visual(void);

				void OnChangeVisual(void);
				void StopAllAnimations(void);
				void PlayAnimation(void);
				void PlayAnimationFirstFrame(void);
				void PlayAnimationLastFrame(void);
				void PauseAnimation(void);

			public:
				static bool m_g_tmp_lock;
				IRenderVisual* m_visual;
				CSE_Visual* m_source;
			};
#pragma endregion

#pragma region CLE_Motion
			class CLE_Motion
			{
			public:
				CLE_Motion(void) = delete;
				CLE_Motion(CSE_Motion* source);
				virtual ~CLE_Motion(void);

				void OnChangeMotion(void);
				void PlayMotion(void);

			public:
				CSE_Motion* m_source;
				CObjectAnimator* m_animator;
			};
#pragma endregion

#pragma region SpawnData
			class SpawnData : public IServerEntityLEOwner
			{
			public:
				SpawnData(void) = delete;
				SpawnData(SDK_ObjectSpawnElement* object);
				~SpawnData(void);

				void get_bone_xform(LPCSTR name, Fmatrix& xform);

				inline bool Valid(void) { return this->m_data; }

				bool LoadStream(IReader& reader);
				void SaveStream(IWriter& writer);
				bool LoadLTX(CInifile& ini, LPCSTR section_name);
				void SaveLTX(CInifile& ini, LPCSTR section_name);
				bool ExportGame(SExportStreams* stream, SDK_ObjectSpawnElement* owner);
				void Render(bool selected, const Fmatrix& parent, int priority, bool strictB2F);
				void Create(LPCSTR entity_reference);
				void Destroy(void);
				
				void OnFrame(void);

				enum
				{
					eSDTypeRespawn = (1 << 0),
				};

			public:
				Flags8 m_flags;
				CLASS_ID m_classid;
				CSE_Abstract* m_data;
				CLE_Visual* m_visual;
				CLE_Motion* m_motion;
				xr_vector<CLE_Visual*> m_visualhelpers;
				xr_string m_profile;
				SDK_ObjectSpawnElement* m_owner;
			}; // SpawnData
#pragma endregion

			SDK_ObjectSpawnElement(void) = delete;
			SDK_ObjectSpawnElement(LPCSTR name, LPCSTR reference_name);
			~SDK_ObjectSpawnElement(void);

			void Render(const int&, const bool&);
			bool RayPick(float&, const Fvector&, const Fvector&);
			bool FrustumPick(const CFrustum&);
			
			inline bool ReferenceCompare(LPCSTR reference_name){ return reference_name && reference_name[0] && this->m_spawndata.Valid() ? (strcmp(reference_name, this->m_spawndata.m_data->name()) == 0) : false;}
			inline LPCSTR getReferenceName(void) { return this->m_spawndata.Valid() ? this->m_spawndata.m_data->name() : nullptr; }

			bool CreateSpawnData(LPCSTR entity_reference);
			IKinematics* ObjectKinematics(void);
			void RenderSimulationBoxes(void);
			// Lord: потом реализовать данные методы
			bool AttachObject(SDK_CustomObject* object);
			bool OnAppendObject(SDK_CustomObject* object);
			bool LoadLTX(CInifile &ini, LPCSTR section_name);
			void SaveLTX(CInifile &ini, LPCSTR section_name);
			bool LoadStream(IReader &reader);
			void SaveStream(IWriter &writer);	
			bool ExportGame(SExportStreams* stream);
			void DetachObject(void);



		public:
			SpawnData m_spawndata;
			SDK_CustomObject* m_attachedobject;
			EPointType m_type;
			xr_string m_rp_profile;

			// @ Lord: $rpoint, $env_mod
// 			union
// 			{
// 				struct
// 				{
// 					unsigned char m_rp_teamid;
// 					unsigned char m_rp_type;
// 					GameTypeChooser m_gametype;
// 				};
// 
// 				struct
// 				{
// 					Flags16 m_em_flags;
// 					unsigned int m_em_fogcolor;
// 					unsigned int m_em_ambientcolor;
// 					unsigned int m_em_skycolor;
// 					unsigned int m_em_hemicolor;
// 					float m_em_radius;
// 					float m_em_power;
// 					float m_em_viewdist;
// 					float m_em_fogdensity;
// 				};
// 			}; 


		}; // SDK_ObjectSpawnElement
	}
}

