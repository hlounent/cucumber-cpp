#ifndef CUKE_SCOPEDLIST_HPP_
#define CUKE_SCOPEDLIST_HPP_

#include <list>

namespace cucumber {
namespace internal {

/**
Deletes the objects added to the container
*/
template <class ObjectType>
class ScopedList
{
public:
    typedef std::list<ObjectType*> ObjectList;

private:
    ObjectList objects;

public:
    ScopedList();
    ~ScopedList();

    template <typename RealObjectType>
    void add(RealObjectType* object);

};

template <class ObjectType>
inline ScopedList<ObjectType>::ScopedList()
  : objects()
{
}

template <class ObjectType>
inline ScopedList<ObjectType>::~ScopedList()
{
    typename ObjectList::const_iterator it = objects.begin();
    const typename ObjectList::const_iterator end = objects.end();
    for (; it != end; ++it)
    {
        delete *it;
    }
}

template <class ObjectType>
template <typename RealObjectType>
inline void ScopedList<ObjectType>::add(RealObjectType* object)
{
    objects.push_back(object);
}

} // namespace internal
} // namespace cucumber

#endif // CUKE_SCOPEDLIST_HPP_
