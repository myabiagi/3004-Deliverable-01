#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QString>

class Ingredient
{
public:
    Ingredient(int id, const QString& name);

    int getId() const;
    QString getName() const;

private:
    int id;
    QString name;
};

#endif
