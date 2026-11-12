#include "ingredientRequirement.h"

IngredientRequirement::IngredientRequirement(const std::shared_ptr<Ingredient>& ingredient,
                                             double quantity, const QString& unit)
  : ingredient(ingredient),
  quantity(quantity), unit(unit) {
}

std::shared_ptr<Ingredient> IngredientRequirement::getIngredient() const { return ingredient; }
double IngredientRequirement::getQuantity() const { return quantity; }
QString IngredientRequirement::getUnit() const { return unit; }