#ifndef INGREDIENTREQUIREMENT_H
#define INGREDIENTREQUIREMENT_H

#include "ingredient.h"

#include <QString>
#include <memory>

class IngredientRequirement {
public:
  // create a recipe requirement with an ingredient, quantity and unit
  IngredientRequirement(const std::shared_ptr<Ingredient>& ingredient,
                        double quantity, const QString& unit);

  // access the required ingredient information
  std::shared_ptr<Ingredient> getIngredient() const;
  double getQuantity() const;
  QString getUnit() const;

private:
  std::shared_ptr<Ingredient> ingredient;
  double quantity;
  QString unit;
};

#endif
