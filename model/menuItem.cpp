#include "menuItem.h"

MenuItem::MenuItem(
    int id,
    const QString& name,
    MenuCategory category,
    qint64 priceCents)
    : id(id),
      name(name),
      category(category),
      priceCents(priceCents)
{
}

int MenuItem::getId() const
{
    return id;
}

QString MenuItem::getName() const
{
    return name;
}

MenuCategory MenuItem::getCategory() const
{
    return category;
}

qint64 MenuItem::getPriceCents() const
{
    return priceCents;
}

void MenuItem::addIngredientRequirement(
    const IngredientRequirement& requirement)
{
    ingredientRequirements.push_back(requirement);
}

const std::vector<IngredientRequirement>&
MenuItem::getIngredientRequirements() const
{
    // return a const reference so the collection is not unnecessarily copied
    return ingredientRequirements;
}

QString menuCategoryToString(MenuCategory category)
{
    switch (category)
    {
    case MenuCategory::Sandwich:
        return "Sandwich";

    case MenuCategory::HotBeverage:
        return "Hot Beverage";

    case MenuCategory::ColdBeverage:
        return "Cold Beverage";
    }

    return "Unknown";
}
