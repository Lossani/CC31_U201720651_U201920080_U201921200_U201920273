#include "followerlistui.h"
#include <QHBoxLayout>

FollowerListUI::FollowerListUI(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    user_name_button = new QPushButton("");
    user_name_button->setFixedHeight(50);

    remove_follower_button = new QPushButton("Eliminar");
    remove_follower_button->setFixedHeight(50);

    layout->addWidget(user_name_button);
    layout->addWidget(remove_follower_button);
}

void FollowerListUI::view_user_button_click()
{
    if (user_click_function != nullptr)
    {
        user_click_function(user_shown);
    }
}

void FollowerListUI::remove_follower_button_click()
{
    if (remove_follower_function != nullptr)
    {
        remove_follower_function(current_follower);
    }
}

void FollowerListUI::set_user_click_action(function<void (User *)> show_user)
{
    user_click_function = show_user;
    connect(user_name_button, SIGNAL(released()), this, SLOT (view_user_button_click()));
}

void FollowerListUI::set_remove_click_action(function<void (Follower*)> remove_follower)
{
    remove_follower_function = remove_follower;
    connect(remove_follower_button, SIGNAL(released()), this, SLOT (remove_follower_button_click()));
}
