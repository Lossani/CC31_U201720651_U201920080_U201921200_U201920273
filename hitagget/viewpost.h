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
    void set_current_post(Post post, list<PostComment*> postComments);

private:
    Ui::ViewPost *ui;
};

#endif // VIEWPOST_H
