#include "viewpost.h"
#include "ui_viewpost.h"
#include <QMessageBox>

ViewPost::ViewPost(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewPost)
{
    ui->setupUi(this);
}

ViewPost::~ViewPost()
{
    delete ui;
}

void ViewPost::set_current_post(Post post, list<PostComment*> postComments)
{
    ui->lblPostTitle->setText(post.title.c_str());
    ui->lblPostContent->setText(post.content.c_str());

    for (PostComment* comment: postComments)
    {
        ui->listPostComments->addItem(comment->content.c_str());
    }
}
