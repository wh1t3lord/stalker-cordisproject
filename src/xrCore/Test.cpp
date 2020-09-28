#include "stdafx.h"

#include <src/gtest-all.cc>

#define CurrentTest Test_xrCore

GTEST_TEST(CurrentTest, xr_string) {
	xr_string str;
	str = "data";

	EXPECT_TRUE(str == "data");

	EXPECT_FALSE(str.empty());

	EXPECT_TRUE(str.find('d') != xr_string::npos);

	EXPECT_TRUE(str[0] == 'd');
	EXPECT_TRUE(str[1] == 'a');
	EXPECT_TRUE(str[2] == 't');
	EXPECT_TRUE(str[3] == 'a');

	str.append("_something");

	EXPECT_TRUE(str == "data_something");

	str.clear();

	EXPECT_TRUE(str.empty());
}