#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Graf/graf.hpp"
#include <cmath>

#include <QFileDialog>
#include <QMessageBox>

Graf *sc;

extern Cvor *izabran_cvor;
extern Ivica *izabrana_ivica;

extern bool animacija;
extern bool izvrsavanje;

QImage tegic_slika;
QImage protok_slika;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    tegic_slika = QImage(":/res/tegici.png");
    protok_slika = QImage(":/res/protok.png");

    sc = new Graf();
    ui->setupUi(this);
    sc->setSceneRect(0,0,ui->graphicsView->geometry().height(),ui->graphicsView->geometry().height());
    ui->graphicsView->setAcceptDrops(true);
    ui->graphicsView->setScene(sc);

    ui->actionBiranje->setChecked(true);


    connect(sc,&Graf::sig_cvor,this,&MainWindow::izaberi_cvor);
    connect(sc,&Graf::sig_ivica,this,&MainWindow::izaberi_ivicu);
    connect(sc,&Graf::sig_nista,this,&MainWindow::izaberi_prazno);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::kraj_animacije()
{
    ui->actionStop->setEnabled(false);
    ui->actionPauziraj->setEnabled(false);
}

void MainWindow::boji_cvor(Cvor *cv, QColor col)
{
    if (cv!=nullptr){
        QPen p =cv->pen();
        p.setColor(col);
        cv->setPen(p);
    }
}

void MainWindow::boji_ivicu(Ivica *iv, QColor col)
{
    if (iv!=nullptr){
        QPen p =iv->pen();
        p.setColor(col);
        iv->setPen(p);
        iv->strelica.setPen(p);
        iv->strelica.setBrush(QBrush(p.color()));
    }
}


void MainWindow::on_actionBiranje_triggered()
{
    sc->postavi_stanje(Biranje);
    ui->actionCvorovi->setChecked(false);
    ui->actionIvice->setChecked(false);
}


void MainWindow::on_actionCvorovi_triggered()
{
    sc->postavi_stanje(Dodaj_cvor);
    ui->actionBiranje->setChecked(false);
    ui->actionIvice->setChecked(false);
}


void MainWindow::on_actionIvice_triggered()
{
    sc->postavi_stanje(Dodaj_ivicu);
    ui->actionCvorovi->setChecked(false);
    ui->actionBiranje->setChecked(false);
}

void MainWindow::izaberi_cvor(Cvor *cvor)
{

    ui->stackedWidget_2->setEnabled(true);
    izabrana_ivica = nullptr;
    izabran_cvor = cvor;

    ui->stackedWidget_2->setCurrentIndex(0);

    ui->velicinaCvoraDoubleSpinBox->setValue(izabran_cvor->rect().width());
    ui->velicinaTekstaSpinBox->setValue(izabran_cvor->velicina_teksta);
    ui->ignorisiCheckBox_2->setChecked(izabran_cvor->ignorisi);
    ui->debljinaPrstenaDoubleSpinBox->setValue(izabran_cvor->pen().widthF());
    ui->indeksCvoraLineEdit->setText(QString::number(izabran_cvor->id));
    ui->nazivCvoraLineEdit->setText(izabran_cvor->naziv);

    if(ui->metodaComboBox->currentIndex()==5){
        int ind = 0;
        for(auto c:sc->cvorovi){
            if(c==cvor)
                break;
            ind ++;
        }
        ui->pocetniCvorComboBox_2->setCurrentIndex(ind);
    }

}

void MainWindow::izaberi_prazno()
{
    izabran_cvor = nullptr;
    izabrana_ivica = nullptr;
    ui->stackedWidget_2->setEnabled(false);

}

