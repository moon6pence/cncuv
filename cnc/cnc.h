#ifndef CNC_H_
#define CNC_H_

namespace CnC {

// forward declarations
template <typename Derived> 
class Context;

template <typename UserStep>
class StepCollection;

// TagCollection
template <typename Tag>
class TagCollection
{
public:
    template <typename Derived>
    TagCollection(Context<Derived> &context);

    template <typename UserStep>
    void prescribes(const StepCollection<UserStep> &stepCollection);
};

// ItemCollection
template <typename Tag, typename Item>
class ItemCollection
{
public:
    template <typename Derived>
    ItemCollection(Context<Derived> &context);
};

// StepCollection
template <typename UserStep>
class StepCollection
{
public:
    template <typename Derived>
    StepCollection(Context<Derived> &context);

    template <typename Tag, typename Item>
    void consumes(ItemCollection<Tag, Item> &dataCollection);

    template <typename Tag, typename Item>
    void produces(ItemCollection<Tag, Item> &dataCollection);

    template <typename Tag>
    void controls(TagCollection<Tag> &tagCollection);
};

// Context
template <class Derived>
class Context
{
};

}; // namespace CnC

// Implementation
#include "TagCollection.h"
#include "ItemCollection.h"
#include "StepCollection.h"

#endif // CNC_H_