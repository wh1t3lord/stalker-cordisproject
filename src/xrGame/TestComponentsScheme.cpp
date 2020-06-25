#include "stdafx.h"

#include <src\gtest-all.cc>

#define CurrentTest TestComponentsScheme
using namespace Cordis::Scripts::DataBase;
GTEST_TEST(CurrentTest, TestComponent_SRIdle) {
	Script_ComponentScheme_SRIdle instance;
	instance.setActionSchemeID(15);
	CGameObject obj;
	CScriptGameObject object(&obj);
	instance.setClientObject(&object);

	instance.clear();
}