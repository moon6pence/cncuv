#ifndef CNC_H_
#define CNC_H_

#include "StepLauncher.h"

#include <map>
#include <memory>

namespace CnC {

// forward declarations
template <typename Derived> 
class Context;

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

    template <typename Derived, typename Arg>
    StepCollection(Context<Derived> &context, Arg &arg);

    void put(const Tag &tag);

    const UserStep step;

private:
    std::unique_ptr<StepLauncherBase<Tag>> _stepLauncher;
};

// Context
template <class Derived>
class Context
{
};

}; // namespace CnC

// Implementation
#include "ItemCollection.h"
#include "StepCollection.h"

#endif // CNC_H_