void MainWindow::izaberi_ivicu(Ivica *ivica)
{

    izabran_cvor = nullptr;
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->stackedWidget_2->setEnabled(true);
    ui->koristiTezineCheckBox->setChecked(izabrana_ivica->koristi_tezine);
    ui->tezinaDoubleSpinBox->setValue(izabrana_ivica->tezina);
    ui->protokDoubleSpinBox->setValue(izabrana_ivica->protok);
    ui->dotokDoubleSpinBox->setValue(izabrana_ivica->dotok);
    ui->ignorisiCheckBox_2->setChecked(izabrana_ivica->ignorisi);
    ui->velicinaTekstaSpinBox_2->setValue(izabrana_ivica->tekst.font().pointSize());
    ui->debljinaLinijeDoubleSpinBox->setValue(izabrana_ivica->pen().widthF());
    if(izabrana_ivica->tip==Protocna){
        ui->tipIviceComboBox->setCurrentIndex(1);
    } else
        ui->tipIviceComboBox->setCurrentIndex(0);
    ui->usmerenostComboBox->setItemText(1,ivica->prvi->tekst.text() + " ka " +ivica->drugi->tekst.text());
    ui->usmerenostComboBox->setItemText(2,ivica->drugi->tekst.text() + " ka " +ivica->prvi->tekst.text());


    if(izabrana_ivica->usmeren_ka == nullptr)
        ui->usmerenostComboBox->setCurrentIndex(0);
    if(izabrana_ivica->usmeren_ka == izabrana_ivica->prvi)
        ui->usmerenostComboBox->setCurrentIndex(2);
    if(izabrana_ivica->usmeren_ka == izabrana_ivica->drugi)
        ui->usmerenostComboBox->setCurrentIndex(1);
}




void MainWindow::on_metodaComboBox_currentIndexChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
    if(ui->metodaComboBox->currentIndex()==4){
        ui->pocetniCvorComboBox->clear();
        ui->ciljniCvorComboBox->clear();

        for(auto c:sc->cvorovi){
            ui->pocetniCvorComboBox->addItem(c->naziv);
            ui->ciljniCvorComboBox->addItem(c->naziv);
        }
    }
    if(ui->metodaComboBox->currentIndex()==5){
        ui->pocetniCvorComboBox_2->clear();

        for(auto c:sc->cvorovi){
            ui->pocetniCvorComboBox_2->addItem(c->naziv);
        }
    }

    if(ui->metodaComboBox->currentIndex()==8){
        for(auto c:sc->cvorovi){
            ui->izvorComboBox->addItem(c->naziv);
            ui->ponorComboBox->addItem(c->naziv);
        }
    }
}



void MainWindow::on_actionPusti_triggered()
{

    sc->ocisti_animacije();

    Algoritam *alg = new Algoritam(this);

    alg->bfs_nivoi = ui->pisiNivoeCheckBox->isChecked();
    alg->pocetak = izabran_cvor;
    alg->method = ui->metodaComboBox->currentIndex();
    alg->g = sc;
    animacija = false;

    if (alg->method == 0 || alg->method == 1){
        if (izabran_cvor!=nullptr){
            ui->pocetniCvorLineEdit->setText(izabran_cvor->tekst.text());
            ui->pocetniCvorLineEdit_2->setText(izabran_cvor->tekst.text());
        }
        else {
            ui->pocetniCvorLineEdit->setText("nista nije izabrano!");
            ui->pocetniCvorLineEdit_2->setText("nista nije izabrano!");
        }
    }
    if ( alg->method == 3){
        alg->labeliraj_cvorove = ui->labelirajCvoroveCheckBox->isChecked();
        alg->labeliraj_ivice = ui->labelirajIviceCheckBox_2->isChecked();
    }
    alg->belezi_stepene = ui->obeleziStepeneCheckBox->isChecked();
    alg->labeliraj_ivice = ui->labelirajIviceCheckBox->isChecked();

    connect(alg,&Algoritam::boji_cvor,this,&MainWindow::boji_cvor);
    connect(alg,&Algoritam::boji_ivica,this,&MainWindow::boji_ivicu);
    connect(alg, &Algoritam::finished, alg, &Algoritam::deleteLater);
    connect(alg, &Algoritam::kraj_animacije, this, &MainWindow::kraj_animacije);
    connect(alg, &Algoritam::stavi_tekst_cvor, this, &MainWindow::stavi_tekst_cvor);
    connect(alg, &Algoritam::animiraj_ivicu, this, &MainWindow::animiraj_ivicu);
    connect(alg, &Algoritam::error_box, this, &MainWindow::error_box);
    connect(alg, &Algoritam::transp_cvor_sve, this, &MainWindow::transp_cvor_sve);
    connect(alg, &Algoritam::stavi_tekst_ivica, this, &MainWindow::stavi_tekst_ivica);
    connect(alg, &Algoritam::transp_ivicu, this, &MainWindow::transp_ivicu);

    alg->start();
}


