#include "viewpost.h"
#include "ui_viewpost.h"

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

void ViewPost::set_current_post(Post post)
{
    ui->lblPostContent->setText(post.content.c_str());
}
