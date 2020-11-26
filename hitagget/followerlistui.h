#ifndef FOLLOWERLISTUI_H
#define FOLLOWERLISTUI_H

#include <QWidget>
#include <QPushButton>
#include <functional>

#include "User.h"

class FollowerListUI : public QWidget
{
    Q_OBJECT
public:
    explicit FollowerListUI(QWidget *parent = nullptr);
    QPushButton* user_name_button;
    QPushButton* remove_follower_button;
    User* user_shown;
    Follower* current_follower;

private:
    function<void(User* user)> user_click_function = nullptr;
    function<void(Follower* follower)> remove_follower_function = nullptr;

private slots:
    void view_user_button_click();
    void remove_follower_button_click();

public:
    void set_user_click_action(function<void(User* user)> show_user);
    void set_remove_click_action(function<void(Follower* follower)> remove_follower);

signals:

};

#endif // FOLLOWERLISTUI_H
