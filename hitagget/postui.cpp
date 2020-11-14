#include "postui.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <functional>

PostUI::PostUI(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    post_text = new QLabel("Vacío");

    view_button = new QPushButton("Ver");
    //edit_button = new QPushButton("Editar");
    //delete_button = new QPushButton("Borrar");
    layout->addWidget(post_text);
    layout->addWidget(view_button);
    //layout->addWidget(edit_button);
    //layout->addWidget(delete_button);
}

void PostUI::set_view_button_click_action(std::function<void(int)> show_post)
{
    view_click_function = show_post;
    connect(view_button, SIGNAL (released()), this, SLOT (view_button_click()));
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
        view_click_function(post_id);
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
