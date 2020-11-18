#include "principal.h"
#include "ui_principal.h"
#include <time.h>
#include "viewpost.h"
#include "newpost.h"
#include "postui.h"
#include <QFileDialog>
#include <QFile>
#include <QTextCodec>

#include <QRandomGenerator>

bool invertir = false;

void Principal::show_all_posts()
{
    function<void(int)> show_post = [this](int id)
    {
        Post post_to_show = *main_instance->getPost(id);

        if (post_to_show.id != -1)
        {
            ViewPost *view_post_dialog = new ViewPost();
            view_post_dialog->set_current_post(post_to_show);
            view_post_dialog->exec();
        }
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
    //ui->listWidgetPubli->clear();

    list<Post*> posts = main_instance->getAllPosts();
    int maxPostsToShow = 50;

    for (Post* post : posts)
    {
         //ui->listWidgetGroup->addItem(post.content.c_str());
        add_item_to_list_widget(ui->listWidgetPubli, *post, show_post);

        maxPostsToShow--;

        if (maxPostsToShow == 0)
            break;
        /*
        add_item_to_list_widget(ui->listPublications, post, false, false, show_post, edit_post, delete_post);

        if (post.author_id == main_instance->get_logged_user().id || main_instance->get_logged_user().is_admin)
        {
            add_item_to_list_widget(ui->listUserPosts, post, true, true, show_post, edit_post, delete_post);
        }
        */
    }
}

void Principal::add_item_to_list_widget(QListWidget *list, Post individual_post, function<void(int)> show_post)
{
    QListWidgetItem *item = new QListWidgetItem();
    list->addItem(item);

    PostUI *post_ui = new PostUI();

    post_ui->post_text->setText(individual_post.title.c_str());
    post_ui->post_id = individual_post.id;

    post_ui->set_view_button_click_action(show_post);

    item->setSizeHint(post_ui->minimumSizeHint());

    list->setItemWidget(item, post_ui);
}

Principal::Principal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Principal)
{
    setlocale(LC_ALL,"");
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //Arboles Contactos
    this->BST_Cont_Apodo = new BST<Contacto, QString>([](Contacto j){return j.get_apodo();});
    this->BST_Cont_CName = new BST<Contacto, QString>([](Contacto j){return j.get_comp_name();});
    this->BST_Cont_Number = new BST<Contacto, QString>([](Contacto j){return j.get_numero();});





    main_instance = new Hitagget();

    show_all_posts();

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



    /*

    ui->listWidgetCont->clear();

    for(int i =0;i<lcont.getCount();i++){
    ui->listWidgetCont->addItem(lcont.selected->data.get_comp_name() + "\t" + lcont.selected->data.get_numero() + "\t" +lcont.selected->data.get_apodo() );
    lcont.sgt();
    }
    */


    /*
    ifstream c("Contactos.csv");

    string Nombre_cont;
    string Apellido_cont;
    string Numero_cont;
    string Apodo_cont;

    if (c.good()){

        while(!c.eof()){
            c >> Nombre_cont>>Apellido_cont>>Numero_cont >> Apodo_cont;

            QString tempNC = QString::fromStdString(Nombre_cont);
            QString tempAC = QString::fromStdString(Apellido_cont);
            QString tempNumC = QString::fromStdString(Numero_cont);
            QString tempApC = QString::fromStdString(Apodo_cont);

            Contacto nuevo(tempNC, tempAC, tempNumC, tempApC);

            lcont.append(nuevo);
        }

    }

    ui->listWidgetCont->clear();

    for(int i =0;i<lcont.getCount();i++){
    ui->listWidgetCont->addItem(lcont.selected->data.get_nombre() + "\t" + lcont.selected->data.get_apellido() + "\t" + lcont.selected->data.get_numero() + "\t" +lcont.selected->data.get_apodo() );
    lcont.sgt();
    }
    */
    //LISTA DE CONTACTOS fin






    //Imprimir publicaciones
    while (!Publi.empty()){
      ui->listWidgetPubli->addItem( Publi.head()->get_nombre() + "\t"+Publi.head()->get_titulo());
      ui->listWidgetPubli->addItem( Publi.head()->get_hashtag());
      ui->listWidgetPubli->addItem( Publi.head()->get_texto());
      ui->listWidgetPubli->addItem(" ");
      Publi.dequeue();
    }


    //Grupos

    ifstream g("Grupos.txt");

    QVector<Grupo> Grupos;
    Grupo new_grupo;
    string Nombre_grupo;
    string Cant_grupo;
    string Desc_grupo;

    while (g.good()){

      g >> Nombre_grupo >> Cant_grupo>>Desc_grupo;

      QString tempNG = QString::fromStdString(Nombre_grupo);
      QString tempCG = QString::fromStdString(Cant_grupo);
      QString tempDG = QString::fromStdString(Desc_grupo);

      new_grupo = Grupo(tempNG,tempCG,tempDG);
      Grupos.push_back(new_grupo);

    }


    for(int i = 0;i<Grupos.size()-1;i++){
        ui->listWidgetGroup->addItem(Grupos[i].get_nombre());
        ui->listWidgetGroup->addItem(Grupos[i].get_Describcion());
        ui->listWidgetGroup->addItem(Grupos[i].get_n());
        ui->listWidgetGroup->addItem(" ");
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



void Principal::cambiar_nombre(){
    ui->lblUsuario->clear();
    ui->lblUsuario->setText("Usuario:\n" + *Unombre);
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
        //main_instance->add_post(*result);
        //show_all_posts();
    }
    /*
    QString text = ui->tePubli->toPlainText();

    if(ui->leTitleP->text() != "" && ui->leTag->text()!= "" && text != "" ){

            QString temphs = "#"+ui->leTag->text();

        lst->addLast(temphs);

        act_tend();

        //arreglar publi
        int tamL = 93;
        int cont = 0;
        int SizeLineaPubli = text.size();
        for (int i = 0; i < SizeLineaPubli;i++) {
            cont++;
            if (cont>tamL) {
                if (text[i] == ' ') {
                    text[i] = '\n';
                    cont=0;
                }
            }
        }

        string tempTitP = ui->leTitleP->text().toUpper().toStdString();
        string tempHS = ui->leTag->text().toStdString();
        string tempTexto = text.toStdString();

        fstream testeo;
        testeo.open("Publicaciones.txt", ios::out | ios::app);
        testeo<<endl;
        testeo<<(*Unombre).toStdString()<<endl;
        testeo<<tempTitP<<endl;
        testeo<<"#"<<tempHS<<endl;
        testeo<<tempTexto;
        testeo.close();

        //arreglar publi


    ui->listWidgetPubli->addItem(*Unombre + "\t"+ (ui->leTitleP->text()).toUpper());
    ui->listWidgetPubli->addItem(temphs);
    ui->listWidgetPubli->addItem(text);
    ui->listWidgetPubli->addItem(" ");

    ui->leTitleP->setText("");
    ui->leTag->setText("");
    ui->tePubli->setMarkdown("");


    }


*/
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

Principal::~Principal()
{
    delete ui;
}
