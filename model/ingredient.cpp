#include "ingredient.h"

// Constructor for ingredient
Ingredient::Ingredient(int id, const QString& name)
    : id(id),
      name(name)
{
}

// Get ingredent ID
int Ingredient::getId() const
{
    return id;
}

// Get its name
QString Ingredient::getName() const
{
    return name;
}
