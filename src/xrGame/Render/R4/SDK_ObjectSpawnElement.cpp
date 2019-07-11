#include "StdAfx.h"
#include "SDK_ObjectSpawnElement.h"
#include "xrServerEntities/xrMessages.h"
#include "xrServerEntities/xrServer_Objects_ALife.h"
#include "SkeletonAnimated.h"
#include "xrEngine/ObjectAnimator.h"
#include "SIniFileStream.h"
 

#define SPAWNPOINT_CHUNK_VERSION 0xE411
#define SPAWNPOINT_CHUNK_POSITION 0xE412
#define SPAWNPOINT_CHUNK_RPOINT 0xE413
#define SPAWNPOINT_CHUNK_DIRECTION 0xE414
#define SPAWNPOINT_CHUNK_SQUADID 0xE415
#define SPAWNPOINT_CHUNK_GROUPID 0xE416
#define SPAWNPOINT_CHUNK_TYPE 0xE417
#define SPAWNPOINT_CHUNK_FLAGS 0xE418

#define SPAWNPOINT_CHUNK_ENTITYREF 0xE419
#define SPAWNPOINT_CHUNK_SPAWNDATA 0xE420

#define SPAWNPOINT_CHUNK_ATTACHED_OBJ 0xE421

#define SPAWNPOINT_CHUNK_ENVMOD 0xE422
#define SPAWNPOINT_CHUNK_ENVMOD2 0xE423
#define SPAWNPOINT_CHUNK_ENVMOD3 0xE424
#define SPAWNPOINT_CHUNK_FLAGS 0xE425

//----------------------------------------------------
#define RPOINT_SIZE 0.5f
#define ENVMOD_SIZE 0.25f
#define MAX_TEAM 6
const u32 RP_COLORS[MAX_TEAM] = { 0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0x00ffff, 0xff00ff };

namespace Cordis
{
	namespace SDK
	{
#pragma region CLE_Visual
		bool SDK_ObjectSpawnElement::CLE_Visual::m_g_tmp_lock = false;
		SDK_ObjectSpawnElement::CLE_Visual::CLE_Visual(CSE_Visual* source) : m_source(source), m_visual(nullptr)
		{

		}
		
		SDK_ObjectSpawnElement::CLE_Visual::~CLE_Visual(void)
		{
			if (this->m_visual)
				RImplementation.model_Delete(this->m_visual, true);
		}

		void SDK_ObjectSpawnElement::CLE_Visual::OnChangeVisual(void)
		{
			if (this->m_visual)
				RImplementation.model_Delete(this->m_visual, true);

			if (this->m_source->visual_name.size())
			{
				this->m_visual = RImplementation.model_Create(this->m_source->visual_name.c_str());

				if (!this->m_visual && !this->m_g_tmp_lock)
				{
					SDKUI_Log::Widget().SetColor(error);
					SDKUI_Log::Widget().AddText("This section doesn't implemented!");
					// Lord: сюда modal window
					/*
								xr_string _msg = "Model [" + xr_string(source->visual_name.c_str()) +
				"] not found. Do you want to select it from library?";
			int mr = ELog.DlgMsg(mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, _msg.c_str());
			LPCSTR _new_val = 0;
			g_tmp_lock = true;
			if (mr == mrYes && TfrmChoseItem::SelectItem(smVisual, _new_val, 1))
			{
				source->visual_name = _new_val;
				visual = ::Render->model_Create(source->visual_name.c_str());
			}
			g_tmp_lock = false;
					*/
				}

				this->PlayAnimationFirstFrame();
			}
		}

		void SDK_ObjectSpawnElement::CLE_Visual::PlayAnimation(void)
		{
			if (this->m_g_tmp_lock)
				return;

			this->StopAllAnimations();

			CKinematicsAnimated* kinematics_animated = PKinematicsAnimated(this->m_visual);
			IKinematics* kinematics = PKinematics(this->m_visual);
			if (kinematics_animated)
			{
				MotionID id = kinematics_animated->ID_Cycle_Safe(this->m_source->startup_animation.c_str());
				if (id.valid())
					kinematics_animated->PlayCycle(id);
			}

			if (kinematics)
				kinematics->CalculateBones();
		}