void MainWindow::on_actionAnimacija_triggered()
{

    sc->ocisti_animacije();


    animacija = true;

    ui->actionStop->setEnabled(true);
    ui->actionPauziraj->setEnabled(true);

    Algoritam *alg = new Algoritam(this);

    alg->bfs_nivoi = ui->pisiNivoeCheckBox->isChecked();
    alg->pocetak = izabran_cvor;
    alg->method = ui->metodaComboBox->currentIndex();
    alg->brzina = ui->animacijaSpinBox->value();
    alg->g = sc;
    alg->belezi_mostove = ui->obeleziMostoveCheckBox->isChecked();
    alg->belezi_stepene = ui->obeleziStepeneCheckBox->isChecked();
    alg->labeliraj_ivice = ui->labelirajIviceCheckBox->isChecked();

    if (alg->method == 0 || alg->method == 1){
        if (izabran_cvor!=nullptr){
            ui->pocetniCvorLineEdit->setText(izabran_cvor->tekst.text());
            ui->pocetniCvorLineEdit_2->setText(izabran_cvor->tekst.text());
        }
        else {
            ui->pocetniCvorLineEdit->setText("nista nije izabrano!");
            ui->pocetniCvorLineEdit_2->setText("nista nije izabrano!");
        }
    }

    if(ui->metodaComboBox->currentIndex()==8){
        alg->izvor = sc->cvorovi[ui->izvorComboBox->currentIndex()];
        alg->ponor = sc->cvorovi[ui->ponorComboBox->currentIndex()];

    }

    if ( alg->method == 3){
        alg->labeliraj_cvorove = ui->labelirajCvoroveCheckBox->isChecked();
        alg->labeliraj_ivice = ui->labelirajIviceCheckBox_2->isChecked();
    }

    if ( alg->method == 4){
        alg->labeliraj_cvorove = ui->labelirajCvoroveDistancomCheckBox->isChecked();
        alg->obidji_ceo_graf = ui ->obidjiCeoGrafCheckBox->isChecked();
        alg->pocetak = sc->cvorovi[ui->pocetniCvorComboBox->currentIndex()];
        alg->meta = sc->cvorovi[ui->ciljniCvorComboBox->currentIndex()];
    }

    if ( alg->method == 4){
        alg->labeliraj_cvorove = ui->labelirajDistanceCheckBox->isChecked();
        alg->pocetak = sc->cvorovi[ui->pocetniCvorComboBox->currentIndex()];
    }

    connect(alg,&Algoritam::boji_cvor,this,&MainWindow::boji_cvor);
    connect(alg,&Algoritam::boji_ivica,this,&MainWindow::boji_ivicu);
    connect(alg, &Algoritam::finished, alg, &Algoritam::deleteLater);
    connect(alg, &Algoritam::kraj_animacije, this, &MainWindow::kraj_animacije);
    connect(alg, &Algoritam::stavi_tekst_cvor, this, &MainWindow::stavi_tekst_cvor);
    connect(alg, &Algoritam::animiraj_ivicu, this, &MainWindow::animiraj_ivicu);
    connect(alg, &Algoritam::error_box, this, &MainWindow::error_box);
    connect(alg, &Algoritam::transp_cvor_sve, this, &MainWindow::transp_cvor_sve);
    connect(alg, &Algoritam::stavi_tekst_ivica, this, &MainWindow::stavi_tekst_ivica);
    connect(alg, &Algoritam::transp_ivicu, this, &MainWindow::transp_ivicu);
    connect(alg, &Algoritam::anim_boji_cvor, this, &MainWindow::anim_boji_cvor);
    connect(alg, &Algoritam::okviri_cvor, this, &MainWindow::okviri_cvor);
    connect(alg, &Algoritam::dodaj_text, this, &MainWindow::dodaj_text);
    connect(alg, &Algoritam::pomeri_cvor, this, &MainWindow::pomeri_cvor);
    connect(alg, &Algoritam::dodaj_anim_cvor, this, &MainWindow::dodaj_anim_cvor);
    connect(alg, &Algoritam::dodaj_anim_ivicu, this, &MainWindow::dodaj_anim_ivicu);
    alg->start();

}


void MainWindow::on_velicinaTekstaSpinBox_valueChanged(int arg1)
{
    izabran_cvor->velicina_teksta = arg1;
    QFont f= izabran_cvor->tekst.font();
    f.setPointSize(arg1);
    izabran_cvor->tekst.setFont(f);
}


void MainWindow::on_debljinaPrstenaDoubleSpinBox_valueChanged(double arg1)
{
    izabran_cvor->velicina_prstena = arg1;
    QPen p = izabran_cvor->pen();
    p.setWidthF(arg1);
    izabran_cvor->setPen(p);
}


