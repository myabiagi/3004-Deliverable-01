#include "userdirectory.h"

bool UserDirectory::addUser(const std::shared_ptr<User>& user)
{
    if (user == nullptr)
    {
        return false;
    }

    QString newUsername = user->getUsername().trimmed();

    if (newUsername.isEmpty())
    {
        return false;
    }

    // usernames must be unique ignoring capitalization
    for (const std::shared_ptr<User>& existingUser : users)
    {
        if (existingUser->getUsername().trimmed().compare(
                newUsername,
                Qt::CaseInsensitive) == 0)
        {
            return false;
        }
    }

    users.push_back(user);
    return true;
}

std::shared_ptr<User>
UserDirectory::findByNameOrUsername(const QString& input) const
{
    QString searchText = input.trimmed();

    if (searchText.isEmpty())
    {
        return nullptr;
    }

    for (const std::shared_ptr<User>& user : users)
    {
        QString name = user->getName().trimmed();
        QString username = user->getUsername().trimmed();

        // identification accepts either name or username
        bool nameMatches =
            name.compare(searchText, Qt::CaseInsensitive) == 0;

        bool usernameMatches =
            username.compare(searchText, Qt::CaseInsensitive) == 0;

        if (nameMatches || usernameMatches)
        {
            return user;
        }
    }

    return nullptr;
}

const std::vector<std::shared_ptr<User>>&
UserDirectory::getUsers() const
{
    return users;
}