		void SDK_ObjectSpawnElement::CLE_Visual::StopAllAnimations(void)
		{
			if (this->m_g_tmp_lock)
				return;

			CKinematicsAnimated* kinematics_animated = PKinematicsAnimated(this->m_visual);
			if (kinematics_animated)
			{
				for (uint16_t i = 0; i < MAX_PARTS; ++i)
				{
					kinematics_animated->LL_CloseCycle(i, unsigned char(-1));
				}
			}
		}

		void SDK_ObjectSpawnElement::CLE_Visual::PlayAnimationFirstFrame(void)
		{
			if (this->m_g_tmp_lock)
				return;

			this->StopAllAnimations();

			CKinematicsAnimated* kinematics_animated = PKinematicsAnimated(this->m_visual);
			IKinematics* kinematics = PKinematics(this->m_visual);

			if (kinematics_animated)
			{
				MotionID id = kinematics_animated->ID_Cycle_Safe(this->m_source->startup_animation.c_str());
				if (id.valid())
				{
					kinematics_animated->PlayCycle(id);
					this->PauseAnimation();
				}
				else
				{
					SDKUI_Log::Widget().SetColor(warning);
					SDKUI_Log::Widget().AddText("Visual - [%s] has no animation -> [%s]", this->m_source->visual_name.c_str(), this->m_source->startup_animation.c_str());
				}
			}

			if (kinematics)
				kinematics->CalculateBones();
		}
		// Lord: вообще переписать систему анимаций ПЫС, создать что-то новое и получше
		struct SetBlendLastFrameCB : public IterateBlendsCallback
		{
			virtual void operator()(CBlend& B)
			{
				B.timeCurrent = B.timeTotal - 0.4444f;
				B.blendAmount = 1.0f;
				B.playing = !B.playing;
			}
		} g_Set_blend_last_frame_CB;

		void SDK_ObjectSpawnElement::CLE_Visual::PlayAnimationLastFrame()
		{
			if (this->m_g_tmp_lock)
				return;

			this->StopAllAnimations();

			CKinematicsAnimated* kinematics_animated = PKinematicsAnimated(this->m_visual);
			IKinematics* kinematics = PKinematics(this->m_visual);

			if (kinematics_animated)
			{
				MotionID id = kinematics_animated->ID_Cycle_Safe(this->m_source->startup_animation.c_str());
				if (id.valid())
				{
					kinematics_animated->PlayCycle(id);
					kinematics_animated->LL_IterateBlends(g_Set_blend_last_frame_CB);
				}
			}

			if (kinematics)
			{
				kinematics->CalculateBones();
			}
		}

		struct TogglelendCB : public IterateBlendsCallback
		{
			virtual void operator()(CBlend& B) { B.playing = !B.playing; }
		} g_toggle_pause_blendCB;

		void SDK_ObjectSpawnElement::CLE_Visual::PauseAnimation()
		{
			if (this->m_g_tmp_lock)
				return;

			CKinematicsAnimated* kinematics_animated = PKinematicsAnimated(this->m_visual);
			IKinematics* kinematics = PKinematics(this->m_visual);

			if (kinematics_animated)
			{
				kinematics_animated->LL_IterateBlends(g_toggle_pause_blendCB);
			}

			if (kinematics)
			{
				kinematics->CalculateBones();
			}
 		}
#pragma endregion


#pragma region CLE_Motion
		SDK_ObjectSpawnElement::CLE_Motion::CLE_Motion(CSE_Motion* source) : m_source(source), m_animator(nullptr)
		{

		}

		SDK_ObjectSpawnElement::CLE_Motion::~CLE_Motion(void)
		{

		}

		void SDK_ObjectSpawnElement::CLE_Motion::OnChangeMotion(void)
		{
			if (this->m_animator)
				xr_delete(this->m_animator);

			if (this->m_source->motion_name.size())
			{
				this->m_animator = new CObjectAnimator();
				this->m_animator->Load(this->m_source->motion_name.c_str());
				this->PlayMotion();
			}
		}

		void SDK_ObjectSpawnElement::CLE_Motion::PlayMotion(void)
		{
			if (this->m_animator)
				this->m_animator->Play(true);
		}
#pragma endregion 


#pragma region SpawnData
		SDK_ObjectSpawnElement::SpawnData::SpawnData(SDK_ObjectSpawnElement* object) : m_classid(0), m_data(nullptr), m_visual(nullptr), m_motion(nullptr), m_owner(object)
		{
			this->m_flags.zero();
		}

