#include "stdafx.h"
#include "SiniFileStream.h"

namespace Cordis
{
	namespace SDK
	{
		void SIniFileStream::w_float(float a) { this->m_ini->w_float(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_vec3(const Fvector& a) { this->m_ini->w_fvector3(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_vec4(const Fvector4& a) { this->m_ini->w_fvector4(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_u64(u64 a) { this->m_ini->w_u64(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_s64(s64 a) { this->m_ini->w_s64(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_u32(u32 a) { this->m_ini->w_u32(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_s32(s32 a) { this->m_ini->w_s32(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_u16(u16 a) { this->m_ini->w_u16(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_s16(s16 a) { this->m_ini->w_s16(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_u8(u8 a) { this->m_ini->w_u8(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_s8(s8 a) { this->m_ini->w_s8(this->m_section.c_str(), this->generate_name(), a); }
		void SIniFileStream::w_stringZ(LPCSTR S)
		{
			string4096 buff;
			xr_sprintf(buff, sizeof(buff), "\"%s\"", (S) ? S : "");
			this->m_ini->w_string(this->m_section.c_str(), this->generate_name(), buff);
		}

		void SIniFileStream::r_vec3(Fvector& A) { A = this->m_ini->r_fvector3(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_vec4(Fvector4& A) { A = this->m_ini->r_fvector4(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_float(float& A) { A = this->m_ini->r_float(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_u8(u8& A) { A = this->m_ini->r_u8(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_u16(u16& A) { A = this->m_ini->r_u16(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_u32(u32& A) { A = this->m_ini->r_u32(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_u64(u64& A) { A = this->m_ini->r_u64(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_s8(s8& A) { A = this->m_ini->r_s8(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_s16(s16& A) { A = this->m_ini->r_s16(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_s32(s32& A) { A = this->m_ini->r_s32(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_s64(s64& A) { A = this->m_ini->r_s64(this->m_section.c_str(), this->generate_name()); }
		void SIniFileStream::r_string(LPSTR dest, u32 dest_size)
		{
			shared_str S;
			S = this->m_ini->r_string_wb(this->m_section.c_str(), this->generate_name());
			R_ASSERT(dest_size >= S.size());
			xr_strcpy(dest, dest_size, S.c_str());
			//.    Msg("[%s] [%s]=[%s]",this->m_section.c_str(),tmp_buff,dest);
		}

		void SIniFileStream::skip_stringZ() { this->generate_name(); }
	}
}

