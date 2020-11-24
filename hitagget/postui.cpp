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
    author_profile_button->setFixedWidth(100);

    post_text = new QLabel("Vacío");
    //edit_button = new QPushButton("Editar");
    //delete_button = new QPushButton("Borrar");

    layout->addWidget(view_button);
    layout->addWidget(author_profile_button);
    layout->addWidget(post_text);

    //layout->addWidget(edit_button);
    //layout->addWidget(delete_button);
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

/*
void PostUI::set_edit_button_click_action(std::function<void (int)> edit_post)
{
    edit_click_function = edit_post;
    connect(edit_button, SIGNAL (released()), this, SLOT (edit_button_click()));
}

void PostUI::set_delete_button_click_action(std::function<void (int)> delete_post)
{
    delete_click_function = delete_post;
    connect(delete_button, SIGNAL (released()), this, SLOT (delete_button_click()));
}
*/
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
/*
void PostUI::edit_button_click()
{
    if (edit_click_function != nullptr)
    {
        edit_click_function(post_id);
    }
}

void PostUI::delete_button_click()
{
    if (delete_click_function != nullptr)
    {
        delete_click_function(post_id);
    }
}
*/
