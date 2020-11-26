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
    QPushButton* user_name;
    User* user_shown;

private:
    function<void(User* user)> user_click_function;

private slots:
    void view_user_button_click();

public:
    void set_user_click_action(function<void(User* user)> show_user);

signals:

};

#endif // FOLLOWERLISTUI_H
