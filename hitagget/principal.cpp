#include "principal.h"
#include "ui_principal.h"
#include <time.h>
#include "viewpost.h"
#include "newpost.h"
#include "postui.h"
#include "trend.h"
#include "followerlistui.h"
#include <QFileDialog>
#include <QFile>
#include <QTextCodec>
#include <QDate>
#include <QRandomGenerator>
#include <QCloseEvent>
#include <stdlib.h>

void Principal::show_all_posts(int op, bool inv, bool show_specific_profile)
{
    function<void(Post*, string)> show_post = [this](Post* post, string author_name)
    {
        list<PostComment*> comments = main_instance->getPostComments(post->id);

        if (post->id != -1)
        {
            main_instance->addInteraction(main_instance->logged_user->id, post, false);
            ViewPost *view_post_dialog = new ViewPost();

            view_post_dialog->share_post_function = [this](Post* post)
            {
                main_instance->editLastInteraction(true);
                post->numInteractions++;
                main_instance->updatePostsAVLs(post);

                if (ui->cb_criterios->currentIndex() == 0)
                    show_all_posts(op_busq, invertir, main_instance->get_shown_user() != nullptr ? true : false);
            };

            view_post_dialog->new_comment_function = [this](Post* post, string content)
            {
                main_instance->addInteraction(main_instance->logged_user->id, post, false);
                main_instance->addComment(post->id, content);

                main_instance->updatePostsAVLs(post);

                if (ui->cb_criterios->currentIndex() == 0)
                    show_all_posts(op_busq, invertir, main_instance->get_shown_user() != nullptr ? true : false);
            };

            view_post_dialog->like_post_function = [this](Post* post)
            {
                main_instance->updatePostsAVLs(post);

                if (ui->cb_criterios->currentIndex() == 2)
                    show_all_posts(op_busq, invertir, main_instance->get_shown_user() != nullptr ? true : false);
            };

            view_post_dialog->edit_post_function = [this](Post* oldPost, Post* post, bool hasChangedTitle)
            {
                if (hasChangedTitle)
                {
                    main_instance->updatePostsTitleAVL(post);
                    main_instance->updateTrendsFromEditedPostTitle(oldPost, post);
                    act_tend();
                }

                show_all_posts(op_busq, invertir, main_instance->get_shown_user() != nullptr ? true : false);
            };

            view_post_dialog->delete_post_function = [this](Post* post)
            {
                main_instance->deletePost(post);
            };

            view_post_dialog->set_current_post(post, author_name, comments);

            if (post->authorId != main_instance->logged_user->id)
                view_post_dialog->hide_author_actions_buttons();

            view_post_dialog->exec();
        }
    };

    function<void(int)> show_author_profile = [this](int authorID)
    {
        main_instance->set_shown_user(authorID);

        if (!main_instance->logged_user->isFollowing(authorID))
            ui->btnFollow->setVisible(true);

        ui->btnCloseUserProfile->setVisible(true);
        ui->lblShownUser->setVisible(true);
        ui->lblShownUser->clear();
        ui->lblShownUser->setText(("Mostrando usuario:\n" + main_instance->get_shown_user()->fullname + "\n" + main_instance->get_shown_user()->registerDate).c_str());

        show_all_posts(ui->cb_criterios->currentIndex(), invertir, true);
    };

    ui->listWidgetPubli->clear();

    list<Post*> posts;

    switch(op){
    case 0:
        //INTERACCIONES

        if (ui->txtSearchBox->text() == "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsByNumInteractions(inv, 50);
            }
            else
                posts = main_instance->getPostsByNumInteractions(inv, 50);
        }
        else
        {
            // We have something in the search box, we will show the results of the search box instead ordered by the interaction criteria
            show_all_posts(3 + (ui->cmbBoxSearchOptions->currentIndex()), invertir, show_specific_profile);
            return;
        }
        break;
    case 1:
        //FECHA

        if (ui->txtSearchBox->text() == "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsByPubDate(inv, 50);
            }
            else
                posts = main_instance->getPostsByPubDate(inv, 50);
        }
        else
        {
            // We have something in the search box, we will show the results of the search box instead ordered by the date criteria
            show_all_posts(3 + (ui->cmbBoxSearchOptions->currentIndex()), invertir, show_specific_profile);
            return;
        }
        break;
    case 2:
        //LIKES

        if (ui->txtSearchBox->text() == "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsByLikes(inv, 50);
            }
            else
                posts = main_instance->getPostsByLikes(inv, 50);
        }
        else
        {
            // We have something in the search box, we will show the results of the search box instead ordered by the likes criteria
            show_all_posts(3 + (ui->cmbBoxSearchOptions->currentIndex()), invertir, show_specific_profile);
            return;
        }
        break;
    case 3:
        //IGUAL A
        if (ui->txtSearchBox->text()!= "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsThatTitleEqualsToString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsThatTitleEqualsToString(ui->txtSearchBox->text().toStdString(), inv, 50, 3);
        }
        break;
    case 4:
        //INICIA CON
        if (ui->txtSearchBox->text() != "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsThatStartsWithString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsThatStartsWithString(ui->txtSearchBox->text().toStdString(), inv, 50, 0);
        }
        break;
    case 5:
        //FINALIZA CON
        if (ui->txtSearchBox->text() != "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsThatEndsWithString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsThatEndsWithString(ui->txtSearchBox->text().toStdString(), inv, 50, 1);
        }
        break;
    case 6:
        //CONTIENE
        if (ui->txtSearchBox->text() != "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsThatContainsString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsThatContainsString(ui->txtSearchBox->text().toStdString(), inv, 50, 2);
        }
        break;
    case 7:
        //NO CONTIENE
        if (ui->txtSearchBox->text() != "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsNoContainsString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsNoContainsString(ui->txtSearchBox->text().toStdString(), inv, 50, 0);
        }
        break;
    default:
        posts = main_instance->getPostsByTitle(inv, 50);
        break;
    }

    for (Post* post : posts)
    {
        if (!post->isDeleted)
        {
            string author_name = main_instance->getUserById(post->authorId)->fullname;
            add_item_to_list_widget(ui->listWidgetPubli, post, show_post, show_author_profile, author_name);
        }
    }
}

