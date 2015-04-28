#ifndef STEP_COLLECTION_H_
#define STEP_COLLECTION_H_

namespace CnC {

template <typename Tag, typename UserStep>
template <typename Derived>
StepCollection<Tag, UserStep>::StepCollection(Context<Derived> &context) : 
    step(UserStep())
{
    auto *stepLauncher = new StepLauncherAsync<Tag, UserStep, Derived>(*this, static_cast<Derived &>(context));
    _stepLauncher.reset(stepLauncher);
}

template <typename Tag, typename UserStep>
void StepCollection<Tag, UserStep>::put(const Tag &tag)
{
    _stepLauncher->executeStep(tag);
}

} // namespace CnC

#endif // STEP_COLLECTION_H_