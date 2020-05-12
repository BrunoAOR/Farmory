#include <maz/globals.h>
#include <maz/CDelegate.h>
#include <deprecated/CDelegateOld.h>

namespace maz
{

int test(float aF)
{
    return static_cast<int>(aF);
}

float test(float aF, int aI)
{
    return aI * aF;
}

float test()
{
    return 17.45f;
}

CDelegate< int, float > GetDel(const TTest* aObj)
{
    return CDelegate< int, float >::Create< TTest, &TTest::myFunc >(aObj);
}

void TestMain()
{
    MAZ_LOGGER("char          : %u", sizeof(char));
    MAZ_LOGGER("short         : %u", sizeof(short));
    MAZ_LOGGER("int           : %u", sizeof(int));
    MAZ_LOGGER("uint          : %u", sizeof(unsigned int));
    MAZ_LOGGER("long int      : %u", sizeof(long int));
    MAZ_LOGGER("long uint     : %u", sizeof(unsigned long int));
    MAZ_LOGGER("long long int : %u", sizeof(long long int));
    MAZ_LOGGER("long long uint: %u", sizeof(unsigned long long int));
    MAZ_LOGGER("uint8  max:  %hhu", MAX_UINT8);
    MAZ_LOGGER(" int8  min: %hhd", MIN_INT8);
    MAZ_LOGGER(" int8  max:  %hhd", MAX_INT8);
    MAZ_LOGGER("uint16 max:  %hu", MAX_UINT16);
    MAZ_LOGGER(" int16 min: %hd", MIN_INT16);
    MAZ_LOGGER(" int16 max:  %hd", MAX_INT16);
    MAZ_LOGGER("uint32 max:  %u", MAX_UINT32);
    MAZ_LOGGER(" int32 min: %d", MIN_INT32);
    MAZ_LOGGER(" int32 max:  %d", MAX_INT32);
    MAZ_LOGGER("uint64 max:  %llu", MAX_UINT64);
    MAZ_LOGGER(" int64 min: %lld", MIN_INT64);
    MAZ_LOGGER(" int64 max:  %lld", MAX_INT64);
    MAZ_LOGGER(" float max:  %f", MAX_FLOAT);

    {
        TTest l5(5);
        TTest l10(10);

        using TMyOldDel = CDelegateHolder< int, float >;
        using TMyNewDel = CDelegate< int, float >;
        using TTestDel = CDelegate< float, float, int >;
        using TEmptyDel = CDelegate< float >;
        TTestDel lHopeful = TTestDel::Create< &test >();
        float lHopefulRes = lHopeful(1.5, 3);
        MAZ_UNUSED_VAR(lHopefulRes);
        TEmptyDel lEmpty = TEmptyDel::Create< &test >();
        float lEmptyRes = lEmpty();
        MAZ_UNUSED_VAR(lEmptyRes);

        //CDelegateHolder< int, float > lDelx;
        //CDelegateHolder< int, float > lDel5 = BindMethod(&l5, &TTest::myFunc);
        TMyOldDel lDel1(&l10, &TTest::myFunc);
        TMyOldDel lDel2(test);
        //TMyOldDel lDel3(&l10, &TTest::myConstFunc);
        TMyOldDel lDel4(&TTest::myStaticFunc);

        int i1 = lDel1(1.5);
        int i2 = lDel2(1.5);
        //int i3 = lDel3(1.5);
        int i4 = lDel4(1.5);
        MAZ_UNUSED_VAR(i1);
        MAZ_UNUSED_VAR(i2);
        //MAZ_UNUSED_VAR(i3);
        MAZ_UNUSED_VAR(i4);

        using TFunc = int(TTest::*)(float);
        TFunc lF = &TTest::myFunc;
        int asd = ((*(&l5)).*lF)(5.0f);
        MAZ_UNUSED_VAR(asd);

        TMyNewDel lNDel1 = TMyNewDel::Create< TTest, &TTest::myFunc >(&l10);
        TMyNewDel lNDel2 = TMyNewDel::Create< &test >();
        TMyNewDel lNDel3 = TMyNewDel::CreateConst< TTest, &TTest::myConstFunc >(&l10);
        TMyNewDel lNDel4 = TMyNewDel::Create< &TTest::myStaticFunc >();
        TMyNewDel lNDel5 = TMyNewDel::CreateConst< TTest, &TTest::myFunc >(&l10);
        TMyNewDel lNDel6 = GetDel(&l10);

        int in1 = lNDel1(1.5);
        int in2 = lNDel2(1.5);
        int in3 = lNDel3(1.5);
        int in4 = lNDel4(1.5);
        int in5 = lNDel5(1.5);
        int in6 = lNDel6(1.5);
        MAZ_UNUSED_VAR(in1);
        MAZ_UNUSED_VAR(in2);
        MAZ_UNUSED_VAR(in3);
        MAZ_UNUSED_VAR(in4);
        MAZ_UNUSED_VAR(in5);
        MAZ_UNUSED_VAR(in6);

        TMyNewDel lNDelX1 = TMyNewDel::Create< TTest, &TTest::myFunc >(&l5);
        TMyNewDel lNDelX2 = TMyNewDel::Create< &test >();
        TMyNewDel lNDelX3 = TMyNewDel::CreateConst< TTest, &TTest::myConstFunc >(&l5);
        TMyNewDel lNDelX4 = TMyNewDel::Create< &TTest::myStaticFunc >();
        TMyNewDel lNDelX5 = TMyNewDel::CreateConst< TTest, &TTest::myFunc >(&l5);

        bool cmp01 = lNDel1 == lNDel1;
        bool cmp02 = lNDel2 == lNDel2;
        bool cmp03 = lNDel3 == lNDel3;
        bool cmp04 = lNDel4 == lNDel4;
        bool cmp05 = lNDel1 == lNDelX1;
        bool cmp06 = lNDel2 == lNDelX2;
        bool cmp07 = lNDel3 == lNDelX3;
        bool cmp08 = lNDel4 == lNDelX4;
        bool cmp09 = lNDel1 == lNDel3;
        bool cmp10 = lNDel1 == lNDel5;
        bool cmp11 = lNDel1 == lNDel6;
        MAZ_UNUSED_VAR(cmp01);
        MAZ_UNUSED_VAR(cmp02);
        MAZ_UNUSED_VAR(cmp03);
        MAZ_UNUSED_VAR(cmp04);
        MAZ_UNUSED_VAR(cmp05);
        MAZ_UNUSED_VAR(cmp06);
        MAZ_UNUSED_VAR(cmp07);
        MAZ_UNUSED_VAR(cmp08);
        MAZ_UNUSED_VAR(cmp09);
        MAZ_UNUSED_VAR(cmp10);
        MAZ_UNUSED_VAR(cmp11);
    }
}


} // maz