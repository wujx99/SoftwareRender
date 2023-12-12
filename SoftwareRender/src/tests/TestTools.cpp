#include "TestTools.h"
#include "Tools.h"

namespace test
{
	TestTools::TestTools()
	{
	}

	TestTools::~TestTools()
	{
	}
	void TestTools::TestBaryCentric()
	{
		Vec3f A{ 1.0, 1.0, 0.0 }, B{ 3.0, 1.0, 0.0 }, C{ 2.0, 3.0, 0.0 }, P{ 2.0, 2.0, 0.0 };
		auto ret = Tools::BaryCentric(A, B, C, P);
		std::cout << ret.x << " "
			<< ret.y << " "
			<< ret.z << " " << std::endl;
		ASSERT(ret.x == 0.25f && ret.y == 0.25f && ret.z == 0.5f)
	}
}