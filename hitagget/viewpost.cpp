#include "viewpost.h"
#include "ui_viewpost.h"
#include <QMessageBox>
#include <QTime>

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
    ui->lblDate->setText(post->pubDate.c_str());
    ui->lblPostID->setText(("ID: " + to_string(post->id)).c_str());
    ui->lblNumInteractions->setText(to_string(post->numInteractions).c_str());

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

    if (like_post_function != nullptr)
        like_post_function(current_post);
    ui->lblNumLikes->setText(to_string(current_post->numLikes).c_str());
}

void ViewPost::on_btnShare_clicked()
{
    ui->btnShare->setVisible(false);

    if (share_post_function != nullptr)
        share_post_function(current_post);

    ui->lblNumInteractions->setText(to_string(current_post->numInteractions).c_str());
}

void ViewPost::on_btnComment_clicked()
{
    if (new_comment_function != nullptr)
    {
        new_comment_function(current_post, ui->txtNewCommentContent->text().toStdString());
        ui->listPostComments->addItem(QDateTime::currentDateTime().toString("yyyy-MM-dd") + '\t' + ui->txtNewCommentContent->text());
        ui->txtNewCommentContent->setText("");
    }
}

void ViewPost::on_btnDialog_accepted()
{
    this->close();
}
