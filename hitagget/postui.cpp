#include "postui.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <functional>

PostUI::PostUI(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    view_button = new QPushButton("Ver");
    view_button->setFixedWidth(50);

    author_profile_button = new QPushButton("");
    author_profile_button->setFixedWidth(150);

    post_text = new QLabel("Vacío");

    layout->addWidget(view_button);
    layout->addWidget(author_profile_button);
    layout->addWidget(post_text);
}

void PostUI::set_view_button_click_action(std::function<void(Post*, string)> show_post)
{
    view_click_function = show_post;
    connect(view_button, SIGNAL (released()), this, SLOT (view_button_click()));
}

void PostUI::set_view_user_button_click_action(function<void (int)> show_user_profile)
{
    view_user_function = show_user_profile;
    connect(author_profile_button, SIGNAL (released()), this, SLOT (view_user_button_click()));
}

void PostUI::view_button_click()
{
    if (view_click_function != nullptr)
    {
        view_click_function(post_to_show, author_profile_button->text().toStdString());
    }
}

void PostUI::view_user_button_click()
{
    if (view_user_function != nullptr)
    {
        view_user_function(post_to_show->authorId);
    }
}
