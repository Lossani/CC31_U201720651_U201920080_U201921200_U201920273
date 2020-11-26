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
    if (new_post == nullptr)
        return;

    ui->txtPostTitle->setText(new_post->title.c_str());
    ui->txtPostContent->setText(new_post->content.c_str());
}

void NewPost::on_buttonBox_accepted()
{
    new_post = new Post();
    new_post->title = ui->txtPostTitle->text().toStdString();
    new_post->content = ui->txtPostContent->toPlainText().toStdString();
}

void NewPost::on_buttonBox_rejected()
{
    new_post = nullptr;
}
