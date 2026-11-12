#include "manager.h"

// initialize the inherited staff information
Manager::Manager(const QString& name,const QString& username,const QString& staffId)
    : Staff(name, username, staffId){
}

// return the manager's user role
UserRole Manager::getRole() const { return UserRole::Manager; }
