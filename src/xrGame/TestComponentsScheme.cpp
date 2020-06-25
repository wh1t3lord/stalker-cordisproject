#include "stdafx.h"

#include <src\gtest-all.cc>

#define CurrentTest TestComponentsScheme
using namespace Cordis;
using namespace Cordis::Scripts;
using namespace Cordis::Scripts::DataBase;

GTEST_TEST(CurrentTest, TestComponent_SRIdle_SetGet) {
	Script_ComponentScheme_SRIdle instance;
	instance.setActionSchemeID(15);
	CGameObject obj;
	CScriptGameObject object(&obj);
	instance.setClientObject(&object);
	instance.setEnabled(true);
	instance.setLogic({LogicData(), LogicData()});
	instance.setLogicName("test");
	instance.setSchemeName("scheme");
	instance.setSignals("test", true);


	EXPECT_TRUE(instance.getSignals().at("test") == true);
	EXPECT_TRUE(instance.getSchemeName() == "scheme");
	EXPECT_TRUE(instance.getLogicName() == "test");
	EXPECT_TRUE(instance.getLogic().size() == 2);
	EXPECT_TRUE(instance.IsEnabled());
	EXPECT_FALSE(instance.isCalledClear());
	EXPECT_TRUE(instance.getClientObject());
}

GTEST_TEST(CurrentTest, Test_CGameObject) {
	CGameObject instance;
}