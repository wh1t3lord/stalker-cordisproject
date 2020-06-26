#include "StdAfx.h"
#include "dxUIShader.h"

tbb::spin_mutex _spin_create_sheader;

void dxUIShader::Copy(IUIShader& _in) { *this = *((dxUIShader*)&_in); }
void dxUIShader::create(LPCSTR sh, LPCSTR tex) 
{
	tbb::spin_mutex::scoped_lock mutex{_spin_create_sheader};
	hShader.create(sh, tex); 
}

void dxUIShader::destroy() { hShader.destroy(); }
