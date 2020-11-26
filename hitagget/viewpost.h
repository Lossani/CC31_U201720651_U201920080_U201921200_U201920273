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
    function<void(Post*)> share_post_function = nullptr;
    function<void(Post*, string)> new_comment_function = nullptr;
    function<void(Post*)> like_post_function = nullptr;
    function<void(Post*, Post*, bool)> edit_post_function = nullptr;
    function<void(Post*)> delete_post_function = nullptr;

    void hide_author_actions_buttons();

private slots:
    void on_btnLike_clicked();

    void on_btnShare_clicked();

    void on_btnComment_clicked();

    void on_btnDialog_accepted();

    void on_btnDeletePost_clicked();

    void on_btnEditPost_clicked();

private:
    Ui::ViewPost *ui;
    Post* current_post;
    bool hasLikedPost = false;
};

#endif // VIEWPOST_H
