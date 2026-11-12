#ifndef MENUITEM_H
#define MENUITEM_H

#include "ingredientRequirement.h"

#include <QString>
#include <vector>

// menu categories
enum class MenuCategory {
  Sandwich,
  HotBeverage,
  ColdBeverage
};

class MenuItem {
public:
  // create a menu item with its id, name, category and price in cents
  MenuItem(int id, const QString& name, MenuCategory category, int priceCents);

  // access menu item information
  int getId() const;
  QString getName() const;
  MenuCategory getCategory() const;
  int getPriceCents() const;

  // add and access the ingredients required to prepare this item
  void addIngredientRequirement(const IngredientRequirement& requirement);
  const std::vector<IngredientRequirement>& getIngredientRequirements() const;

private:
  int id;
  QString name;
  MenuCategory category;
  int priceCents;
  std::vector<IngredientRequirement> ingredientRequirements;
};

#endif