void MainWindow::on_velicinaCvoraDoubleSpinBox_valueChanged(double arg1)
{
    izabran_cvor->setRect(izabran_cvor->rect().x(),izabran_cvor->rect().y(),arg1,arg1);
}


void MainWindow::on_tezinaDoubleSpinBox_valueChanged(double arg1)
{
    izabrana_ivica->tezina = arg1;
    izabrana_ivica->postavi_tekst(QString::number(arg1));
}


void MainWindow::on_protokDoubleSpinBox_valueChanged(double arg1)
{
    izabrana_ivica->protok = arg1;
    if(izabrana_ivica->tip==Protocna)
        izabrana_ivica->postavi_tekst(QString::number(izabrana_ivica->dotok)+"/"+QString::number(izabrana_ivica->protok));
}


void MainWindow::on_dotokDoubleSpinBox_valueChanged(double arg1)
{
    izabrana_ivica->dotok = arg1;
    izabrana_ivica->a_tezina = arg1;
    if(izabrana_ivica->tip==Protocna)
        izabrana_ivica->postavi_tekst(QString::number(izabrana_ivica->dotok)+"/"+QString::number(izabrana_ivica->protok));
}

void MainWindow::stavi_tekst_cvor(Cvor *cv, QString txt)
{
    cv->tekst.setText(txt);
    cv->azuriraj_poziciju_ivica();
}

void MainWindow::animiraj_ivicu(Ivica *iv, Cvor *p, Cvor *d, QColor col, float param)
{
    iv->anim_line.setOpacity(1);
    iv->anim_line.show();
    QPen pn = iv->pen();
    pn.setColor(col);

    pn.setWidthF(pn.widthF()+2);
    iv->anim_line.setPen(pn);


    if(iv->usmeren!=Sam_u_sebe){

        float x1 = p->pos().x() + 25;
        float y1 = p->pos().y() + 25;

        float x2 = d->pos().x() + 25;
        float y2 = d->pos().y() + 25;

        float dx = x2 - x1;
        float dy = y2 - y1;

        float dnorm = sqrtf(dx * dx + dy * dy);

        dx /= dnorm;
        dy /= dnorm;

        float x_poc = x1 + dx*25;
        float y_poc = y1 + dy*25;

        float x_kraj = x2 - dx*25;
        float y_kraj = y2 - dy*25;

        float vec_x = x_kraj - x_poc;
        float vec_y = y_kraj - y_poc;


        QPainterPath pp;
        pp.moveTo(x1 + dx*25 , y1 + dy*25);
        pp.lineTo(x_poc + param*vec_x , y_poc + param*vec_y);
        iv->anim_line.setPath(pp);
    } else {
        QPainterPath pp;

        float sx = -sqrtf(2.0)/2.0;
        float sy = -sqrtf(2.0)/2.0;

        float px = iv->prvi->pos().x() + 25;
        float py = iv->prvi->pos().y() + 25;

        QPointF pr = QPointF(px + sx*25,py + sy*25);
        pp.moveTo(pr);
        pp.arcTo(pr.x()-15,pr.y()-15, 30, 30, 30, param*200.0);

        iv->anim_line.setPath(pp);
    }

    if(param >=0.90){
        QPen pn2 = iv->strelica.pen();
        QBrush bn2 = iv->strelica.brush();
        pn2.setColor(col);
        bn2.setColor(col);
        iv->strelica.setPen(pn2);
        iv->strelica.setBrush(bn2);
        iv->strelica.setOpacity(1);
    }
}


void MainWindow::on_koristiTezineCheckBox_stateChanged(int arg1)
{
    izabrana_ivica->postavi_koristi_tezine(arg1);
}


void MainWindow::on_usmerenostComboBox_currentIndexChanged(int index)
{
    if(index == 0){
        izabrana_ivica -> usmeren = Neusmeren;
        izabrana_ivica -> usmeren_ka = nullptr;
        izabrana_ivica -> azuriraj_poziciju();
    }
    if(index == 1){
        izabrana_ivica -> usmeren = Usmeren;
        izabrana_ivica -> usmeren_ka = izabrana_ivica->drugi;
        izabrana_ivica -> azuriraj_poziciju();
    }
    if(index == 2){
        izabrana_ivica -> usmeren = Usmeren;
        izabrana_ivica -> usmeren_ka = izabrana_ivica->prvi;
        izabrana_ivica -> azuriraj_poziciju();
    }
    izabrana_ivica -> azuriraj_poziciju();
}


