#include "ingredientrequirement.h"

IngredientRequirement::IngredientRequirement(
    const std::shared_ptr<Ingredient>& ingredient,
    double quantity,
    const QString& unit)
    : ingredient(ingredient),
      quantity(quantity),
      unit(unit)
{
}

// get ingredients
std::shared_ptr<Ingredient>
IngredientRequirement::getIngredient() const
{
    return ingredient;
}

// get quantity
double IngredientRequirement::getQuantity() const
{
    return quantity;
}

// get unit (measuremnt unit we measure quantity with)
QString IngredientRequirement::getUnit() const
{
    return unit;
}