void Principal::add_item_to_list_widget(QListWidget *list, Post* individual_post, function<void(Post*, string)> show_post, function<void(int)> show_author_profile, string author_name)
{

    QListWidgetItem *item = new QListWidgetItem();
    list->addItem(item);

    PostUI *post_ui = new PostUI();

    post_ui->post_text->setText(individual_post->title.c_str());
    post_ui->post_to_show = individual_post;
    post_ui->author_profile_button->setText(author_name.c_str());

    post_ui->set_view_button_click_action(show_post);
    post_ui->set_view_user_button_click_action(show_author_profile);

    item->setSizeHint(post_ui->minimumSizeHint());

    list->setItemWidget(item, post_ui);
}

void Principal::show_followed_users()
{
    ui->listWidgetFollowers->clear();

    for(Follower* follower : main_instance->logged_user->followedUsers)
    {
        if (!follower->isDeleted)
        {
            User* user_shown = main_instance->getUserById(follower->followedUserID);

            if (user_shown != nullptr)
            {
                QListWidgetItem *item = new QListWidgetItem();
                ui->listWidgetFollowers->addItem(item);

                FollowerListUI *follower_ui = new FollowerListUI();

                follower_ui->user_name_button->setText(user_shown->fullname.c_str());
                follower_ui->user_shown = user_shown;

                follower_ui->current_follower = follower;

                follower_ui->set_user_click_action([this](User* user)
                {
                    main_instance->set_shown_user(user);

                    if (!main_instance->logged_user->isFollowing(user->id))
                        ui->btnFollow->setVisible(true);

                    ui->btnCloseUserProfile->setVisible(true);
                    ui->lblShownUser->setVisible(true);
                    ui->lblShownUser->clear();
                    ui->lblShownUser->setText(("Mostrando usuario:\n" + main_instance->get_shown_user()->fullname + "\n" + main_instance->get_shown_user()->registerDate).c_str());

                    show_all_posts(ui->cb_criterios->currentIndex(), invertir, true);
                });

                follower_ui->set_remove_click_action([this](Follower* follower)
                {
                    follower->isDeleted = true;

                    main_instance->deleteFollower(follower);

                    show_followed_users();
                });

                item->setSizeHint(follower_ui->minimumSizeHint());

                ui->listWidgetFollowers->setItemWidget(item, follower_ui);
            }
        }
    }
}

