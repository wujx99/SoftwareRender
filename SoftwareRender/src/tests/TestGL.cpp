#include "TestGL.h"
#include "GL.h"

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
		auto ret = GL::BaryCentric(A, B, C, P);
		std::cout << ret.x << " "
			<< ret.y << " "
			<< ret.z << " " << std::endl;
		ASSERT(ret.x == 0.25f && ret.y == 0.25f && ret.z == 0.5f)
	}
}