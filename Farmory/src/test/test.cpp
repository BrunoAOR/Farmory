#include <maz/globals.h>
#include <maz/CDelegate.h>
#include <deprecated/CDelegateOld.h>


namespace maz
{

struct CDelegateTest
{
    CDelegateTest(int a) : mMult(a) { ; }

    int myFunc(float aF)
    {
        return static_cast<int>(aF) * mMult;
    }

    int myFunc(float aF) const
    {
        return static_cast<int>(aF) * mMult * 1000;
    }

    int myConstFunc(float aF) const
    {
        return static_cast<int>(aF) * 2 * mMult;
    }

    static int myStaticFunc(float aF)
    {
        return static_cast<int>(aF) * 100;
    }

    int mMult;
};


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


CDelegate< int, float > GetDel(const CDelegateTest* apObj)
{
    return CDelegate< int, float >::Create< CDelegateTest, &CDelegateTest::myFunc >(apObj);
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
    MAZ_LOGGER("uint8  max: %hhu",  MAX_UINT8);
    MAZ_LOGGER(" int8  min: %hhd",  MIN_INT8);
    MAZ_LOGGER(" int8  max: %hhd",  MAX_INT8);
    MAZ_LOGGER("uint16 max: %hu",   MAX_UINT16);
    MAZ_LOGGER(" int16 min: %hd",   MIN_INT16);
    MAZ_LOGGER(" int16 max: %hd",   MAX_INT16);
    MAZ_LOGGER("uint32 max: %u",    MAX_UINT32);
    MAZ_LOGGER(" int32 min: %d",    MIN_INT32);
    MAZ_LOGGER(" int32 max: %d",    MAX_INT32);
    MAZ_LOGGER("uint64 max: %llu",  MAX_UINT64);
    MAZ_LOGGER(" int64 min: %lld",  MIN_INT64);
    MAZ_LOGGER(" int64 max: %lld",  MAX_INT64);
    MAZ_LOGGER(" float max: %f",    MAX_FLOAT);

    {
        CDelegateTest l5(5);
        CDelegateTest l10(10);

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
        //CDelegateHolder< int, float > lDel5 = BindMethod(&l5, &CDelegateTest::myFunc);
        TMyOldDel lDel1(&l10, &CDelegateTest::myFunc);
        TMyOldDel lDel2(test);
        //TMyOldDel lDel3(&l10, &CDelegateTest::myConstFunc);
        TMyOldDel lDel4(&CDelegateTest::myStaticFunc);

        int lRes1 = lDel1(1.5);
        int lRes2 = lDel2(1.5);
        //int lRes3 = lDel3(1.5);
        int lRes4 = lDel4(1.5);
        MAZ_UNUSED_VAR(lRes1);
        MAZ_UNUSED_VAR(lRes2);
        //MAZ_UNUSED_VAR(lRes3);
        MAZ_UNUSED_VAR(lRes4);

        using TFunc = int(CDelegateTest::*)(float);
        TFunc lF = &CDelegateTest::myFunc;
        int lFuncRes = ((*(&l5)).*lF)(5.0f);
        MAZ_UNUSED_VAR(lFuncRes);

        TMyNewDel lNDel1 = TMyNewDel::Create< CDelegateTest, &CDelegateTest::myFunc >(&l10);
        TMyNewDel lNDel2 = TMyNewDel::Create< &test >();
        TMyNewDel lNDel3 = TMyNewDel::CreateConst< CDelegateTest, &CDelegateTest::myConstFunc >(&l10);
        TMyNewDel lNDel4 = TMyNewDel::Create< &CDelegateTest::myStaticFunc >();
        TMyNewDel lNDel5 = TMyNewDel::CreateConst< CDelegateTest, &CDelegateTest::myFunc >(&l10);
        TMyNewDel lNDel6 = GetDel(&l10);

        int lNewRes1 = lNDel1(1.5);
        int lNewRes2 = lNDel2(1.5);
        int lNewRes3 = lNDel3(1.5);
        int lNewRes4 = lNDel4(1.5);
        int lNewRes5 = lNDel5(1.5);
        int lNewRes6 = lNDel6(1.5);
        MAZ_UNUSED_VAR(lNewRes1);
        MAZ_UNUSED_VAR(lNewRes2);
        MAZ_UNUSED_VAR(lNewRes3);
        MAZ_UNUSED_VAR(lNewRes4);
        MAZ_UNUSED_VAR(lNewRes5);
        MAZ_UNUSED_VAR(lNewRes6);

        TMyNewDel lNDelX1 = TMyNewDel::Create< CDelegateTest, &CDelegateTest::myFunc >(&l5);
        TMyNewDel lNDelX2 = TMyNewDel::Create< &test >();
        TMyNewDel lNDelX3 = TMyNewDel::CreateConst< CDelegateTest, &CDelegateTest::myConstFunc >(&l5);
        TMyNewDel lNDelX4 = TMyNewDel::Create< &CDelegateTest::myStaticFunc >();
        TMyNewDel lNDelX5 = TMyNewDel::CreateConst< CDelegateTest, &CDelegateTest::myFunc >(&l5);

        bool lCmp01 = lNDel1 == lNDel1;
        bool lCmp02 = lNDel2 == lNDel2;
        bool lCmp03 = lNDel3 == lNDel3;
        bool lCmp04 = lNDel4 == lNDel4;
        bool lCmp05 = lNDel1 == lNDelX1;
        bool lCmp06 = lNDel2 == lNDelX2;
        bool lCmp07 = lNDel3 == lNDelX3;
        bool lCmp08 = lNDel4 == lNDelX4;
        bool lCmp09 = lNDel1 == lNDel3;
        bool lCmp10 = lNDel1 == lNDel5;
        bool lCmp11 = lNDel1 == lNDel6;
        MAZ_UNUSED_VAR(lCmp01);
        MAZ_UNUSED_VAR(lCmp02);
        MAZ_UNUSED_VAR(lCmp03);
        MAZ_UNUSED_VAR(lCmp04);
        MAZ_UNUSED_VAR(lCmp05);
        MAZ_UNUSED_VAR(lCmp06);
        MAZ_UNUSED_VAR(lCmp07);
        MAZ_UNUSED_VAR(lCmp08);
        MAZ_UNUSED_VAR(lCmp09);
        MAZ_UNUSED_VAR(lCmp10);
        MAZ_UNUSED_VAR(lCmp11);
    }
}

} // maz