void MainWindow::on_actionOcisti_triggered()
{
    sc->ocisti_animacije();
}


void MainWindow::on_actionIzbrisi_triggered()
{
    sc->izbrisi_sve();
}


void MainWindow::on_skalaDoubleSpinBox_valueChanged(double arg1)
{
}

void MainWindow::error_box(QString txt)
{
    QMessageBox b;
    b.setText(txt);
    b.exec();
}

void MainWindow::transp_cvor_sve(Cvor *c, float val)
{
    c->setOpacity(val);
    c->tekst.setOpacity(val);
    auto susedi = c->nadji_par_susede();
    for(auto s:susedi){
        s.second->setOpacity(val);
        s.second->tekst.setOpacity(val);
        s.second->strelica.setOpacity(val);
    }
}

void MainWindow::stavi_tekst_ivica(Ivica *iv, QString txt)
{
    iv->koristi_tezine = true;
    iv->tekst.setText(txt);
    iv->azuriraj_poziciju();
    iv->tekst.show();
}

void MainWindow::transp_ivicu(Ivica *iv, float val)
{
    iv->setOpacity(val);
    iv->tekst.setOpacity(val);
    iv->anim_line.setOpacity(val);
    iv->strelica.setOpacity(val);
}

void MainWindow::anim_boji_cvor(Cvor *cv,QColor col1,QColor col2,float param)
{
    float dx = col2.red() - col1.red();
    float dy = col2.green() - col1.green();
    float dz = col2.blue() - col1.blue();

    QPen p = cv->pen();
    p.setColor(QColor::fromRgb(col1.red() + dx*param,col1.green()+dy*param,col1.blue()+dz*param));
    cv->setPen(p);
}

void MainWindow::okviri_cvor(Cvor *cv,QRectF rc,float len, float param)
{
    if(param>1)
        param = 1.0-param;
    param = sin(param);

    float nov_w = rc.width() + param*len;
    float nov_h = rc.height() + param*len;

    float d = (param*len)/2.0;

    cv->setRect(rc.x()-d,rc.y()-d,nov_w,nov_h);
}

void MainWindow::dodaj_text(QString txt)
{
    ui->textBrowser->append(txt);
}




void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->clear();
}


void MainWindow::on_nazivCvoraLineEdit_textChanged(const QString &arg1)
{
    if(izabran_cvor!=nullptr){
        izabran_cvor->naziv = arg1;
        izabran_cvor->tekst.setText(arg1);
        izabran_cvor->azuriraj_poziciju_ivica();
    }
}

void MainWindow::dodaj_anim_cvor(Cvor *c)
{
    sc->anim_cvorovi.push_back(c);

    sc->dodaj_cvor(c);
}

void MainWindow::dodaj_anim_ivicu(Ivica *i)
{
    sc->ivice.push_back(i);
    sc->anim_ivice.push_back(i);

    i->roditelj = sc;

    QPen p;
    p.setWidth(5);
    i->setPen(p);

    sc->addItem(i);
     sc->addItem(&(i->anim_line));
     sc->addItem(&(i->tekst));
     sc->addItem(&i->strelica);
     i->azuriraj_poziciju();
}

void MainWindow::pomeri_cvor(Cvor *c, QPointF p)
{
   c->pomeri(p);
}


void MainWindow::on_tipIviceComboBox_currentIndexChanged(int index)
{
    if(index==1){
        izabrana_ivica->tip = Protocna;
        izabrana_ivica->postavi_koristi_tezine( true);
        izabrana_ivica->postavi_tekst(QString::number(izabrana_ivica->dotok)+"/"+QString::number(izabrana_ivica->protok));
        izabrana_ivica->azuriraj_poziciju();
    } else {
        izabrana_ivica->tip = Obicna;
        izabrana_ivica->postavi_tekst(QString::number(izabrana_ivica->tezina));
        izabrana_ivica->azuriraj_poziciju();
    }
}


void MainWindow::on_actionUcitaj_graf_triggered()
{
    std::string putanja = QFileDialog::getOpenFileName(nullptr,"Izaberite graf").toStdString();
    sc->ucitaj_graf(putanja);
}


void MainWindow::on_actionSacuvaj_graf_triggered()
{
    std::string putanja = QFileDialog::getSaveFileName(this,"Sacuvaj fajl").toStdString();
    sc->sacuvaj_graf(putanja);
}