		SDK_ObjectSpawnElement::SpawnData::~SpawnData(void)
		{
			this->Destroy();
		}

		void SDK_ObjectSpawnElement::SpawnData::Create(LPCSTR entity_reference)
		{
			this->m_data = object_factory().server_object(pSettings->r_clsid(entity_reference, "class"), entity_reference);
 
			if (this->m_data)
			{
				// Lord: здесь багует вызывает вместо set_name -> angle()
 
 
				if (this->m_data->visual())
				{
                    this->m_visual = new CLE_Visual(this->m_data->visual());
					this->m_data->set_editor_flag(IServerEntity::flVisualChange | IServerEntity::flVisualAnimationChange);
				}

				if (this->m_data->motion())
				{
                    this->m_motion = new CLE_Motion(this->m_data->motion());
					this->m_data->set_editor_flag(IServerEntity::flMotionChange);
				}
 
				if (pSettings->line_exist(entity_reference, "$player"))
				{
					if (pSettings->r_bool(entity_reference, "$player"))
					{
						this->m_data->flags().set(M_SPAWN_OBJECT_ASPLAYER, true);
					}
				}
				this->m_classid = pSettings->r_clsid(entity_reference, "class");
			}
			else
			{
				SDKUI_Log::Widget().SetColor(warning);
				SDKUI_Log::Widget().AddText("Can't create the entity -> %s", entity_reference);
			}
		}

		void SDK_ObjectSpawnElement::SpawnData::Destroy(void)
		{
			if (this->m_data)
			{
				IServerEntity* reference = this->m_data;
				Engine.External.m_callback_destroy_entity(reference);
			}


			if (this->m_visual)
				xr_delete(this->m_visual);

			if (this->m_motion)
				xr_delete(this->m_motion);
		}

		void SDK_ObjectSpawnElement::SpawnData::get_bone_xform(LPCSTR name, Fmatrix& xform)
		{
			if (!name)
			{
				SDKUI_Log::Widget().SetColor(warning);
				SDKUI_Log::Widget().AddText("The name of the bone was null!");
				return;
			}
				
			xform.identity();

			if (this->m_visual && this->m_visual->m_visual)
			{
				IKinematics* kinematics = PKinematics(this->m_visual->m_visual);
				if (kinematics)
				{
					uint16_t id = kinematics->LL_BoneID(name);
					if (id != BI_NONE)
						xform = kinematics->LL_GetTransform(id);
				}
			}
		}
 
		bool SDK_ObjectSpawnElement::SpawnData::LoadLTX(CInifile& ini, LPCSTR section_name)
		{
			xr_string object_section = ini.r_string(section_name, "name");
			this->Create(object_section.c_str());

			if (ini.line_exist(section_name, "fl"))
				this->m_flags.assign(ini.r_u8(section_name, "fl"));

			NET_Packet packet; // @ Лол, пакет
			SIniFileStream inistream;
			inistream.m_ini = &ini;
			inistream.m_section = section_name;
			inistream.move_begin();
			packet.inistream = &inistream;
			// Lord: Подумать здесь 
			/*
				if (Valid())
		if (!m_Data->Spawn_Read(Packet))
			Destroy();

	return Valid();
			*/

			return false;
		}


		void SDK_ObjectSpawnElement::SpawnData::SaveLTX(CInifile& ini, LPCSTR section_name)
		{
			ini.w_string(section_name, "name", this->m_data->name());
			ini.w_u8(section_name, "fl", this->m_flags.get());

			NET_Packet packet;
			SIniFileStream inistream;
			inistream.m_ini = &ini;
			inistream.m_section = section_name;
			inistream.move_begin();
			packet.inistream = &inistream;

			this->m_data->Spawn_Write(packet, TRUE);
		}

		void SDK_ObjectSpawnElement::SpawnData::SaveStream(IWriter& writer)
		{
			writer.open_chunk(SPAWNPOINT_CHUNK_ENTITYREF);
			writer.w_stringZ(this->m_data->name());
			writer.close_chunk();

			writer.open_chunk(SPAWNPOINT_CHUNK_FLAGS);
			writer.w_u8(this->m_flags.get());
			writer.close_chunk();

			writer.open_chunk(SPAWNPOINT_CHUNK_SPAWNDATA);
			
			NET_Packet packet;
			this->m_data->Spawn_Write(packet, TRUE);
			writer.w_u32(packet.B.count);
			writer.w(packet.B.data, packet.B.count);
			writer.close_chunk();
		}

