#ifndef STEP_COLLECTION_H_
#define STEP_COLLECTION_H_

namespace CnC {

template <typename UserStep>
template <typename Derived>
StepCollection<UserStep>::StepCollection(Context<Derived> &context) : 
    _step(UserStep())
{
}

template <typename UserStep>
template <typename Tag, typename Item>
void StepCollection<UserStep>::consumes(ItemCollection<Tag, Item> &dataCollection)
{
    // currently doing nothing
}

template <typename UserStep>
template <typename Tag, typename Item>
void StepCollection<UserStep>::produces(ItemCollection<Tag, Item> &dataCollection)
{
    // currently doing nothing
}

template <typename UserStep>
template <typename Tag>
void StepCollection<UserStep>::controls(TagCollection<Tag> &tagCollection)
{
    // currently doing nothing
}

} // namespace CnC

#endif // STEP_COLLECTION_H_