#ifndef TAG_COLLECTION_H_
#define TAG_COLLECTION_H_

namespace CnC {

template <typename Tag>
template <typename Derived>
TagCollection<Tag>::TagCollection(Context<Derived> &context) 
{ 
}

template <typename Tag>
template <typename UserStep>
void TagCollection<Tag>::prescribes(const StepCollection<UserStep> &stepCollection)
{
}

template <typename Tag>
void TagCollection<Tag>::put(const Tag &t)
{
}

} // namespace CnC

#endif // TAG_COLLECTION_H_