		bool SDK_ObjectSpawnElement::SpawnData::LoadStream(IReader& reader)
		{
			string64 temporary;
			R_ASSERT(reader.find_chunk(SPAWNPOINT_CHUNK_ENTITYREF));
			reader.r_stringZ(temporary, sizeof(temporary));

			if (reader.find_chunk(SPAWNPOINT_CHUNK_FLAGS))
				this->m_flags.assign(reader.r_u8());
			
			NET_Packet packet;

			R_ASSERT(reader.find_chunk(SPAWNPOINT_CHUNK_SPAWNDATA));
			packet.B.count = reader.r_u32();
			reader.r(packet.B.data, packet.B.count);

			this->Create(temporary);


			return false;
			/* Lord: подумать
				if (Valid())
		if (!m_Data->Spawn_Read(Packet))
			Destroy();

	return Valid();
			*/
		}

		// Lord: Данный метод реализовать!!!
		bool SDK_ObjectSpawnElement::SpawnData::ExportGame(SExportStreams* stream, SDK_ObjectSpawnElement* owner)
		{
			this->m_data->set_name_replace(owner->m_scene_name.c_str());
			this->m_data->position().set(owner->getPosition());
			this->m_data->angle().set(owner->getRotation());

			IServerEntityShape* cform = this->m_data->shape();

			if (cform && !(owner->m_attachedobject && (owner->m_attachedobject->getType() == kSection_Shapes)))
			{
				SDKUI_Log::Widget().SetColor(warning);
				SDKUI_Log::Widget().AddText("Spawn Element: '%s' must contain attached shape.", owner->m_scene_name.c_str());
				return false;
			}

			if (cform)
			{
				SDK_ObjectShape* shape = dynamic_cast<SDK_ObjectShape*>(owner->m_attachedobject);
				R_ASSERT(shape);
				// shape->ApplyScale();
				owner->setScale(shape->getScale());
				// Lord: Данный метод не реализован смотри метод cform->assign_shapes()
			//	cform->assign_shapes(&*)
			}

			NET_Packet packet;
			this->m_data->Spawn_Write(packet, TRUE);

			SExportStreamItem& item = (this->m_flags.test(eSDTypeRespawn)) ? stream->spawn_rs : stream->spawn;
			item.stream.open_chunk(item.chunk++);
			item.stream.w(packet.B.data, packet.B.count);
			item.stream.close_chunk();

			return true;
		}

		// Lord: интерпретировать данный функционал 
		/*
		void CSpawnPoint::SSpawnData::OnAnimControlClick(ButtonValue* value, bool& bModif, bool& bSafe)
{
	ButtonValue* B = dynamic_cast<ButtonValue*>(value);
	R_ASSERT(B);
	switch (B->btn_num)
	{
	//      "First,Play,Pause,Stop,Last",
	case 0: // first
	{
		m_Visual->PlayAnimationFirstFrame();
	}
	break;
	case 1: // play
	{
		m_Visual->PlayAnimation();
	}
	break;
	case 2: // pause
	{
		m_Visual->PauseAnimation();
	}
	break;
	case 3: // stop
	{
		m_Visual->StopAllAnimations();
	}
	break;
	case 4: // last
	{
		m_Visual->PlayAnimationLastFrame();
	}
	break;
	}
}

void CSpawnPoint::SSpawnData::FillProp(LPCSTR pref, PropItemVec& items)
{
	m_Data->FillProp(pref, items);

	if (Scene->m_LevelOp.m_mapUsage.MatchType(
			eGameIDDeathmatch | eGameIDTeamDeathmatch | eGameIDArtefactHunt | eGameIDCaptureTheArtefact))
		PHelper().CreateFlag8(items, PrepareKey(pref, "MP respawn"), &m_flags, eSDTypeRespawn);

	if (m_Visual)
	{
		ButtonValue* BV = PHelper().CreateButton(
			items, PrepareKey(pref, m_Data->name(), "Model\\AnimationControl"), "|<<,Play,Pause,Stop,>>|", 0);
		BV->OnBtnClickEvent.bind(this, &CSpawnPoint::SSpawnData::OnAnimControlClick);
	}
}
		*/

