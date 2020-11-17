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

GTEST_TEST(CurrentTest, Test_CScriptGameObject) {
	CGameObject owner;
	CScriptGameObject instance(&owner);
}

/// <summary>
/// Here we test Script_ComponentScheme_SRIdle clear method
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
GTEST_TEST(CurrentTest, Test_ClearMethodCorrectness_SRIdle) {
	Script_ComponentScheme_SRIdle instance;
	instance.clear();
}

/// <summary>
/// Here we test Script_IComponentScheme clear method
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
GTEST_TEST(CurrentTest, Test_ClearMethodCorrectness_Script_InterfaceComponentSheme) {
	Script_IComponentScheme instance;
	instance.setActionSchemeID(1);
	instance.setSignals("test", true);

	CScriptIniFile ini("", false, false, false);
	instance.setIni(&ini);
	instance.setSchemeName("scheme");
	instance.setLogicName("logic_name");
	instance.setEnabled(true);

	CGameObject obj_game;
	CScriptGameObject obj(&obj_game);
	instance.setClientObject(&obj);

	LogicData element;
	xr_vector<LogicData> logic;
	logic.push_back(element);
	instance.setLogic(logic);
	instance.addAction(new Script_ISchemeEntity(nullptr, nullptr));

	instance.clear();

	EXPECT_TRUE(instance.IsEnabled() == false);
	EXPECT_TRUE(instance.getSchemeName().empty());
	EXPECT_TRUE(instance.getIni() == nullptr);
	EXPECT_TRUE(instance.getClientObject() == nullptr);
	EXPECT_TRUE(instance.getSignals().empty());
	EXPECT_TRUE(instance.getLogic().empty());
	EXPECT_TRUE(instance.getLogicName().empty());
	EXPECT_TRUE(instance.isCalledClear() == true);
	EXPECT_TRUE(instance.getActions().empty());
}

/// <summary>
/// Here we test Script_ComponentScheme_SRLight clear method
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
GTEST_TEST(CurrentTest, Test_ClearMethodCorrectness_SRLight) {
	Script_ComponentScheme_SRLight instance;
	instance.setLight(true);

	instance.clear();
	EXPECT_FALSE(instance.isLight());
}

/// <summary>
/// Here we test Script_ComponentScheme_SRParticle clear method
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
GTEST_TEST(CurrentTest, Test_ClearMethodCorrectness_SRParticle) {
	Script_ComponentScheme_SRParticle instance;

	instance.setMode(12);
	instance.setName("test");
	instance.setPathName("heh");
	instance.setLooped(true);

	instance.clear();

	EXPECT_TRUE(instance.getMode() == 0);
	EXPECT_TRUE(instance.getName().empty());
	EXPECT_TRUE(instance.getPathName().empty());
	EXPECT_FALSE(instance.isLooped());
}

/// <summary>
/// Here we test Script_ComponentScheme_SRPostProcess clear method
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
GTEST_TEST(CurrentTest, Test_ClearMethodCorrectness_SRPostProcess) {
	Script_ComponentScheme_SRPostProcess instance;

	instance.setIntensity(std::numeric_limits<float>::max());
	instance.setIntensitySpeed(std::numeric_limits<float>::max());
	instance.setHitIntensity(std::numeric_limits<float>::max());

	instance.clear();

	EXPECT_TRUE(fis_zero(instance.getIntensity()));
	EXPECT_TRUE(fis_zero(instance.getIntensitySpeed()));
	EXPECT_TRUE(fis_zero(instance.getHitIntensity()));
}

/// <summary>
/// Here we test Script_ComponentScheme_SRPsyAntenna clear method
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
GTEST_TEST(CurrentTest, Test_ClearMethodCorrectness_SRPsyAntenna) {
	Script_ComponentScheme_SRPsyAntenna instance;

	float _max_float = std::numeric_limits<float>::max();
	instance.setIntensity(_max_float);
	instance.setPhantomProbability(_max_float);
	instance.setHitIntensity(_max_float);
	instance.setMuteSoundThreshold(_max_float);
	instance.setHitFrequency(_max_float);
	instance.setNoStatic(true);
	instance.setNoMumble(true);
	instance.setHitTypeName("test");
	instance.setPostProcessName("process");

	instance.clear();

	EXPECT_TRUE(fis_zero(instance.getIntensity()));
	EXPECT_TRUE(fis_zero(instance.getPhantomProbability()));
	EXPECT_TRUE(fis_zero(instance.getMuteSoundThreshold()));
	EXPECT_TRUE(fis_zero(instance.getHitFrequency()));
	EXPECT_TRUE(fis_zero(instance.getHitIntensity()));
	EXPECT_FALSE(instance.IsNoStatic());
	EXPECT_FALSE(instance.IsNoMumble());
	EXPECT_TRUE(instance.getHitTypeName().empty());
	EXPECT_TRUE(instance.getPostProcessName().empty());
}