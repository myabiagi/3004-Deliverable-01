#ifndef DEFAULTDATALOADER_H
#define DEFAULTDATALOADER_H

class UserDirectory;
class MenuCatalog;
class OrderRepository;

class DefaultDataLoader {
public:
  // create and load the default data when the application starts
  bool loadDefaults(UserDirectory& users, MenuCatalog& menu, OrderRepository& orders) const;

private:
  // create the default customer and staff accounts
  bool loadUsers(UserDirectory& users) const;

  // create the default menu items and their recipes
  bool loadMenu(MenuCatalog& menu) const;

  // create five historical orders for each customer
  bool loadOrderHistory(UserDirectory& users, MenuCatalog& menu, OrderRepository& orders) const;
};

#endif
