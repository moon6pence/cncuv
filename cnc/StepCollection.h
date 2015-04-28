#ifndef STEP_COLLECTION_H_
#define STEP_COLLECTION_H_

namespace CnC {

template <typename UserStep>
template <typename Derived>
StepCollection<UserStep>::StepCollection(Context<Derived> &context) : 
    step(UserStep())
{
}

} // namespace CnC

#endif // STEP_COLLECTION_H_