		void SDK_ObjectSpawnElement::SpawnData::Render(bool selected, const Fmatrix& parent, int priority, bool strictB2F)
		{
			if (this->m_visual && this->m_visual->m_visual)
				RImplementation.Models->Render((dxRender_Visual*)this->m_visual->m_visual, parent, priority, strictB2F, 1.0f);

			if (this->m_motion && this->m_motion->m_animator && selected && (priority == 1) && (strictB2F == false))
			{	// Lord: смотри данный метод CObjectAnimator::DrawPath()! Его нужно реализовать в DUImpl
				//this->m_motion->m_animator->DrawPath();
			}

			RCache.set_xform_world(Fidentity);
			RCache.set_Shader(RImplementation.m_WireShader);
/*   Lord: Сделать данные методы видимыми!!!
			this->m_data->on_render(&DUImpl, this, selected, parent, priority, strictB2F);

			if (selected)
			{
				Fmatrix matrix;
				unsigned int index = 0;

				for (xr_vector<CLE_Visual*>::iterator it = this->m_visualhelpers.begin(); it != this->m_visualhelpers.end(); ++it, ++index)
				{
					visual_data* visual_collection_element = this->m_data->visual_collection() + index;
					matrix.mul(parent, visual_collection_element->matrix);
					CLE_Visual* visual = *it;
					RImplementation.Models->Render((dxRender_Visual*)visual->m_visual, matrix, priority, strictB2F, 1.0f);
				}
			}*/
		}

		// Lord: не совсем реализовано!!!
		void SDK_ObjectSpawnElement::SpawnData::OnFrame(void)
		{
			if (this->m_data->m_editor_flags.is(IServerEntity::flUpdateProperties))
			{
				// Lord: подумать оставить именно так или передалть?
				// ExecCommand(COMMAND_UPDATE_PROPERTIES);
			}

			if (this->m_visual)
			{
				if (this->m_data->m_editor_flags.is(IServerEntity::flVisualChange))
				{
					this->m_visual->OnChangeVisual();
				}

				if (this->m_data->m_editor_flags.is(IServerEntity::flVisualAnimationChange))
				{
					this->m_visual->PlayAnimationFirstFrame();
					this->m_data->m_editor_flags.set(IServerEntity::flVisualAnimationChange, FALSE);
				}

				if (this->m_visual->m_visual && PKinematics(this->m_visual->m_visual))
				{
					PKinematics(this->m_visual->m_visual)->CalculateBones(TRUE);
				}
			} // m_visual

			if (this->m_motion)
			{
				if (this->m_data->m_editor_flags.is(IServerEntity::flMotionChange))
					this->m_motion->OnChangeMotion();

				if (this->m_motion->m_animator)
					this->m_motion->m_animator->Update(Device.fTimeDelta);
			} // m_motion

			if (this->m_data->m_editor_flags.is(IServerEntity::flVisualChange))
			{
				for (auto it : this->m_visualhelpers)
				{
					xr_delete(it);
				}
				this->m_visualhelpers.clear();

/* Lord: Сделать данные методы видимыми!!!
				visual_data* _visual_data = this->m_data->visual_collection();

				for (unsigned int i = 0; i < this->m_data->visual_collection_size(); ++i, ++_visual_data)
				{
					CLE_Visual* visual = new CLE_Visual(_visual_data->visual);
					visual->OnChangeVisual();
					this->m_visualhelpers.push_back(visual);
					visual->PlayAnimation();
				}*/
			} // flVisualChange

			for (xr_vector<CLE_Visual*>::value_type it : this->m_visualhelpers)
			{
				IKinematics* kinematics = PKinematics(it->m_visual);

				if (kinematics)
					kinematics->CalculateBones(TRUE);
				
			}

			this->m_data->m_editor_flags.zero();
		}
#pragma endregion

