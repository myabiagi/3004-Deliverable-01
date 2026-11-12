#include "ingredient.h"

Ingredient::Ingredient(int id, const QString& name)
  : id(id), name(name) {
}

int Ingredient::getId() const { return id; }

QString Ingredient::getName() const { return name; }
