#ifndef STEP_COLLECTION_H_
#define STEP_COLLECTION_H_

namespace CnC {

template <typename UserStep>
template <typename Derived>
StepCollection<UserStep>::StepCollection(Context<Derived> &context)
{
}

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

} // namespace CnC

#endif // STEP_COLLECTION_H_