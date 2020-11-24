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

void ViewPost::set_current_post(Post* post, string author_name, list<PostComment*> postComments)
{
    ui->lblPostTitle->setText(post->title.c_str());
    ui->lblPostContent->setText(post->content.c_str());
    ui->lblNumLikes->setText(to_string(post->numLikes).c_str());
    ui->lblAuthorName->setText(author_name.c_str());

    for (PostComment* comment: postComments)
    {
        ui->listPostComments->addItem((comment->pubDate + '\t' + comment->content).c_str());
    }

    current_post = post;
}

void ViewPost::on_btnLike_clicked()
{
    if (hasLikedPost)
    {
        hasLikedPost = false;

        current_post->numLikes--;

        ui->btnLike->setStyleSheet("");
    }
    else
    {
        hasLikedPost = true;

        current_post->numLikes++;

        ui->btnLike->setStyleSheet("color: rgb(255, 0, 0);");
    }

    ui->lblNumLikes->setText(to_string(current_post->numLikes).c_str());
}
