#include "followerlistui.h"
#include <QHBoxLayout>

FollowerListUI::FollowerListUI(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    user_name = new QPushButton("");
    user_name->setFixedHeight(50);

    layout->addWidget(user_name);
}

void FollowerListUI::view_user_button_click()
{
    if (user_click_function != nullptr)
    {
        user_click_function(user_shown);
    }
}

void FollowerListUI::set_user_click_action(function<void (User *)> show_user)
{
    user_click_function = show_user;
    connect(user_name, SIGNAL(released()), this, SLOT (view_user_button_click()));
}
