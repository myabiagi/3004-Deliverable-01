#ifndef VIEWFORMATTING_H
#define VIEWFORMATTING_H

#include "menuItem.h"
#include "order.h"
#include "user.h"

#include <QString>
#include <QStringList>
#include <QtGlobal>

// formatting helpers shared by the views
namespace ViewFormatting {

// convert cents into a dollar amount for display
inline QString money(int cents) {
  const QString sign = cents < 0 ? QStringLiteral("-") : QString();
  const int absoluteCents = qAbs(cents);

  return QStringLiteral("%1$%2.%3")
    .arg(sign)
    .arg(absoluteCents / 100)
    .arg(absoluteCents % 100, 2, 10, QLatin1Char('0'));
}

// convert menu categories into display labels
inline QString categoryName(MenuCategory category) {
  switch (category) {
    case MenuCategory::Sandwich:
      return QStringLiteral("Sandwiches");

    case MenuCategory::HotBeverage:
      return QStringLiteral("Hot Beverages");

    case MenuCategory::ColdBeverage:
      return QStringLiteral("Cold Beverages");
  }

  return QString();
}

// convert user roles into display labels
inline QString roleName(UserRole role) {
  switch (role) {
    case UserRole::Customer:
      return QStringLiteral("Customer");

    case UserRole::KitchenStaff:
      return QStringLiteral("Kitchen Staff");

    case UserRole::Cashier:
      return QStringLiteral("Cashier");

    case UserRole::Manager:
      return QStringLiteral("Manager");

    case UserRole::Administrator:
      return QStringLiteral("System Administrator");
  }

  return QString();
}

// format ingredient quantities such as 2, 0.5 or 250
inline QString quantity(double value) {
  return QString::number(value, 'g', 6);
}

// create a readable summary of an order's purchased items
inline QString orderItemsSummary(const Order& order) {
  QStringList parts;

  for (const OrderItem& item : order.getItems()) {
    const QString name = item.getMenuItem()
      ? item.getMenuItem()->getName()
      : QStringLiteral("(unknown item)");

    parts << QStringLiteral("%1 x %2").arg(item.getQuantity()).arg(name);
  }

  return parts.join(QStringLiteral(", "));
}

}

#endif
