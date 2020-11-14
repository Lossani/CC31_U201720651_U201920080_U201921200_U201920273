#ifndef NEWPOST_H
#define NEWPOST_H

#include <QDialog>
#include "post.h"

#include <string>

using namespace std;

namespace Ui {
class NewPost;
}

class NewPost : public QDialog
{
    Q_OBJECT

public:
    explicit NewPost(QWidget *parent = nullptr);
    ~NewPost();
    //char* get_post_content();
    //int get_post_id();
    //int post_id = -1;
    string post_content = "";
    Post* new_post = nullptr;
    void show_post();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::NewPost *ui;

};

#endif // NEWPOST_H