void Principal::show_contacts(list<User*> contacts)
{
    for (User* user : contacts)
    {
        QListWidgetItem *item = new QListWidgetItem();
        ui->listWidgetCont->addItem(item);

        FollowerListUI *follower_ui = new FollowerListUI();

        follower_ui->user_name_button->setText(user->fullname.c_str());
        follower_ui->user_shown = user;

        follower_ui->remove_follower_button->setVisible(false);

        follower_ui->set_user_click_action([this](User* user)
        {
            main_instance->set_shown_user(user);

            if (!main_instance->logged_user->isFollowing(user->id))
                ui->btnFollow->setVisible(true);

            ui->btnCloseUserProfile->setVisible(true);
            ui->lblShownUser->setVisible(true);
            ui->lblShownUser->clear();
            ui->lblShownUser->setText(("Mostrando usuario:\n" + main_instance->get_shown_user()->fullname + "\n" + main_instance->get_shown_user()->registerDate).c_str());

            show_all_posts(ui->cb_criterios->currentIndex(), invertir, true);
        });

        item->setSizeHint(follower_ui->minimumSizeHint());

        ui->listWidgetCont->setItemWidget(item, follower_ui);
    }
}

void Principal::closeEvent(QCloseEvent *event)
{
    main_instance->save_instance();
}

Principal::Principal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Principal)
{
    main_instance = new Hitagget();

    setlocale(LC_ALL,"");
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    ui->btnCloseUserProfile->setVisible(false);
    ui->lblShownUser->setVisible(false);
    ui->btnFollow->setVisible(false);



    show_all_posts(ui->cmbBoxSearchOptions->currentIndex(), invertir, false);

    act_tend();

    string tempstr;

    fstream arch_foto("Foto.txt");
        getline(arch_foto,tempstr);

    QString tempR_F = QString::fromStdString(tempstr);
    if(tempR_F != ""){
        QPixmap *img = new QPixmap(tempR_F);
        ui->lblFPerfil->setPixmap(*img);
    }

    connect(ui->btnCamFoto,SIGNAL(released()),this,SLOT(cambiar_imagen()));

    connect(ui->btnPublicar,SIGNAL(released()),this,SLOT(new_publi()));

    connect(ui->btnInvertir, SIGNAL(released()), this, SLOT(f_invertir()));
    /*
        ifstream file("Texto.txt");
        string finalString = "";

        if (file.is_open())
        {
            string texto;

            getline(file, texto);

            vector<string> tokens;
            size_t prev = 0, pos = 0;
            do
            {
                pos = texto.find(' ', prev);
                if (pos == string::npos) pos = texto.length();
                string token = texto.substr(prev, pos-prev);
                if (token != "") tokens.push_back(token);

                prev = pos + 1;
            }
            while (pos < texto.length() && prev < texto.length());

            srand(time(0));

            for (int i = 0; i < 5000; ++i)
            {
                int random = rand();
                random <<= 15;
                random ^= rand();
                random %= tokens.size();

                if (tokens[random][0] != '#')
                    tokens[random] = "#" + tokens[random];
            }

            for (string word : tokens)
            {
                finalString += word + " ";
            }
        }

        ofstream newFile("Texto2.txt");

        newFile << finalString;

        file.close();
        newFile.close();
    */
}


void Principal::act_tend()
{
    ui->listWidgetTend->clear();

    for (Trend* trend : main_instance->get_latest_trends(20))
    {
        if (trend->count > 0)
            ui->listWidgetTend->addItem(QString(trend->tag.c_str()) + "\n" + QString::number(trend->count) + " posts");
    }
}

