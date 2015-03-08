#include <gtest/gtest.h>

#include <cucumber-cpp/internal/utils/ScopedList.hpp>

using namespace cucumber::internal;

class Base
{
public:
    Base() { ++creationCount; }
    virtual ~Base() { ++destructionCount; }
    static int creationCount;
    static int destructionCount;
};

int Base::creationCount = 0;
int Base::destructionCount = 0;

#if 0
class Sub
{
public:
    Sub() { ++creationCount; }
    virtual ~Sub() { ++destructionCount; }
    static int creationCount;
    static int destructionCount;
};

int Sub::creationCount = 0;
int Sub::destructionCount = 0;
#endif

TEST(ScopedListTest, emptyList)
{
    {
        ScopedList<Base> uut;
    }
    ASSERT_EQ(Base::destructionCount, 0);
}

TEST(ScopedListTest, addedObjectAreDestroyed)
{
    {
        ScopedList<Base> uut;
        uut.add(new Base());
    }
    ASSERT_EQ(Base::destructionCount, 1);
}

TEST(ScopedListTest, addedObjectsAreDestroyed)
{
    static const int N_OBJECTS = 5;
    {
        ScopedList<Base> uut;
        for (int i = 0; i < N_OBJECTS; ++i)
        {
            uut.add(new Base());
        }
    }
    ASSERT_EQ(Base::destructionCount, N_OBJECTS);
}

#if 0
TEST(ScopedListTest, addedSubclassObjectsAreDestroyed)
{
    {
        ScopedList<Base> uut;
        uut.add(new Sub());
    }
    ASSERT_EQ(Sub::destructionCount, 1);
}
#endif
