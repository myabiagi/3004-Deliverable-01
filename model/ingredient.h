#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QString>

class Ingredient {
public:
  // create an ingredient with its unique id and name
  Ingredient(int id, const QString& name);

  // access ingredient information
  int getId() const;
  QString getName() const;

private:
  int id;
  QString name;
};

#endif
