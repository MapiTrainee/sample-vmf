#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../src/vmf.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestSampleVMF
{
	TEST_CLASS(UnitTestSampleVMF)
	{
	public:

		TEST_METHOD(TestFilterVMF)
		{
			Assert::IsTrue(1==1);
		}

	};
}