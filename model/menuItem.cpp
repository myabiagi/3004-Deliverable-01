#include "menuItem.h"

MenuItem::MenuItem(int id, const QString& name, MenuCategory category, int priceCents)
  : id(id), name(name), category(category), priceCents(priceCents) {
}

int MenuItem::getId() const { return id; }
QString MenuItem::getName() const { return name; }
MenuCategory MenuItem::getCategory() const { return category; }
int MenuItem::getPriceCents() const { return priceCents; }
const std::vector<IngredientRequirement>& MenuItem::getIngredientRequirements() const {
  return ingredientRequirements;
}

void MenuItem::addIngredientRequirement(const IngredientRequirement& requirement) {
  ingredientRequirements.push_back(requirement);
}