void Principal::update_contacts()
{
    ui->listWidgetCont->clear();

    srand(main_instance->logged_user->id);

    int numUsers = main_instance->getUsersCount();

    list<User*> contacts;

    for (int i = 0; i < 1 + rand() % 20; ++i)
    {
       contacts.push_back(main_instance->getUserById(1 + rand() % numUsers));
    }

    contacts.unique([](const User* a, const User* b) { return a->id == b->id; });

    show_contacts(contacts);
}



void Principal::show_user_info(){

    ui->lblUsuario->clear();
    ui->lblUsuario->setText("Usuario:\n" + *Unombre + "\n" + *UfechaR);
    show_followed_users();
}

void Principal::cambiar_imagen(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"C:\\",tr("Image Files(*.png *.pjg *.bmp)"));
    if(filename!="") {
        QPixmap *img = new QPixmap(filename);
        ui->lblFPerfil->setPixmap(*img);

        ofstream archivo;
        archivo.open("Foto.txt");
        archivo<<filename.toStdString();
        archivo.close();
    }
}

void Principal::new_publi(){
    NewPost *newPost = new NewPost();
            newPost->exec();

    if (newPost->new_post == nullptr)
    {
        return;
    }
    else
    {
        if (newPost->new_post->title != "" && newPost->new_post->content != "")
        {
            main_instance->addPost(main_instance->logged_user->id, newPost->new_post->title, newPost->new_post->content);

            if (main_instance->get_shown_user() != nullptr)
                show_all_posts(op_busq, invertir, true);
            else
                show_all_posts(op_busq, invertir, false);

            act_tend();
        }
    }
}

void Principal::f_invertir()
{
    invertir = !invertir;

    if (main_instance->get_shown_user() == nullptr)
        show_all_posts(op_busq, invertir, false);
    else
        show_all_posts(op_busq, invertir, true);
}


Principal::~Principal()
{
    delete ui;
}

void Principal::on_txtSearchBox_returnPressed()
{
    if (ui->txtSearchBox->text() == "")
    {
        op_busq = ui->cmbBoxSearchOptions->currentIndex();

        if (main_instance->get_shown_user() == nullptr)
            show_all_posts(op_busq, invertir, false);
        else
            show_all_posts(op_busq, invertir, true);

        return;
    }

    op_busq = 3 + (ui->cmbBoxSearchOptions->currentIndex());

    if (main_instance->get_shown_user() == nullptr)
        show_all_posts(op_busq, invertir, false);
    else
        show_all_posts(op_busq, invertir, true);
}

void Principal::on_cb_criterios_currentIndexChanged(int index)
{
    op_busq = index;

    if (main_instance->get_shown_user() == nullptr)
        show_all_posts(index, invertir, false);
    else
        show_all_posts(index, invertir, true);
}

void Principal::on_btnCloseUserProfile_clicked()
{
    main_instance->clear_shown_user();

    ui->btnCloseUserProfile->setVisible(false);
    ui->lblShownUser->setVisible(false);
    ui->btnFollow->setVisible(false);
    ui->lblShownUser->clear();

    show_all_posts(op_busq, invertir, false);
}

void Principal::on_btnFollow_clicked()
{
    ui->btnFollow->setVisible(false);
    Follower* newFollower = main_instance->addFollower(main_instance->logged_user->id, main_instance->get_shown_user()->id);
    main_instance->logged_user->followedUsers.push_back(newFollower);

    show_followed_users();
}

void Principal::on_btnLogOut_clicked()
{
    main_instance->log_out();

    invertir = false;
    op_busq = 0;

    this->hide();
    this->loginWindow->show();
}

void Principal::on_btnShowMyProfile_clicked()
{
    main_instance->set_shown_user(main_instance->logged_user);

    ui->btnCloseUserProfile->setVisible(true);
    ui->lblShownUser->setVisible(true);
    ui->lblShownUser->clear();
    ui->lblShownUser->setText("Mostrando:\n Mi perfil");

    show_all_posts(ui->cb_criterios->currentIndex(), invertir, true);
}
