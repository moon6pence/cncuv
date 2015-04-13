#ifndef ITEM_COLLECTION_H_
#define ITEM_COLLECTION_H_

namespace CnC {

class cannot_find_item : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Cannot find item in item collection";
    }
};

template <typename Tag, typename Item>
void ItemCollection<Tag, Item>::put(const Tag &tag, const Item &item)
{
    _map[tag] = item;
}

template <typename Tag, typename Item>
void ItemCollection<Tag, Item>::get(const Tag &tag, Item &item) const
{
    if (_map.find(tag) != _map.end())
    {
        item = _map.at(tag);
    }
    else
    {
        //std::cerr << "Cannot find item with tag: " << tag << std::endl;

        //for (auto i = _map.begin(); i != _map.end(); ++i)
        //    std::cerr << i->first << " -> " << i->second << std::endl;

        // throw exception
        throw cannot_find_item();
    }
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