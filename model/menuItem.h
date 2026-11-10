#ifndef MENUITEM_H
#define MENUITEM_H

#include <QString>
#include <QtGlobal>

#include <vector>

#include "ingredientrequirement.h"

enum class MenuCategory
{
    Sandwich,
    HotBeverage,
    ColdBeverage
};

class MenuItem
{
public:
    MenuItem(
        int id,
        const QString& name,
        MenuCategory category,
        qint64 priceCents);

    int getId() const;
    QString getName() const;
    MenuCategory getCategory() const;
    qint64 getPriceCents() const;

    void addIngredientRequirement(
        const IngredientRequirement& requirement);

    const std::vector<IngredientRequirement>&
    getIngredientRequirements() const;

private:
    int id;
    QString name;
    MenuCategory category;

    // values are stored in cents to avoid floating point errors
    qint64 priceCents;

    std::vector<IngredientRequirement> ingredientRequirements;
};

// converts a category into text that can be displayed in the gui
QString menuCategoryToString(MenuCategory category);

#endif
