#ifndef ITEM_COLLECTION_H_
#define ITEM_COLLECTION_H_

namespace CnC {

template <typename Tag, typename Item>
template <typename Derived>
ItemCollection<Tag, Item>::ItemCollection(Context<Derived> &context) 
{ 
}

template <typename Tag, typename Item>
void ItemCollection<Tag, Item>::put(const Tag &tag, const Item &item)
{
}

template <typename Tag, typename Item>
void ItemCollection<Tag, Item>::get(const Tag &tag, Item &item) const
{
}

} // namespace CnC

#endif // ITEM_COLLECTION_H_