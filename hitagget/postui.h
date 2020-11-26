#ifndef POSTUI_H
#define POSTUI_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <functional>

#include "post.h"

using namespace std;

class PostUI : public QWidget
{
    Q_OBJECT
public:
    explicit PostUI(QWidget *parent = nullptr);
    QLabel *post_text;
    QPushButton *view_button;
    QPushButton *author_profile_button;
    Post* post_to_show;

private:
    function<void(Post* post, string author_name)> view_click_function;
    function<void(int userID)>  view_user_function;

private slots:
    void view_button_click();
    void view_user_button_click();

public:
    void set_view_button_click_action(function<void(Post* post, string author_name)> show_post);
    void set_view_user_button_click_action(function<void(int userID)> show_user_profile);

};

#endif // POSTUI_H
