#ifndef INGREDIENTREQUIREMENT_H
#define INGREDIENTREQUIREMENT_H

#include <QString>
#include <memory>

#include "ingredient.h"

class IngredientRequirement
{
public:
    IngredientRequirement(
        const std::shared_ptr<Ingredient>& ingredient,
        double quantity,
        const QString& unit);

    std::shared_ptr<Ingredient> getIngredient() const;
    double getQuantity() const;
    QString getUnit() const;

private:
    std::shared_ptr<Ingredient> ingredient;
    double quantity;
    QString unit;
};

#endif
