#ifndef STEP_COLLECTION_H_
#define STEP_COLLECTION_H_

namespace CnC {

template <typename Tag, typename UserStep>
template <typename Derived>
StepCollection<Tag, UserStep>::StepCollection(Context<Derived> &context) : 
    step(UserStep())
{
}

template <typename Tag, typename UserStep>
void StepCollection<Tag, UserStep>::put(const Tag &t)
{
}

} // namespace CnC

#endif // STEP_COLLECTION_H_