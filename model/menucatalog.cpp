#include "menucatalog.h"

bool MenuCatalog::addMenuItem(
    const std::shared_ptr<MenuItem>& item)
{
    if (item == nullptr)
    {
        return false;
    }

    // prevent two menu items from using the same id
    if (findById(item->getId()) != nullptr)
    {
        return false;
    }

    menuItems.push_back(item);
    return true;
}

std::shared_ptr<MenuItem>
MenuCatalog::findById(int id) const
{
    for (const std::shared_ptr<MenuItem>& item : menuItems)
    {
        if (item->getId() == id)
        {
            return item;
        }
    }

    return nullptr;
}

std::vector<std::shared_ptr<MenuItem>>
MenuCatalog::getItemsByCategory(MenuCategory category) const
{
    std::vector<std::shared_ptr<MenuItem>> matchingItems;

    for (const std::shared_ptr<MenuItem>& item : menuItems)
    {
        if (item->getCategory() == category)
        {
            matchingItems.push_back(item);
        }
    }

    return matchingItems;
}

const std::vector<std::shared_ptr<MenuItem>>&
MenuCatalog::getAllItems() const
{
    return menuItems;
}
