#ifndef CNC_H_
#define CNC_H_

namespace CnC {

// forward declarations
template <typename Derived> 
class Context;

template <typename Tag, typename Item> 
class ItemCollection;

template <typename Tag>
class TagCollection;

// StepCollection
template <typename UserStep>
class StepCollection
{
public:
    template <typename Derived>
    StepCollection(Context<Derived> &context) { }

    template <typename Tag, typename Item>
    void consumes(ItemCollection<Tag, Item> &dataCollection);

    template <typename Tag, typename Item>
    void produces(ItemCollection<Tag, Item> &dataCollection);

    template <typename Tag>
    void controls(TagCollection<Tag> &tagCollection);
};

// TagCollection
template <typename Tag>
class TagCollection
{
public:
    template <typename Derived>
    TagCollection(Context<Derived> &context) { }

    template <typename UserStep>
    void prescribes(const StepCollection<UserStep> &stepCollection);
};

// ItemCollection
template <typename Tag, typename Item>
class ItemCollection
{
public:
    template <typename Derived>
    ItemCollection(Context<Derived> &context) { }
};

// Context
template <class Derived>
class Context
{
};

// Implementation

template <typename UserStep>
template <typename Tag, typename Item>
void StepCollection<UserStep>::consumes(ItemCollection<Tag, Item> &dataCollection)
{
}

template <typename UserStep>
template <typename Tag, typename Item>
void StepCollection<UserStep>::produces(ItemCollection<Tag, Item> &dataCollection)
{
}

template <typename UserStep>
template <typename Tag>
void StepCollection<UserStep>::controls(TagCollection<Tag> &tagCollection)
{
}

template <typename Tag>
template <typename UserStep>
void TagCollection<Tag>::prescribes(const StepCollection<UserStep> &stepCollection)
{
}

}; // namespace CnC

#endif // CNC_H_