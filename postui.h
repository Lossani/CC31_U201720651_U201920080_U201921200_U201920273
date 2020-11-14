#ifndef POSTUI_H
#define POSTUI_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <functional>

class PostUI : public QWidget
{
    Q_OBJECT
public:
    explicit PostUI(QWidget *parent = nullptr);
    QLabel *post_text;
    QPushButton *view_button;
    //QPushButton *edit_button;
    //QPushButton *delete_button;
    int post_id;

private:
    std::function<void(int param)> view_click_function;
    //std::function<void(int param)> edit_click_function;
    //std::function<void(int param)> delete_click_function;

private slots:
    void view_button_click();
    //void edit_button_click();
    //void delete_button_click();

public:
    void set_view_button_click_action(std::function<void(int param)> show_post);
    //void set_edit_button_click_action(std::function<void(int param)> edit_post);
    //void set_delete_button_click_action(std::function<void(int param)> delete_post);

};

#endif // POSTUI_H
