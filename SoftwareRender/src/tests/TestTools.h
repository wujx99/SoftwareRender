#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();

namespace test
{
	class TestTools
	{
	public:
		TestTools();
		~TestTools();

		static void TestBaryCentric();

	};

	
}