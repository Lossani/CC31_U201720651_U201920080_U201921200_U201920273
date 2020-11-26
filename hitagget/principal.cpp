#include "principal.h"
#include "ui_principal.h"
#include <time.h>
#include "viewpost.h"
#include "newpost.h"
#include "postui.h"
#include "followerlistui.h"
#include <QFileDialog>
#include <QFile>
#include <QTextCodec>
#include <QDate>
#include <QRandomGenerator>
#include <QCloseEvent>

bool invertir = false;
int op_busq = 0;

void Principal::show_all_posts(int op, bool inv, bool show_specific_profile)
{
    function<void(Post*, string)> show_post = [this](Post* post, string author_name)
    {
        list<PostComment*> comments = main_instance->getPostComments(post->id);
        QMessageBox msg;

        if (post->id != -1)
        {
            main_instance->addInteraction(main_instance->logged_user->id, post, false);
            ViewPost *view_post_dialog = new ViewPost();

            view_post_dialog->share_post_function = [this](Post* post)
            {
                main_instance->editLastInteraction(true);
            };

            view_post_dialog->new_comment_function = [this](Post* post, string content)
            {
                main_instance->addInteraction(main_instance->logged_user->id, post, false);
                main_instance->addComment(post->id, content);
            };

            view_post_dialog->set_current_post(post, author_name, comments);
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
/*
    function<void(int)> edit_post = [this](int id)
    {
        Post post_to_update = main_instance->get_post(id);

        if (post_to_update.id != 1)
        {
            NewPost *edit_post_dialog = new NewPost();
            edit_post_dialog->post_content = post_to_update.content;
            edit_post_dialog->show_post();
            edit_post_dialog->exec();

            string result = edit_post_dialog->post_content;

            if (result == "")
            {
                return;
            }
            else
            {
                post_to_update.content = result;
                main_instance->update_post(post_to_update);
                show_all_posts();
            }
        }
    };

    function<void(int)> delete_post = [this](int id)
    {
        main_instance->delete_post(id);

        show_all_posts();
    };
*/
    ui->listWidgetPubli->clear();

    list<Post*> posts;

    switch(op){
    case 0:
        //INTERACCIONES

        if (ui->txtSearchBox->text().toStdString().empty())
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

        if (ui->txtSearchBox->text().toStdString().empty())
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

        if (ui->txtSearchBox->text().toStdString().empty())
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
    case 3:
        //IGUAL A
        if (ui->txtSearchBox->text().toStdString() != "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsThatTitleEqualsToString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsThatTitleEqualsToString(ui->txtSearchBox->text().toStdString(), inv, 50);
        }
        break;
    case 4:
        //INICIA CON
        if (ui->txtSearchBox->text().toStdString() != "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsThatStartsWithString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsThatStartsWithString(ui->txtSearchBox->text().toStdString(), inv, 50);
        }
        break;
    case 5:
        //FINALIZA CON
        if (ui->txtSearchBox->text().toStdString() != "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsThatEndsWithString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsThatEndsWithString(ui->txtSearchBox->text().toStdString(), inv, 50);
        }
        break;
    case 6:
        //CONTIENE
        if (ui->txtSearchBox->text().toStdString() != "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsThatContainsString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsThatContainsString(ui->txtSearchBox->text().toStdString(), inv, 50);
        }
        break;
    case 7:
        //NO CONTIENE
        if (ui->txtSearchBox->text().toStdString() != "")
        {
            if (show_specific_profile)
            {
                posts = main_instance->getShownUserPostsNoContainsString(ui->txtSearchBox->text().toStdString(), inv, 50);
            }
            else
                posts = main_instance->getPostsNoContainsString(ui->txtSearchBox->text().toStdString(), inv, 50);
        }
        break;
    default:
        posts = main_instance->getPostsByTitle(inv, 50);
        break;
    }

    for (Post* post : posts)
    {
        string author_name = main_instance->getUserById(post->authorId)->fullname;
        add_item_to_list_widget(ui->listWidgetPubli, post, show_post, show_author_profile, author_name);
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
        QListWidgetItem *item = new QListWidgetItem();
        ui->listWidgetFollowers->addItem(item);

        FollowerListUI *follower_ui = new FollowerListUI();

        User* user_shown = main_instance->getUserById(follower->followedUserID);
        follower_ui->user_name->setText(user_shown->fullname.c_str());
        follower_ui->user_shown = user_shown;

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

        ui->listWidgetFollowers->setItemWidget(item, follower_ui);
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
    setlocale(LC_ALL,"");
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    ui->btnCloseUserProfile->setVisible(false);
    ui->lblShownUser->setVisible(false);
    ui->btnFollow->setVisible(false);

    //Arboles Contactos
    this->BST_Cont_Apodo = new BST<Contacto, QString>([](Contacto j){return j.get_apodo();});
    this->BST_Cont_CName = new BST<Contacto, QString>([](Contacto j){return j.get_comp_name();});
    this->BST_Cont_Number = new BST<Contacto, QString>([](Contacto j){return j.get_numero();});





    main_instance = new Hitagget();

    show_all_posts(0, false, false);

    lst = new ListaT();

    string tempstr;

    fstream arch_foto("Foto.txt");
        getline(arch_foto,tempstr);


    QString tempR_F = QString::fromStdString(tempstr);
    if(tempR_F != ""){
        QPixmap *img = new QPixmap(tempR_F);
        ui->lblFPerfil->setPixmap(*img);
    }



    //Publicaciones






    QRandomGenerator r;

    string nom_publi;
    string tit_publi;
    string hash_publi;
    string linea_publi;

    ifstream publi("Publicaciones.txt");

    while(publi.good()){
        getline(publi,nom_publi);
        getline(publi,tit_publi);
        getline(publi,hash_publi);
        getline(publi,linea_publi);

        int tamL = 93;
        int cont = 0;
        int SizeLineaPubli = linea_publi.size();
        for (int i = 0; i < SizeLineaPubli;i++) {
            cont++;
            if (cont>tamL) {
                if (linea_publi[i] == ' ') {
                    linea_publi[i] = '\n';
                    cont=0;
                }
            }
        }

        QString temp_nm = QString::fromStdString(nom_publi);
        QString temp_tp = QString::fromStdString(tit_publi);
        QString temp_hp = QString::fromStdString(hash_publi);
        QString temp_lp = QString::fromStdString(linea_publi);





        new_public = new Hitagged(temp_nm, temp_tp,temp_lp, temp_hp);

        lst->addFirst(new_public->get_hashtag());

        Publi.enqueue(new_public);

    }



    //Tendencias


    ui->listWidgetTend->clear();


    ListaT *tempLT = new ListaT;
    for(int i=0;i<lst->size();i++){
        tempLT->addFirst(lst->get(i));
    }



    vector<int> cont;

    for (int i = 0; i < tempLT->size(); i++){
        int n = 1;
        cont.push_back(n);

          for (int j = tempLT->size(); j > i;j--){
            if(tempLT->get(i) == tempLT->get(j)){
              tempLT->removePos(j);
              cont[i]++;
              j=tempLT->size();
            }
          }

    }

    //MOSTRAR TENDENCIAS

    QVector<tnd> ord;

    for (int i = 0; i < tempLT->size(); i++){

        tnd temptend(tempLT->get(i),cont[i]);
        ord.append(temptend);
    }

    for (int i = 0; i < ord.size() - 1; i++) {
      int min = i;
      for (int j = i + 1; j < ord.size(); j++) {
        if (ord[j].cant > ord[min].cant) {
          min = j;
        }
      }
      if (min != i) {
        tnd temp = ord[i];
        ord[i] = ord[min];
        ord[min] = temp;
      }
    }

    for (int i = 0; i < ord.size(); i++){
      ui->listWidgetTend->addItem( ord[i].hash + "\t" + "\t"+ QString::number(ord[i].cant));
    }






    //Tendencias Fin



    //LISTA DE CONTACTOS

    QFile file("Contactos.csv");
    if(file.open(QIODevice::ReadOnly)){ //WriteOnly
        QTextStream in(&file); // in << palabras[1] << "\n";
        while(!in.atEnd()){
            QString linea = in.readLine();
            QStringList palabras = linea.split(",");
            Contacto nuevo(palabras[0],palabras[1],palabras[2]);
            lcont.append(nuevo);

            BST_Cont_CName->add(nuevo);
            BST_Cont_Number->add(nuevo);
            BST_Cont_Apodo->add(nuevo);
        }
        file.close();
    }




    act_cont();

    //Imprimir publicaciones
    while (!Publi.empty()){
      ui->listWidgetPubli->addItem( Publi.head()->get_nombre() + "\t"+Publi.head()->get_titulo());
      ui->listWidgetPubli->addItem( Publi.head()->get_hashtag());
      ui->listWidgetPubli->addItem( Publi.head()->get_texto());
      ui->listWidgetPubli->addItem(" ");
      Publi.dequeue();
    }

    limcont = 10;

    //ARBOL CONTACTO
    this->ventAC.BST_Cont_Apodo = this->BST_Cont_Apodo;
    this->ventAC.BST_Cont_CName = this->BST_Cont_CName;
    this->ventAC.BST_Cont_Number = this->BST_Cont_Number;

    this->ventAC.lista = this->ui->listWidgetCont;
    this->ventAC.limcont = limcont;


    connect(ui->btnCamFoto,SIGNAL(released()),this,SLOT(cambiar_imagen()));

    connect(ui->btnPublicar,SIGNAL(released()),this,SLOT(new_publi()));

    connect(ui->btnAgregar,SIGNAL(released()),this,SLOT(add_contact()));

    connect(ui->btnInvertir, SIGNAL(released()), this, SLOT(f_invertir()));

    connect(ui->BCnom,SIGNAL(released()),this,SLOT(act_cont_ANom()));
    connect(ui->BCnum,SIGNAL(released()),this,SLOT(act_cont_ANum()));
    connect(ui->BCapod,SIGNAL(released()),this,SLOT(act_cont_AAp()));
}


void Principal::act_tend(){

    ui->listWidgetTend->clear();

    ListaT *tempLT = new ListaT;

    for(int i=0;i<lst->size();i++){
        tempLT->addFirst(lst->get(i));
    }

    vector<int> cont;

    for (int i = 0; i < tempLT->size(); i++){
        int n = 1;
        cont.push_back(n);

          for (int j = tempLT->size(); j > i;j--){
            if(tempLT->get(i) == tempLT->get(j)){
              tempLT->removePos(j);
              cont[i]++;
              j=tempLT->size();
            }
          }
    }

    //MOSTRAR TENDENCIAS

    QVector<tnd> ord;

    for (int i = 0; i < tempLT->size(); i++){

        tnd temptend(tempLT->get(i),cont[i]);
        ord.append(temptend);
    }

    for (int i = 0; i < ord.size() - 1; i++) {
      int min = i;
      for (int j = i + 1; j < ord.size(); j++) {
        if (ord[j].cant > ord[min].cant) {
          min = j;
        }
      }
      if (min != i) {
        tnd temp = ord[i];
        ord[i] = ord[min];
        ord[min] = temp;
      }
    }

    for (int i = 0; i < ord.size(); i++){
      ui->listWidgetTend->addItem( ord[i].hash + "\t" + "\t"+ QString::number(ord[i].cant));
    }

}



void Principal::show_user_info(){

    ui->lblUsuario->clear();
    ui->lblUsuario->setText("Usuario:\n" + *Unombre + "\n" + *UfechaR);
    show_followed_users();
}

void Principal::cambiar_imagen(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"C:\\",tr("Image Files(*.png *.pjg *.bmp)"));
    if(filename!=""){
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

    Post* result = newPost->new_post;
    if (result == nullptr)
    {
        return;
    }
    else
    {
        if (result->title != "" && result->content != "")
        {
            main_instance->addPost(main_instance->logged_user->id, result->title, result->content);

            if ((main_instance->get_shown_user() != nullptr) && (main_instance->get_shown_user()->id == main_instance->logged_user->id))
            {
                main_instance->set_shown_user(main_instance->logged_user);
                show_all_posts(op_busq, invertir, true);
            }
        }
    }
}

void Principal::act_cont(){

    ui->listWidgetCont->clear();
    lcont.selected = lcont.head;
    int contador = 0;
    for(int i =0;i<lcont.getCount();i++){
        if(contador<10){
            ui->listWidgetCont->addItem(lcont.selected->data.toString());
        }
        contador++;

        /*
        if(lcont.selected->data.get_comp_name().size()<=16){
            ui->listWidgetCont->addItem(lcont.selected->data.get_comp_name() + "\t \t" + lcont.selected->data.get_numero() + "\t" +lcont.selected->data.get_apodo() );
        } else {
            ui->listWidgetCont->addItem(lcont.selected->data.get_comp_name() + "\t" + lcont.selected->data.get_numero() + "\t" +lcont.selected->data.get_apodo() );
        }
        */
       lcont.sgt();
    }




}




void Principal::act_cont_AAp(){
    ui->listWidgetCont->clear();

    if(invertir){
            BST_Cont_Apodo->inorder(ui->listWidgetCont, limcont);
                this->ventAC.ord = 0;
    } else{
         BST_Cont_Apodo->postorder(ui->listWidgetCont,limcont);
          this->ventAC.ord = 1;
    }

    invertir = !invertir;

}

void Principal::act_cont_ANom(){
    ui->listWidgetCont->clear();
    if(invertir){
            BST_Cont_CName->inorder(ui->listWidgetCont,limcont);
                this->ventAC.ord = 2;
    } else{
         BST_Cont_CName->postorder(ui->listWidgetCont,limcont);
          this->ventAC.ord = 3;
    }

    invertir = !invertir;

}
void Principal::act_cont_ANum(){


    ui->listWidgetCont->clear();
    if(invertir){
            BST_Cont_Number->inorder(ui->listWidgetCont,limcont);
                this->ventAC.ord = 4;
    } else{
         BST_Cont_Number->postorder(ui->listWidgetCont,limcont);
          this->ventAC.ord = 5;
    }

    invertir = !invertir;
}


void Principal::add_contact(){
    ventAC.ptrLC = &lcont;
    ventAC.show();
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
