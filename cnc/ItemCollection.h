#ifndef ITEM_COLLECTION_H_
#define ITEM_COLLECTION_H_

namespace CnC {

template <typename Tag, typename Item>
void ItemCollection<Tag, Item>::put(const Tag &tag, const Item &item)
{
    _map[tag] = item;
}

template <typename Tag, typename Item>
void ItemCollection<Tag, Item>::get(const Tag &tag, Item &item) const
{
    item = _map.at(tag);
}

template <typename Tag, typename Item>
inline bool ItemCollection<Tag, Item>::empty() const
{
    return size() == 0;
}

template <typename Tag, typename Item>
inline size_t ItemCollection<Tag, Item>::size() const
{
    return _map.size();
}

} // namespace CnC

#endif // ITEM_COLLECTION_H_