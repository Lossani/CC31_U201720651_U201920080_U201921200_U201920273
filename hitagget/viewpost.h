#ifndef VIEWPOST_H
#define VIEWPOST_H

#include <QDialog>
#include "post.h"

namespace Ui {
class ViewPost;
}

class ViewPost : public QDialog
{
    Q_OBJECT

public:
    explicit ViewPost(QWidget *parent = nullptr);
    ~ViewPost();
    void set_current_post(Post* post, string author_name, list<PostComment*> postComments);

private slots:
    void on_btnLike_clicked();

private:
    Ui::ViewPost *ui;
    Post* current_post;
    bool hasLikedPost = false;
};

#endif // VIEWPOST_H
