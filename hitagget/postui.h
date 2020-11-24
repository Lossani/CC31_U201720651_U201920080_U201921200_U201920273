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
    //QPushButton *edit_button;
    //QPushButton *delete_button;
    Post* post_to_show;

private:
    function<void(Post* post, string author_name)> view_click_function;
    function<void(int userID)>  view_user_function;
    //std::function<void(int param)> edit_click_function;
    //std::function<void(int param)> delete_click_function;

private slots:
    void view_button_click();
    void view_user_button_click();
    //void edit_button_click();
    //void delete_button_click();

public:
    void set_view_button_click_action(function<void(Post* post, string author_name)> show_post);
    void set_view_user_button_click_action(function<void(int userID)> show_user_profile);
    //void set_edit_button_click_action(std::function<void(int param)> edit_post);
    //void set_delete_button_click_action(std::function<void(int param)> delete_post);

};

#endif // POSTUI_H
