#include "TestGL.h"
#include "Tools.h"

namespace test
{
	TestGL::TestGL()
	{
	}

	TestGL::~TestGL()
	{
	}
	void TestGL::TestBaryCentric()
	{
		Vec3f A{ 1.0, 1.0, 0.0 }, B{ 3.0, 1.0, 0.0 }, C{ 2.0, 3.0, 0.0 }, P{ 2.0, 2.0, 0.0 };
		auto ret = Tools::BaryCentric(A, B, C, P);
		std::cout << ret.x << " "
			<< ret.y << " "
			<< ret.z << " " << std::endl;
		ASSERT(ret.x == 0.25f && ret.y == 0.25f && ret.z == 0.5f)
	}

	void TestGL::TestULooKUp()
	{
		Vec3f center{ 0.f, 0.f, .5f }, eye{ 0.f, 0.f, 1.f }, up{ 0.f, 1.f, 0.f };
		Vec3f position{ 0.f, 0.f, 5.f };
		auto transform = Tools::ULookUp(center, eye, up);
		auto tPos = transform * Tools::V3f_to_V4f(position, 1);
		std::cout << "transformed Positon:" << tPos << std::endl;
	}
}