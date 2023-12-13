#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();

namespace test
{
	class TestGL
	{
	public:
		TestGL();
		~TestGL();

		static void TestBaryCentric();

	};

	
}