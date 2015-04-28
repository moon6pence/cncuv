#ifndef CNC_H_
#define CNC_H_

#include "StepLauncher.h"

#include <map>
#include <memory>

namespace CnC {

// forward declarations
template <typename Derived> 
class Context;

template <typename Tag, typename UserStep>
class StepCollection;

// TagCollection
template <typename Tag>
class TagCollection
{
public:
    template <typename Derived>
    TagCollection(Context<Derived> &context);

    template <typename UserStep, typename Arg>
    void prescribes(const StepCollection<Tag, UserStep> &stepCollection, Arg &arg);

    void put(const Tag &t);

private:
    std::unique_ptr<StepLauncherBase<Tag>> _stepLauncher;
};

// ItemCollection
template <typename Tag, typename Item>
class ItemCollection
{
public:
    void put(const Tag &tag, const Item &item);
    void get(const Tag &tag, Item &item) const;

    bool empty() const;
    size_t size() const;

private:
    std::map<Tag, Item> _map;
};

// StepCollection
template <typename Tag, typename UserStep>
class StepCollection
{
public:
    template <typename Derived>
    StepCollection(Context<Derived> &context);

    void put(const Tag &t);

    const UserStep step;
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