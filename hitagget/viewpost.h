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
    function<void(Post*)> share_post_function;
    function<void(Post*, string)> new_comment_function;
    function<void(Post*)> like_post_function;

private slots:
    void on_btnLike_clicked();

    void on_btnShare_clicked();

    void on_btnComment_clicked();

    void on_btnDialog_accepted();

private:
    Ui::ViewPost *ui;
    Post* current_post;
    bool hasLikedPost = false;
};

#endif // VIEWPOST_H
