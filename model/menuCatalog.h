#ifndef MENUCATALOG_H
#define MENUCATALOG_H

#include "menuItem.h"

#include <memory>
#include <vector>

class MenuCatalog {
public:
  // add a menu item if its id is unique
  bool addMenuItem(const std::shared_ptr<MenuItem>& item);

  // find a menu item by its unique id
  std::shared_ptr<MenuItem> findById(int id) const;

  // retrieve menu items belonging to a category
  std::vector<std::shared_ptr<MenuItem>> getItemsByCategory(MenuCategory category) const;

  // access all menu items without copying the collection
  const std::vector<std::shared_ptr<MenuItem>>& getAllItems() const;

private:
  std::vector<std::shared_ptr<MenuItem>> menuItems;
};

#endif
