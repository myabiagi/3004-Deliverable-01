#include "menuCatalog.h"

bool MenuCatalog::addMenuItem(const std::shared_ptr<MenuItem>& item) {
  if (item == nullptr || findById(item->getId()) != nullptr) {
    return false;
  }

  // store a valid menu item only if its id is not already used
  menuItems.push_back(item);
  return true;
}

std::shared_ptr<MenuItem> MenuCatalog::findById(int id) const {
  for (const std::shared_ptr<MenuItem>& item : menuItems) {
    if (item->getId() == id) {
      return item;
    }
  }

  return nullptr;
}

std::vector<std::shared_ptr<MenuItem>> MenuCatalog::getItemsByCategory(MenuCategory category) const {
  std::vector<std::shared_ptr<MenuItem>> matchingItems;

  for (const std::shared_ptr<MenuItem>& item : menuItems) {
    if (item->getCategory() == category) {
      matchingItems.push_back(item);
    }
  }

  return matchingItems;
}

const std::vector<std::shared_ptr<MenuItem>>& MenuCatalog::getAllItems() const {
  return menuItems;
}