		SDK_ObjectSpawnElement::SDK_ObjectSpawnElement(LPCSTR name, LPCSTR reference_name) : inherited(name), m_spawndata(this), m_rp_profile(""), m_attachedobject(nullptr)
		{
			this->m_id_objecttype = kSection_SpawnElements;

			/*
				if (data)
	{
		if (strcmp(LPSTR(data), RPOINT_CHOOSE_NAME) == 0)
		{
			m_Type = ptRPoint;
			m_RP_Type = rptActorSpawn;
			m_GameType.SetDefaults();
			m_RP_TeamID = 1;
		}
		else if (strcmp(LPSTR(data), ENVMOD_CHOOSE_NAME) == 0)
		{
			m_Type = ptEnvMod;
			m_EM_Radius = 10.f;
			m_EM_Power = 1.f;
			m_EM_ViewDist = 300.f;
			m_EM_FogColor = 0x00808080;
			m_EM_FogDensity = 1.f;
			m_EM_AmbientColor = 0x00000000;
			m_EM_SkyColor = 0x00FFFFFF;
			m_EM_HemiColor = 0x00FFFFFF;
		}
		else
		{
			CreateSpawnData(LPCSTR(data));
			if (!m_SpawnData.Valid())
			{
				SetValid(false);
			}
			else
			{
				m_Type = ptSpawnPoint;
			}
		}
	}
	else
	{
		SetValid(false);
	}
			*/

//			this->setValid(); Lord: подумать над данным методом !!!! Вообще нужно или нет
			//this->m_em_flags.one();
			this->CreateSpawnData(reference_name);
			if (!this->m_spawndata.Valid())
			{
				SDKUI_Log::Widget().SetColor(warning);
				SDKUI_Log::Widget().AddText("Can't create geometry for spawnelement - %s", name);
			}
		}

		SDK_ObjectSpawnElement::~SDK_ObjectSpawnElement(void)
		{
			if (this->m_attachedobject)
				xr_delete(this->m_attachedobject);
		}


		void SDK_ObjectSpawnElement::RenderSimulationBoxes(void)
		{
			Fvector center, dimensions;
			this->getBox().get_CD(center, dimensions);
			Fmatrix matrix;
			matrix.scale(Fvector().mul(dimensions, 2.0f));
			matrix.c.set(center);
			unsigned int color = 0x06005000;
			DUImpl.DrawIdentBox(matrix, false);
		}

		void SDK_ObjectSpawnElement::Render(const int&, const bool&)
		{
			if (this->m_physics_shell)
			{
				this->UpdateObjectXform(this->m_transform_rotation_position);
				this->RenderSimulationBoxes();
			}

			//  Scene->SelectLightsForObject(this);

			if (this->m_attachedobject)
				this->m_attachedobject->Render(1, true);

			if (this->m_spawndata.Valid())
			{
				this->m_spawndata.Render(this->IsSelected(), this->m_transform_rotation_position, 1, false);
				
				/* Lord: реализовать
				RCache.set_xform_world(this->m_transform_rotation_position);
								ESceneSpawnTool* st = dynamic_cast<ESceneSpawnTool*>(ParentTool);
				VERIFY(st);
				ref_shader s = st->GetIcon(m_SpawnData.m_Data->name());
				DU_impl.DrawEntity(0xffffffff, s);
				*/
			}


			
		}


		bool SDK_ObjectSpawnElement::RayPick(float& distance, const Fvector& start, const Fvector& direction)
		{
			if (this->m_attachedobject)
			{
				return this->m_attachedobject->RayPick(distance, start, direction);
			}


			Fvector position;
			float radius;
			this->getBox().getsphere(position, radius);

			Fvector ray;
			ray.sub(position, start);

			float result = ray.dotproduct(direction);

			if (result > 0)
			{
				float result2 = ray.magnitude();

				if (((result2 * result2 - result * result) < (radius * radius)) && (result > radius))
				{
					Fvector point;
					Fbox fbox = this->getBox();
					if (Fbox::rpOriginOutside == fbox.Pick2(start, direction, point))
					{
						result = start.distance_to(point);
						if (result < distance)
						{
							distance = result;
							return true;
						}
					}
				}
			}




			return false;
		}

		bool SDK_ObjectSpawnElement::FrustumPick(const CFrustum& frustum)
		{
			if (this->m_attachedobject && this->m_attachedobject->FrustumPick(frustum))
				return true;

			unsigned int mask = 0xff;
			return frustum.testAABB(this->getBox().data(), mask);
		}

