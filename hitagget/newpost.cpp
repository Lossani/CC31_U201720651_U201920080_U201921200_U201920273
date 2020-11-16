#include "newpost.h"
#include "ui_newpost.h"
#include <iostream>

using namespace std;

NewPost::NewPost(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewPost)
{
    ui->setupUi(this);
}

NewPost::~NewPost()
{
    delete ui;
}

void NewPost::show_post()
{
    ui->label_2->setText(post_content.c_str());
}

void NewPost::on_buttonBox_accepted()
{
    new_post = new Post();
    new_post->title = ui->txtPostTitle->text().toStdString();
    new_post->content = ui->txtPostContent->toPlainText().toStdString();
    //post_content = ui->lineEdit->text().toStdString();
}
/*
char* NewPost::get_post_content()
{
    return post_content;
}

int NewPost::get_post_id()
{
    return post_id;
}
*/
void NewPost::on_buttonBox_rejected()
{
    post_content = "";
}