		bool SDK_ObjectSpawnElement::CreateSpawnData(LPCSTR entity_reference)
		{
			if (!entity_reference)
			{
				SDKUI_Log::Widget().SetColor(error);
				SDKUI_Log::Widget().AddText("Can't create data for this object, because reference was null!");
				return false;
			}
			this->m_spawndata.Destroy();
			this->m_spawndata.Create(entity_reference);

			bool result = this->m_spawndata.Valid();

			if (result)
				this->m_type = ptSpawnPoint;

			return result;

		}

		IKinematics* SDK_ObjectSpawnElement::ObjectKinematics(void)
		{
			if (!this->m_spawndata.m_visual || this->m_spawndata.m_visual->m_visual)
				return nullptr;

			return this->m_spawndata.m_visual->m_visual->dcast_PKinematics();
		}

		/*
		void CSpawnPoint::OnFillChooseItems(ChooseValue* val)
{
	ESceneSpawnTool* st = dynamic_cast<ESceneSpawnTool*>(ParentTool);
	VERIFY(st);
	CLASS_ID cls_id = m_SpawnData.m_ClassID;
	ESceneSpawnTool::ClassSpawnMapIt cls_it = st->m_Classes.find(cls_id);
	VERIFY(cls_it != st->m_Classes.end());
	*val->m_Items = cls_it->second;
}

shared_str CSpawnPoint::SectionToEditor(shared_str nm)
{
	ESceneSpawnTool* st = dynamic_cast<ESceneSpawnTool*>(ParentTool);
	VERIFY(st);
	ESceneSpawnTool::ClassSpawnMapIt cls_it = st->m_Classes.find(m_SpawnData.m_ClassID);
	VERIFY(cls_it != st->m_Classes.end());
	for (ESceneSpawnTool::SSVecIt ss_it = cls_it->second.begin(); ss_it != cls_it->second.end(); ++ss_it)
		if (nm.equal(ss_it->hint))
			return ss_it->name;
	return 0;
}

shared_str CSpawnPoint::EditorToSection(shared_str nm)
{
	ESceneSpawnTool* st = dynamic_cast<ESceneSpawnTool*>(ParentTool);
	VERIFY(st);
	ESceneSpawnTool::ClassSpawnMapIt cls_it = st->m_Classes.find(m_SpawnData.m_ClassID);
	VERIFY(cls_it != st->m_Classes.end());
	for (ESceneSpawnTool::SSVecIt ss_it = cls_it->second.begin(); ss_it != cls_it->second.end(); ++ss_it)
		if (nm.equal(ss_it->name))
			return ss_it->hint;
	return 0;
}

void CSpawnPoint::OnRPointTypeChange(PropValue* prop) { ExecCommand(COMMAND_UPDATE_PROPERTIES); }
void CSpawnPoint::OnProfileChange(PropValue* prop)
{
	if (m_SpawnData.m_Profile.size() != 0)
	{
		shared_str s_name = EditorToSection(m_SpawnData.m_Profile);
		VERIFY(s_name.size());
		if (0 != strcmp(m_SpawnData.m_Data->name(), *s_name))
		{
			IServerEntity* tmp = create_entity(*s_name);
			VERIFY(tmp);
			NET_Packet Packet;
			tmp->Spawn_Write(Packet, TRUE);
			R_ASSERT(m_SpawnData.m_Data->Spawn_Read(Packet));
			m_SpawnData.m_Data->set_editor_flag(IServerEntity::flVisualChange | IServerEntity::flVisualAnimationChange);
			destroy_entity(tmp);
		}
	}
	else
	{
		m_SpawnData.m_Profile = SectionToEditor(m_SpawnData.m_Data->name());
	}
}

void CSpawnPoint::FillProp(LPCSTR pref, PropItemVec& items)
{
	inherited::FillProp(pref, items);

	if (m_SpawnData.Valid())
	{
		shared_str pref1 = PrepareKey(pref, m_SpawnData.m_Data->name());
		m_SpawnData.m_Profile = SectionToEditor(m_SpawnData.m_Data->name());
		ChooseValue* C = PHelper().CreateChoose(items, PrepareKey(pref1.c_str(), "Profile (spawn section)"),
			&m_SpawnData.m_Profile, smCustom, 0, 0, 1, cfFullExpand);
		C->OnChooseFillEvent.bind(this, &CSpawnPoint::OnFillChooseItems);
		C->OnChangeEvent.bind(this, &CSpawnPoint::OnProfileChange);
		m_SpawnData.FillProp(pref, items);
	}
	else
	{
		switch (m_Type)
		{
		case ptRPoint:
		{
			if (m_RP_Type == rptItemSpawn)
			{
				ChooseValue* C = PHelper().CreateChoose(
					items, PrepareKey(pref, "Respawn Point\\Profile"), &m_rpProfile, smCustom, 0, 0, 10, cfMultiSelect);
				C->OnChooseFillEvent.bind(this, &CSpawnPoint::OnFillRespawnItemProfile);
			}
			else
			{
				PHelper().CreateU8(items, PrepareKey(pref, "Respawn Point\\Team"), &m_RP_TeamID, 0, 7);
			}
			Token8Value* TV =
				PHelper().CreateToken8(items, PrepareKey(pref, "Respawn Point\\Spawn Type"), &m_RP_Type, rpoint_type);
			TV->OnChangeEvent.bind(this, &CSpawnPoint::OnRPointTypeChange);

			m_GameType.FillProp(pref, items);
		}
		break;
		case ptEnvMod:
		{
			PHelper().CreateFloat(
				items, PrepareKey(pref, "Environment Modificator\\Radius"), &m_EM_Radius, EPS_L, 10000.f);
			PHelper().CreateFloat(items, PrepareKey(pref, "Environment Modificator\\Power"), &m_EM_Power, EPS, 1000.f);

			Flag16Value* FV = NULL;

			FV = PHelper().CreateFlag16(
				items, PrepareKey(pref, "Environment Modificator\\View Distance"), &m_EM_Flags, eViewDist);
			FV->OnChangeEvent.bind(this, &CSpawnPoint::OnEnvModFlagChange);
			if (m_EM_Flags.test(eViewDist))
				PHelper().CreateFloat(items, PrepareKey(pref, "Environment Modificator\\View Distance\\ "),
					&m_EM_ViewDist, EPS_L, 10000.f);

			FV = PHelper().CreateFlag16(
				items, PrepareKey(pref, "Environment Modificator\\Fog Color"), &m_EM_Flags, eFogColor);
			FV->OnChangeEvent.bind(this, &CSpawnPoint::OnEnvModFlagChange);
			if (m_EM_Flags.test(eFogColor))
				PHelper().CreateColor(items, PrepareKey(pref, "Environment Modificator\\Fog Color\\ "), &m_EM_FogColor);

			FV = PHelper().CreateFlag16(
				items, PrepareKey(pref, "Environment Modificator\\Fog Density"), &m_EM_Flags, eFogDensity);
			FV->OnChangeEvent.bind(this, &CSpawnPoint::OnEnvModFlagChange);
			if (m_EM_Flags.test(eFogDensity))
				PHelper().CreateFloat(
					items, PrepareKey(pref, "Environment Modificator\\Fog Density\\ "), &m_EM_FogDensity, 0.f, 10000.f);

			FV = PHelper().CreateFlag16(
				items, PrepareKey(pref, "Environment Modificator\\Ambient Color"), &m_EM_Flags, eAmbientColor);
			FV->OnChangeEvent.bind(this, &CSpawnPoint::OnEnvModFlagChange);
			if (m_EM_Flags.test(eAmbientColor))
				PHelper().CreateColor(
					items, PrepareKey(pref, "Environment Modificator\\Ambient Color\\ "), &m_EM_AmbientColor);

			FV = PHelper().CreateFlag16(
				items, PrepareKey(pref, "Environment Modificator\\Sky Color"), &m_EM_Flags, eSkyColor);
			FV->OnChangeEvent.bind(this, &CSpawnPoint::OnEnvModFlagChange);
			if (m_EM_Flags.test(eSkyColor))
				PHelper().CreateColor(items, PrepareKey(pref, "Environment Modificator\\Sky Color\\ "), &m_EM_SkyColor);

			FV = PHelper().CreateFlag16(
				items, PrepareKey(pref, "Environment Modificator\\Hemi Color"), &m_EM_Flags, eHemiColor);
			FV->OnChangeEvent.bind(this, &CSpawnPoint::OnEnvModFlagChange);
			if (m_EM_Flags.test(eHemiColor))
				PHelper().CreateColor(
					items, PrepareKey(pref, "Environment Modificator\\Hemi Color\\ "), &m_EM_HemiColor);
		}
		break;
		default: THROW;
		}
	}
}
		
		*/
	}
}

