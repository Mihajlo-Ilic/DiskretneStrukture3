#include "graf.hpp"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>

#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <QMessageBox>
#include <numeric>
#include <cmath>

stanje_scene stanje = Biranje;

bool animacija = false;
bool izvrsavanje = false;

Cvor *izabran_cvor;
Ivica *izabrana_ivica;
QGraphicsLineItem *linija;

//koriste se za dodavanje ivice
Cvor *prvi;
Cvor *drugi;

extern QImage tegic_slika;
extern QImage protok_slika;

Dugmic *teg;
Dugmic *protoc;


Graf::Graf()
{
    linija = new QGraphicsLineItem();
    addItem(linija);
    QPen p;
    p.setColor(QColor::fromRgb(170,0,0));
    p.setWidth(5);
    linija->setPen(p);
    linija->hide();

    teg = new Dugmic(tip_dugmeta::Tegic);
    addItem(teg);
    teg->setScale(0.2);
    teg->hide();

    protoc = new Dugmic(tip_dugmeta::Protocni);
    addItem(protoc);
    protoc->setScale(0.2);
    protoc->hide();
}


void Graf::postavi_stanje(stanje_scene st)
{
    stanje = st;
}

bool kliknut_cvor = false;

void Graf::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if (stanje == Biranje){

        if (izabran_cvor!=nullptr){
            QPen p=izabran_cvor->pen();
            p.setColor(QColor::fromRgb(0,0,0));
            izabran_cvor->setPen(p);
        }

        if (izabrana_ivica!=nullptr){
            QPen p=izabrana_ivica->pen();
            p.setColor(QColor::fromRgb(0,0,0));
            izabrana_ivica->setPen(p);
            izabrana_ivica->azuriraj_poziciju();
        }


        izabran_cvor = nullptr;
        izabrana_ivica = nullptr;

        prvi = nullptr;
        drugi = nullptr;
        linija->hide();
        emit sig_nista();
    }




    //trenutni hak smislicu bolje kasnije sad mora ovako sa brojacem klika da bi radilo
    if(stanje==Dodaj_ivicu && !kliknut_cvor){

    }

    QGraphicsScene::mousePressEvent(e);

    kliknut_cvor = false;

    if (stanje == Dodaj_cvor)
        dodaj_cvor(e->scenePos());
}

void Graf::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene::mouseMoveEvent(e);
    if(stanje==Dodaj_ivicu && prvi!=nullptr){
        linija->show();
        float x = prvi->pos().x()+25;
        float y = prvi->pos().y()+25;
        float vec_x = e->scenePos().x() - x;
        float vec_y = e->scenePos().y() - y;
        float dnorm = sqrtf(vec_x*vec_x + vec_y*vec_y);
        vec_x /= dnorm;
        vec_y /= dnorm;

        linija->setLine(x+25*vec_x,y+25*vec_y,e->scenePos().x(),e->scenePos().y());
    }
}

void Graf::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    QGraphicsScene::mouseReleaseEvent(e);
    if(stanje==Dodaj_ivicu && prvi!=nullptr && drugi!=nullptr){
        dodaj_ivicu();
        prvi = nullptr;
        drugi = nullptr;
        linija->hide();
    }
}

void Graf::dodaj_cvor(QPointF mis)
{
    unsigned id = nadji_id();
    Cvor* cvor = new Cvor();
    cvor->id = id;
    cvor->roditelj = this;

    this->addItem(cvor);
    this->addItem(&(cvor->tekst));

    QPen p;
    p.setWidthF(2);

    QFont f;
    f.setPointSize(15);
    cvor->tekst.setFont(f);

    cvor->tekst.setPen(p);
    if(cvor->naziv=="")
        cvor->tekst.setText(QString::number(id));

    QPointF pos = mis;
    pos.rx() -= 25;
    pos.ry() -= 25;

    float d = (50 - cvor->tekst.boundingRect().width())/2.0;
    float h = (50 - cvor->tekst.boundingRect().height())/2.0;

    cvor->setPos(pos);
    pos.rx() += d;
    pos.ry() += h;
    cvor->tekst.setPos(pos);
    cvor->naziv = cvor->tekst.text();
    cvorovi.push_back(cvor);
    cvor->azuriraj_poziciju_ivica();
}

void Graf::dodaj_cvor(Cvor *c)
{
    unsigned id = nadji_id();
    c->id = id;
    c->roditelj = this;

    this->addItem(c);
    this->addItem(&(c->tekst));

    QPen p;
    p.setWidthF(2);

    QFont f;
    f.setPointSize(15);
    c->tekst.setFont(f);

    c->tekst.setPen(p);

    QPointF pos = c->pos();

    float d = (50 - c->tekst.boundingRect().width())/2.0;
    float h = (50 - c->tekst.boundingRect().height())/2.0;

    pos.rx() += d;
    pos.ry() += h;
    c->tekst.setPos(pos);
    cvorovi.push_back(c);
    c->azuriraj_poziciju_ivica();
}

void Graf::dodaj_ivicu()
{
    Ivica *iv = new Ivica(prvi,drugi);
    ivice.push_back(iv);
    iv->azuriraj_poziciju();

    iv->roditelj = this;

    QPen p;
    p.setWidth(5);
    iv->setPen(p);

    addItem(iv);
    addItem(&(iv->anim_line));
    addItem(&(iv->tekst));
    addItem(&iv->strelica);
}

unsigned Graf::nadji_id()
{
    //trazimo maksimalan id
    //posle prolazimo kroz sve brojeve do maxa
    //ako neki od njih nije zauzet vratimo taj id

    unsigned id = 0;
    unsigned max_id = 0;
    for(auto c:cvorovi)
        if(c->id>id)
            max_id = c->id;
    for(unsigned i=0;i<max_id+2;i++){
        bool postoji = false;
        for(auto c:cvorovi)

            if (c->id == i)
                postoji = true;
            if(!postoji){
                id = i;
                break;
            }
        }
    return id;
}

void Graf::BFS(Cvor *pocetni,bool nivoi = false)
{
    std::queue<std::pair<Cvor *,int>> red;

    red.push(std::make_pair(pocetni,0));

    std::set<Cvor *> obidjeni;
    int i = 0;

    QPen p = pocetni->pen();
    p.setColor(QColor::fromRgb(200,0,0));
    pocetni->setPen(p);
    pocetni->tekst.setText(QString::number(i));
    i = 1;
    obidjeni.insert(pocetni);
    while(red.empty()==false){
        auto vrh = red.front();
        red.pop();

        auto susedi = vrh.first->nadji_susede();
        for (auto sused : susedi){
            if (obidjeni.find(sused)==obidjeni.end()){


                QPen p = sused->pen();
                p.setColor(QColor::fromRgb(100,0,200));
                sused->setPen(p);
                if (!nivoi)
                    sused->tekst.setText(QString::number(i));
                else
                    sused->tekst.setText(QString::number(vrh.second+1));

                i++;

                red.push(std::make_pair(sused,vrh.second+1));
                obidjeni.insert(sused);
            }
        }
    }
}

void Graf::ocisti_animacije()
{
    for(auto e:ivice){
        e->anim_line.hide();
        QPen p =e->pen();
        p.setColor(QColor::fromRgb(0,0,0));
        e->setPen(p);
        e->setOpacity(1);
        e->tekst.setOpacity(1);
        e->strelica.setOpacity(1);
        e->strelica.setPen(p);
        QBrush b = e->strelica.brush();
        b.setColor(p.color());
        e->strelica.setBrush(b);
        e->dotok = e->a_tezina;
    }

    for(auto c:cvorovi){

        QPen p =c->pen();
        p.setColor(QColor::fromRgb(0,0,0));
        c->setPen(p);
        c->tekst.setText(c->naziv);
        c->setOpacity(1);
        c->tekst.setOpacity(1);
    }
}

void Graf::izbrisi_sve()
{
    for (auto e:ivice){
        removeItem(e);
        removeItem(&e->anim_line);
        removeItem(&e->strelica);
        removeItem(&e->tekst);
    }

    for (auto c:cvorovi){
        removeItem(c);
        removeItem(&c->tekst);
    }

    ivice.clear();
    cvorovi.clear();
}

#include <fstream>

void Graf::sacuvaj_graf(string &putanja)
{
    std::ofstream file(putanja);
    for(auto cv:cvorovi){
        if(std::find(anim_cvorovi.begin(),anim_cvorovi.end(),cv)==anim_cvorovi.end()){
            std::string str;
            str += "-v ";
            str += std::to_string(cv->pos().x());
            str += " ";
            str += std::to_string(cv->pos().y());
            str += " ";
            str += cv->naziv.toStdString();
            file << str <<"\n";
        }
    }

    for(auto ev:ivice){
        if(std::find(anim_ivice.begin(),anim_ivice.end(),ev)==anim_ivice.end()){
            std::string str;
            str += "-e ";
            unsigned ind_prvi = std::distance(cvorovi.begin(),std::find(cvorovi.begin(),cvorovi.end(),ev->prvi));
            unsigned ind_drugi = std::distance(cvorovi.begin(),std::find(cvorovi.begin(),cvorovi.end(),ev->drugi));

            str += std::to_string(ind_prvi);
            str += " ";
            str += std::to_string(ind_drugi);
            str += " ";

            if(ev->usmeren_ka!=nullptr){
                unsigned ind_usmeren = std::distance(cvorovi.begin(),std::find(cvorovi.begin(),cvorovi.end(),ev->usmeren_ka));
                str += std::to_string(ind_usmeren);
                str += " ";
            } else {
                str += "null ";
            }

            if(ev->tip == Protocna){
                str += "p ";
                if(ev->koristi_tezine){
                    str += "t ";
                } else
                    str += "n ";
                str += std::to_string(ev->dotok) + "/" + std::to_string(ev->protok);
            } else {
                str += "o ";
                if(ev->koristi_tezine){
                    str += "t ";
                } else
                    str += "n ";
                str += std::to_string(ev->tezina);
            }

            file << str << "\n";

        }
    }

    file.close();

}

void Graf::ucitaj_graf(string &putanja)
{
    izbrisi_sve();
    std::ifstream file(putanja);
    if(file.is_open()){
        std::string linija;
        while (std::getline(file,linija)) {
            if(linija[1]=='v'){
                float x;
                float y;

                char cime[50];

                sscanf(linija.c_str(),"%*s %f %f %s",
                       &x,&y,cime);
                std::string ime = std::string(cime);
                Cvor *c = new Cvor();
                dodaj_cvor(c);
                c->naziv = QString::fromStdString(ime);
                c->setPos(QPointF(x,y));
                c->tekst.setText(c->naziv);
                c->azuriraj_poziciju_ivica();

            } else {
                unsigned pprvi;
                unsigned ddrugi;


                char cusmeren[50];
                char ctip[50];
                char ctt[50];
                char ctezina[50];

                sscanf(linija.c_str(),"%*s %u %u %s %s %s %s",
                       &pprvi,&ddrugi,cusmeren,ctip,ctt,ctezina);

                std::string usmeren = std::string(cusmeren);
                std::string tip = std::string(ctip);
                std::string tt = std::string(ctt);
                std::string tezina = std::string(ctezina);


                Ivica *iv = new Ivica(cvorovi[pprvi],cvorovi[ddrugi]);
                iv->usmeren = Neusmeren;
                iv->tip=Obicna;
                iv->koristi_tezine = false;
                iv->usmeren_ka = nullptr;

                addItem(iv);
                addItem(&(iv->anim_line));
                addItem(&(iv->tekst));
                addItem(&iv->strelica);

                QPen p;
                p.setWidth(5);
                iv->setPen(p);

                if(usmeren != "null"){
                    unsigned ind = std::stoul(usmeren);
                    iv->usmeren = Usmeren;
                    iv->usmeren_ka = cvorovi[ind];

                }

                if(pprvi==ddrugi)
                    iv->usmeren = Sam_u_sebe;

                if(tip == "p"){
                    iv->tip=Protocna;

                    iv->koristi_tezine = true;
                }

                if(tt == "t"){
                    iv->koristi_tezine = true;
                    iv->tekst.show();
                }

                if(tip == "o"){
                    float tez  = stof(tezina);
                    iv->tezina = tez;
                    iv->tekst.setText(QString::number(iv->tezina));
                } else {
                    float dot;
                    float prot;
                    sscanf(tezina.c_str(),"%f/%f",&dot,&prot);
                    iv->dotok = dot;
                    iv->protok = prot;
                    iv->tekst.setText(QString::number(iv->dotok)+"/"+QString::number(iv->protok));
                }


                ivice.push_back(iv);


                iv->roditelj = this;


                iv->azuriraj_poziciju();

            }
        }
    }
    file.close();
}

Cvor::Cvor():QGraphicsEllipseItem(0,0,50,50)
{
    QPen p;
    p.setWidthF(velicina_prstena);
    setPen(p);
    QBrush b = QBrush(QColor::fromRgb(255,255,255));
    b.setStyle(Qt::SolidPattern);
    setBrush(b);
    setZValue(1);
    tekst.setZValue(1.1);
}

void Cvor::pomeri(QPointF p)
{
    this->setPos(this->x()+p.x()-25,this->y()+p.y()-25);

    float d = (50 - tekst.boundingRect().width())/2.0;
    float h = (50 - tekst.boundingRect().height())/2.0;

    QPointF pp = pos();
    p.rx() += d;
    p.ry() += h;
    tekst.setPos(pp);

    azuriraj_poziciju_ivica();
}

void Cvor::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if(stanje==Biranje){

            if (e->button() == Qt::LeftButton)
                    drag_start = e->pos();

            if (izabran_cvor!=nullptr){
                QPen p=izabran_cvor->pen();
                p.setColor(QColor::fromRgb(0,0,0));
                izabran_cvor->setPen(p);
            }

            if (izabrana_ivica!=nullptr){
                QPen p=izabrana_ivica->pen();
                p.setColor(QColor::fromRgb(0,0,0));
                izabrana_ivica->setPen(p);
                izabrana_ivica->azuriraj_poziciju();
            }

            izabran_cvor = this;
            QPen p=izabran_cvor->pen();
            p.setColor(QColor::fromRgb(20, 150, 201));
            izabran_cvor->setPen(p);
            emit roditelj->sig_cvor(this);


    } else
    if (stanje == Dodaj_ivicu && prvi==nullptr){
        prvi = this;
        drag_start = QPointF(0,0);
        kliknut_cvor = true;
        e->ignore();
    }

}

void Cvor::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
        if (!(e->buttons() & Qt::LeftButton))
            return;
        if ((e->pos() - drag_start).manhattanLength()
             < QApplication::startDragDistance())
            return;
        if (stanje != Biranje)
            return;

        this->setPos(this->x()+e->pos().x()-25,this->y()+e->pos().y()-25);

        float d = (50 - tekst.boundingRect().width())/2.0;
        float h = (50 - tekst.boundingRect().height())/2.0;

        QPointF p = pos();
        p.rx() += d;
        p.ry() += h;
        tekst.setPos(p);

        azuriraj_poziciju_ivica();
}

void Cvor::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    if (stanje==Dodaj_ivicu && prvi!=nullptr){
        drugi = this;
    }
}

void Cvor::azuriraj_poziciju_ivica()
{
    float d = (50 - tekst.boundingRect().width())/2.0;
    float h = (50 - tekst.boundingRect().height())/2.0;

    QPointF p = pos();
    p.rx() += d;
    p.ry() += h;
    tekst.setPos(p);

    for (auto e : ivice)
        e->azuriraj_poziciju();
}

std::vector<Cvor *> Cvor::nadji_susede()
{
    std::vector<Cvor *> susedi;

    for (Ivica* e:ivice){
        if(e->usmeren==Neusmeren){
            if(e->prvi!=this)
                susedi.push_back(e->prvi);
            else
                susedi.push_back(e->drugi);
        } else {
            if (e->prvi!=this && e->usmeren_ka==drugi)
                susedi.push_back(e->prvi);
            else if(e->drugi!=this && e->usmeren_ka==prvi)
                susedi.push_back(e->drugi);
        }
    }
    return susedi;
}

std::vector<std::pair<Cvor *, Ivica *> > Cvor::nadji_par_susede()
{
    std::vector<std::pair<Cvor *,Ivica *>> susedi;

    for (Ivica* e:ivice){
        if (e->usmeren==Neusmeren){
            if(e->prvi!=this)
                susedi.push_back(std::make_pair(e->prvi,e));
            else
                susedi.push_back(std::make_pair(e->drugi,e));
        } else {
            if(e->usmeren_ka!=this && e->usmeren_ka==e->prvi)
                susedi.push_back(std::make_pair(e->prvi,e));
            else if(e->usmeren_ka!=this && e->usmeren_ka==e->drugi)
                susedi.push_back(std::make_pair(e->drugi,e));
        }
    }
    return susedi;
}

std::vector<std::pair<Cvor *, Ivica *> > Cvor::nadji_ed_susede()
{
    std::vector<std::pair<Cvor *,Ivica *>> susedi;

    for (Ivica* e:ivice){
            if(e->prvi!=this)
                susedi.push_back(std::make_pair(e->prvi,e));
            else
                susedi.push_back(std::make_pair(e->drugi,e));

    }

    std::sort(susedi.begin(),susedi.end(),[&](std::pair<Cvor *,Ivica *> &x,std::pair<Cvor *,Ivica *> &y){
        return x.first->naziv.toStdString()<=y.first->naziv.toStdString();
    });

    return susedi;
}

std::vector<std::pair<Cvor *, Ivica *> > Cvor::nadji_par_susede2()
{
    std::vector<std::pair<Cvor *,Ivica *>> susedi;

    for (Ivica* e:ivice){
        if (e->usmeren == Sam_u_sebe)
            susedi.push_back(std::make_pair(e->drugi,e));
        if (e->usmeren==Neusmeren){
            if(e->prvi!=this)
                susedi.push_back(std::make_pair(e->prvi,e));
            else
                susedi.push_back(std::make_pair(e->drugi,e));
        } else {
            if(e->usmeren_ka!=this && e->usmeren_ka==e->prvi)
                susedi.push_back(std::make_pair(e->prvi,e));
            else if(e->usmeren_ka!=this && e->usmeren_ka==e->drugi)
                susedi.push_back(std::make_pair(e->drugi,e));
        }
    }
    return susedi;
}

Ivica::Ivica(Cvor *prvi, Cvor *drugi)
{
    this->prvi = prvi;
    this->drugi = drugi;

    if(prvi == drugi)
        usmeren = Sam_u_sebe;

    prvi->ivice.push_back(this);
    drugi->ivice.push_back(this);

    tekst.setText(QString::number(0));
    tekst.hide();
    strelica.hide();

    QPen p;
    p.setWidthF(1);

    QFont f;
    f.setPointSize(15);
    tekst.setPen(p);
    tekst.setFont(f);
    azuriraj_poziciju();
}

void Ivica::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if(stanje==Biranje && e->button() == Qt::LeftButton){

        if (izabran_cvor!=nullptr){
            QPen p=izabran_cvor->pen();
            p.setColor(QColor::fromRgb(0,0,0));
            izabran_cvor->setPen(p);
        }

        if (izabrana_ivica!=nullptr){
            QPen p=izabrana_ivica->pen();
            p.setColor(QColor::fromRgb(0,0,0));
            izabrana_ivica->setPen(p);
            izabrana_ivica->azuriraj_poziciju();
        }

        izabrana_ivica = this;
        QPen p=izabrana_ivica->pen();
        p.setColor(QColor::fromRgb(20, 150, 201));
        izabrana_ivica->setPen(p);
        izabrana_ivica->azuriraj_poziciju();
        emit roditelj->sig_ivica(this);
     }
}

void Ivica::azuriraj_poziciju()
{



    if(usmeren != Sam_u_sebe){
        float x1 = prvi->pos().x() + 25;
        float y1 = prvi->pos().y() + 25;

        float x2 = drugi->pos().x() + 25;
        float y2 = drugi->pos().y() + 25;

        float dx = x2 - x1;
        float dy = y2 - y1;

        float dnorm = sqrtf(dx * dx + dy * dy);

        dx /= dnorm;
        dy /= dnorm;

        QPainterPath pp;
        pp.moveTo(x1 + dx*25 , y1 + dy*25);
        pp.lineTo(x2 - dx*25 , y2 - dy*25);
        setPath(pp);



        if(usmeren == Usmeren && usmeren_ka!=nullptr){

            QPointF begin;

            if (usmeren_ka==prvi)
                begin = QPointF(x1 + dx*25 , y1 + dy*25);
            else
                begin = QPointF(x2 - dx*25 , y2 - dy*25);

            QPointF end;
            if (usmeren_ka == prvi)
                end = QPointF(x2 - dx*25 , y2 - dy*25);
            else
                end = QPointF(x1 + dx*25 , y1 + dy*25);

            QPointF vec = end - begin;
            QPointF vec_n = QPointF(-vec.ry(),vec.rx());

            float len = sqrtf(QPointF::dotProduct(vec,vec));
            float scale = 20/len;

            float len_n = sqrtf(QPointF::dotProduct(vec_n,vec_n));
            float scale_n = 10/len_n;

            QPolygonF pol;
            QPointF s=begin+vec*scale;

            QPointF l=s+vec_n*scale_n;
            QPointF d=s-vec_n*scale_n;
            pol.push_back(begin);
            pol.push_back(l);
            pol.push_back(d);

            auto boja = pen().color();
            QBrush brush(boja);
            brush.setStyle(Qt::BrushStyle::SolidPattern);
            strelica.setBrush(brush);
            strelica.setPolygon(pol);
            strelica.show();

        }
        else {
            strelica.hide();
        }
    } else {
        strelica.show();

        QPainterPath pp;

        float sx = -sqrtf(2.0)/2.0;
        float sy = -sqrtf(2.0)/2.0;

        float px = prvi->pos().x() + 25;
        float py = prvi->pos().y() + 25;

        QPointF pr = QPointF(px + sx*25,py + sy*25);
        pp.moveTo(pr);
        pp.arcTo(pr.x()-15,pr.y()-15, 30, 30, 30, 200.0);

        setPath(pp);

        QPointF tacka = QPointF(pp.elementAt(pp.elementCount()-1).x,pp.elementAt(pp.elementCount()-1).y);
        QPolygonF pol;
        pol.push_back(tacka);
        QPointF centar = QPointF(px,py+20);
        QPointF vec = tacka-centar;
        QPointF vecn = QPointF(-vec.y(),vec.x());

        float s1 = QPointF::dotProduct(vec,vec);
        float s2 = QPointF::dotProduct(vecn,vecn);
        vec = QPointF(vec.x()/s1,vec.y()/s1);
        vecn = QPointF(vecn.x()/s2,vecn.y()/s2);
        QPointF sr = QPointF(tacka.x()+300*vec.x(),tacka.y()+300*vec.y());
        QPointF l = QPointF(sr.x()+300*vecn.x(),sr.y()+300*vecn.y());
        QPointF d = QPointF(sr.x()-300*vecn.x(),sr.y()-300*vecn.y());
        pol.push_back(l);
        pol.push_back(d);
        auto boja = pen().color();
        QBrush brush(boja);
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        strelica.setBrush(brush);
        strelica.setPolygon(pol);
        show();
    }
    show();
    if(koristi_tezine){
        postavi_tekst(tekst.text());

    }

    if(izabrana_ivica == this){
        teg->azuriraj_poziciju(this);
        teg->hide();
    }

}

void Ivica::postavi_tekst(QString txt)
{
    o_tekst = txt;

    QPointF pp = path().pointAtPercent(0.5);

    tekst.setText(txt);
    float width = tekst.boundingRect().width();
    float height = tekst.boundingRect().height();

    if(usmeren != Sam_u_sebe && prvi!=drugi){

        float x1 = path().elementAt(0).x;
        float y1 = path().elementAt(0).y;

        float x2 = path().elementAt(1).x;
        float y2 = path().elementAt(1).y;

        float dx = x2 - x1;
        float dy = y2 - y1;

        float dnorm = sqrtf(dx * dx + dy * dy);

        dx /= dnorm;
        dy /= dnorm;

        QPointF kr1 = QPointF(x1 + dx*25 , y1 + dy*25);
        QPointF kr2 = QPointF(x2 - dx*25 , y2 - dy*25);

        float angle = atan2(kr2.x()-kr1.x(),kr2.y()-kr1.y());
        angle *= 180 / 3.14;
        QPointF vec;

        if(angle>0)
            vec = QPointF(kr2.x()-kr1.x(),kr2.y()-kr1.y());
        else
            vec = QPointF(kr1.x()-kr2.x(),kr1.y()-kr2.y());

        vec = QPointF(-vec.y(),vec.x());
        float s = QPointF::dotProduct(vec,vec);
        vec = QPointF(vec.x()/s,vec.y()/s);

        tekst.setPos(pp.x()+width+2*width*vec.x(),pp.y()+2*(height/2.0)*vec.y());

        if(tekst.collidesWithItem(this)){
            tekst.setPos(pp.x()-2*width-width*vec.x(),pp.y()+2*(height/2.0)*vec.y());
        }

    }
     else {
        tekst.setPos(pp.x()-(width+10),pp.y()-height);
    }
}

void Ivica::postavi_koristi_tezine(bool t)
{
    if(t){
    tekst.show();
    koristi_tezine = true;
    postavi_tekst(o_tekst);
    } else {
        tekst.hide();
        koristi_tezine = false;
        postavi_tekst(o_tekst);
    }
}

Algoritam::Algoritam(QObject *parent):QThread(parent)
{

}

void Algoritam::bfs()
{

    if (pocetak!=nullptr){

        std::queue<std::pair<Cvor *,int>> red;

        red.push(std::make_pair(pocetak,0));

        std::set<Cvor *> obidjeni;
        int i = 0;

        emit boji_cvor(pocetak,QColor::fromRgb(200,0,0));

        emit stavi_tekst_cvor(pocetak,QString::number(i));
        i = 1;
        obidjeni.insert(pocetak);
        while(red.empty()==false){
            auto vrh = red.front();
            red.pop();

            auto susedi = vrh.first->nadji_par_susede();
            for (auto sused : susedi){
                if (obidjeni.find(sused.first)==obidjeni.end()){
                    if(animacija)
                        for(int i=0;i<brzina;i++){
                            QThread::msleep(5);
                            emit animiraj_ivicu(sused.second,vrh.first,sused.first,
                                                QColor::fromRgb(31, 194, 219),float(i)/float(brzina-1));
                        }

                    if(animacija)
                        QThread::msleep(10);
                    emit boji_cvor(sused.first,QColor::fromRgb(103, 214, 24));

                    if (!bfs_nivoi)

                        emit stavi_tekst_cvor(sused.first,QString::number(i));
                    else
                        emit stavi_tekst_cvor(sused.first,QString::number(vrh.second+1));


                    i++;

                    red.push(std::make_pair(sused.first,vrh.second+1));
                    obidjeni.insert(sused.first);
                }
            }
        }
    }
    emit kraj_animacije();
}

void Algoritam::dfs()
{
    if (pocetak!=nullptr){
        std::stack<Cvor *> stek;
        //pamtimo odakle smo dosli da bi bojili
        std::stack<std::pair<Cvor *,Ivica *>> roditelji;
        std::set<Cvor *> iskorisceni;

        stek.push(pocetak);

        emit boji_cvor(pocetak,QColor::fromRgb(200,0,0));

        emit stavi_tekst_cvor(pocetak,QString::number(0));

        int i = 0;


        while (stek.empty()==false) {

            auto vrh = stek.top();
            std::pair<Cvor *,Ivica *> roditelj;

            if(vrh!=pocetak){
               roditelj = roditelji.top();
                  roditelji.pop();
            }

            stek.pop();

            if(iskorisceni.find(vrh)!=iskorisceni.end())
                continue;

            iskorisceni.insert(vrh);

            if(vrh!=pocetak){


                if(animacija)
                    for(int i=0;i<brzina;i++){
                        QThread::msleep(5);
                        emit animiraj_ivicu(roditelj.second,roditelj.first,vrh,
                                            QColor::fromRgb(31, 194, 219),float(i)/float(brzina));
                    }

                if(animacija)
                    QThread::msleep(10);
                emit boji_cvor(vrh,QColor::fromRgb(103, 214, 24));
                i++;
                emit stavi_tekst_cvor(vrh,QString::number(i));
            }

            auto susedi = vrh->nadji_par_susede();
            for (auto sused : susedi){
                if (iskorisceni.find(sused.first)==iskorisceni.end()){

                    stek.push(sused.first);
                    roditelji.push(std::make_pair(vrh,sused.second));
                }

        }

        }
    }
    emit kraj_animacije();
}

void most_bfs(Cvor *pocetni,std::map<Cvor*,bool>& helper,Cvor *c){
    if(pocetni!=c){
        std::queue<Cvor* > red;
        red.push(pocetni);
        helper[pocetni]=true;
        while (red.empty()==false) {
            auto vrh = red.front();
            red.pop();

            auto susedi = vrh->nadji_par_susede();
            for(auto s:susedi){
                if(helper[s.first]==false){
                    if(c==nullptr){
                        red.push(s.first);
                        helper[s.first]=true;
                    } else {
                        if(s.first!=c){
                            red.push(s.first);
                            helper[s.first]=true;
                        }
                    }
                }
            }
        }
    }
}

bool jel_most(Graf *g,Cvor *c){

    std::map<Cvor*,bool> helper;
    int br_comp = 0;

    for(auto gc:g->cvorovi)
        helper.insert(std::make_pair(gc,false));

    for(int i=0;i<g->cvorovi.size();i++){
        if(helper[g->cvorovi[i]]==false){
            br_comp += 1;
            most_bfs(g->cvorovi[i],helper,nullptr);
        }
    }

    std::map<Cvor*,bool> helper2;
    for(auto gc:g->cvorovi)
        helper2.insert(std::make_pair(gc,false));

    int br_comp2 = 0;
    for(int i=0;i<g->cvorovi.size();i++){
        if(helper2[g->cvorovi[i]]==false && g->cvorovi[i]!=c){
            br_comp2 += 1;
            most_bfs(g->cvorovi[i],helper2,c);
        }
    }
    return br_comp!=br_comp2;
}

std::set<Cvor *> nadji_mostove(Graf *g){
    std::set<Cvor*> res;
    for(auto c:g->cvorovi)
        if(jel_most(g,c))
            res.insert(c);
    return res;
}

std::pair<int,int> stepen(Graf *g,Cvor *c)
{
    std::vector<std::pair<Cvor *,Ivica *>> susedi;
    int in = 0;
    int out = 0;
    for (Ivica* e:g->ivice){
        if (e->usmeren==Neusmeren){
                if(e->prvi == c && e->drugi!=c){
                in +=1;
                out += 1;
                } else
                if(e->prvi != c && e->drugi==c){
                in +=1;
                out += 1;
                }
        } else {
            if(e->usmeren_ka!=c && e->usmeren_ka==e->prvi && e->drugi ==c)
                out += 1;
            else if(e->usmeren_ka!=c && e->usmeren_ka==e->drugi && e->prvi==c)
                out += 1;
            else if(e->usmeren_ka==c && e->usmeren_ka==e->drugi && e->prvi!=c)
                in += 1;
            else if(e->usmeren_ka==c && e->usmeren_ka==e->prvi && e->drugi!=c)
                in += 1;
        }
    }
    return std::make_pair(in,out);
}

std::set<Cvor*> nadji_neparne(Graf *g){
    std::set<Cvor*> res;

    for(auto c:g->cvorovi){
        auto p = stepen(g,c);
        if(abs(p.first-p.second)==1)
            res.insert(c);
        else
           if(p.first!=p.second)
               res.insert(c);
           if(p.first==p.second && p.first%2==1)
               res.insert(c);
    }
    return res;
}



void most_bfs2(Cvor *pocetni,std::map<Cvor*,bool>& helper,Cvor *c,std::set<Ivica *>& used){
    if(pocetni!=c){
        std::queue<Cvor* > red;
        red.push(pocetni);
        helper[pocetni]=true;
        while (red.empty()==false) {
            auto vrh = red.front();
            red.pop();

            auto susedi = vrh->nadji_par_susede();
            for(auto s:susedi){
                if(used.find(s.second)==used.end())
                if(helper[s.first]==false){
                    if(c==nullptr){
                        red.push(s.first);
                        helper[s.first]=true;
                    } else {
                        if(s.first!=c){
                            red.push(s.first);
                            helper[s.first]=true;
                        }
                    }
                }
            }
        }
    }
}

bool jel_most2(Graf *g,Cvor *c,std::set<Ivica *>& used){

    std::map<Cvor*,bool> helper;
    int br_comp = 0;

    for(auto gc:g->cvorovi)
        helper.insert(std::make_pair(gc,false));

    for(int i=0;i<g->cvorovi.size();i++){
        if(helper[g->cvorovi[i]]==false){
            br_comp += 1;
            most_bfs2(g->cvorovi[i],helper,nullptr,used);
        }
    }

    std::map<Cvor*,bool> helper2;
    for(auto gc:g->cvorovi)
        helper2.insert(std::make_pair(gc,false));

    int br_comp2 = 0;
    for(int i=0;i<g->cvorovi.size();i++){
        if(helper2[g->cvorovi[i]]==false && g->cvorovi[i]!=c){
            br_comp2 += 1;
            most_bfs2(g->cvorovi[i],helper2,c,used);
        }
    }
    return br_comp!=br_comp2;
}

void bfs_povezan_iv(Cvor *c,std::set<Cvor *>& helper,std::set<Ivica *>& used){
    std::queue<Cvor *> red;
    red.push(c);

    while (red.empty()==false) {
        auto vrh = red.front();
        red.pop();

        auto susedi = vrh->nadji_par_susede();
        for(auto s:susedi)
            if(helper.find(s.first)==helper.end() && used.find(s.second)!=used.end()){
                helper.insert(s.first);
                red.push(s.first);
            }
    }
}

bool jako_povezan_iv(Graf *g,std::set<Ivica *>& used){
    for(auto c:g->cvorovi){
        std::set<Cvor *> helper;
        helper.insert(c);
        bfs_povezan_iv(c,helper,used);
        if(helper.size()!=g->cvorovi.size())
            return false;
    }
    return true;
}

bool jel_ivica_most(Graf *g,Ivica *iv,std::set<Ivica *>& used){
    auto r1 = jako_povezan_iv(g,used);
    used.insert(iv);
    auto r2 = jako_povezan_iv(g,used);
    used.erase(iv);
    return r1!=r2;
}

std::set<Cvor *> nadji_mostove2(Graf *g,std::set<Ivica *>& used){
    std::set<Cvor*> res;
    for(auto c:g->cvorovi)
        if(jel_most2(g,c,used))
            res.insert(c);
    return res;
}

void bfs_povezan(Cvor *c,std::set<Cvor *>& helper){
    std::queue<Cvor *> red;
    red.push(c);

    while (red.empty()==false) {
        auto vrh = red.front();
        red.pop();

        auto susedi = vrh->nadji_par_susede();
        for(auto s:susedi)
            if(helper.find(s.first)==helper.end()){
                helper.insert(s.first);
                red.push(s.first);
            }
    }
}

bool jako_povezan(Graf *g){
    for(auto c:g->cvorovi){
        std::set<Cvor *> helper;
        helper.insert(c);
        bfs_povezan(c,helper);
        if(helper.size()!=g->cvorovi.size())
            return false;
    }
    return true;
}

void Algoritam::fleury()
{
    auto neparni = nadji_neparne(g);

    for(auto c:neparni)
        emit boji_cvor(c,QColor::fromRgb(255,0,255));

    for(auto c:g->cvorovi){
        auto p = stepen(g,c);
        emit stavi_tekst_cvor(c,QString::number(p.first)+"/"+QString::number(p.second));
    }

    if(jako_povezan(g)==false){
        emit error_box("Graf nije jako povezan!");
        emit kraj_animacije();
        return;
    }

    Cvor *tekuci = pocetak;

    if(!(neparni.size()==0 || neparni.size()==2)){
        emit error_box("Ne postoje 2 cvora neparnog stepena u grafu!");
        return;
    } else {
        if(neparni.size()==2)
            tekuci = *neparni.begin();
        if(neparni.size()==0 && pocetak==nullptr)
            tekuci = g->cvorovi[0];
    }

    std::set<Ivica *> koriscene;
    while (true) {
        auto susedi = tekuci->nadji_par_susede();

        if(susedi.size()==0)
            return;
        Ivica *iv = nullptr;
        Cvor *cv = nullptr;
        for(int i=0;i<susedi.size();i++){
            if(belezi_mostove && koriscene.find(susedi[i].second)==koriscene.end() && jel_ivica_most(g,susedi[i].second,koriscene))
                emit boji_ivica(susedi[i].second,QColor::fromRgb(255,0,0));


            if(neparni.find(susedi[i].first)==neparni.end() &&
                    koriscene.find(susedi[i].second)==koriscene.end() &&
                    jel_ivica_most(g,susedi[i].second,koriscene)==false){
                    iv = susedi[i].second;
                    cv = susedi[i].first;
            }
        }

        if(iv==nullptr){
            for(int i=0;i<susedi.size();i++){
                if(neparni.find(susedi[i].first)==neparni.end() &&
                        koriscene.find(susedi[i].second)==koriscene.end() &&
                        jel_ivica_most(g,susedi[i].second,koriscene)==true){
                        iv = susedi[i].second;
                        cv = susedi[i].first;
                }
            }
        }

        if(iv==nullptr){
            for(int i=0;i<susedi.size();i++){
                if(neparni.find(susedi[i].first)!=neparni.end() &&
                        koriscene.find(susedi[i].second)==koriscene.end()){
                        iv = susedi[i].second;
                        cv = susedi[i].first;
                }
            }
        }
        if(iv==nullptr){
            emit kraj_animacije();
            return;
        }
        if(animacija)
            for(int i=0;i<brzina;i++){
                QThread::msleep(5);
                emit animiraj_ivicu(iv,tekuci,cv,QColor::fromRgb(31, 194, 219),float(i)/float(brzina-1));
            }
        else
            emit boji_ivica(iv,QColor::fromRgb(255,0,0));
        tekuci = cv;
        koriscene.insert(iv);
    }

}
//--------------------------------------
//--------- FLERIJEV ALGORITAM ---------
//--------------------------------------
//proveravamo da li je di graf jako povezan ili ne
//da bi bio jako povezan od svakog cvora x mora da bude dostupan y i da vazi obrnuto tj tranzitivnost

void bfs_povezanost(Cvor *poc,std::map<Cvor*,bool>& help,std::set<Ivica*>& used){
    std::queue<Cvor*> red;
    red.push(poc);
    help[poc] = true;

    while (red.empty()==false) {
        auto vrh = red.front();
        red.pop();

        auto susedi = vrh->nadji_par_susede();
        for(auto s:susedi){
            if(help.find(s.first)!=help.end()){
            if(help[s.first]==false && used.find(s.second)==used.end()){
                help[s.first] = true;
                red.push(s.first);
            }
            } else {
                if(used.find(s.second)==used.end()){
                    help.insert(std::make_pair(s.first,true));
                    red.push(s.first);
                }
            }
        }
    }
}

bool validan_cvor(Cvor *c,std::set<Ivica*>& used){
    auto sused = c->nadji_par_susede();
    if(sused.size()==0)
        return true;
    for(auto s:sused)
        if(used.find(s.second)==used.end())
            return true;
    return false;
}

bool graf_povezanost(Graf *g,std::set<Ivica*>& used){
    std::vector<int> tek;
    for(auto c:g->cvorovi){
        std::map<Cvor*,bool> help;
        bfs_povezanost(c,help,used);
        if(validan_cvor(c,used))
            tek.push_back(help.size());
    }
    if(tek.size()>0 && g->cvorovi.size()>0){
          int p = tek[0];
          return std::all_of(tek.begin(),tek.end(),[p](auto px){return px==p;});
    }

    return true;
}

std::pair<std::vector<Cvor*>,bool> uslov_digraf(Graf *g){
    std::pair<std::vector<Cvor*>,bool> res;
    res.second = true;
    for(auto c:g->cvorovi){
        std::pair<int,int> p = stepen(g,c);
        if(abs(p.first-p.second)==1){
            res.first.push_back(c);
        } else if(p.first!=p.second)
            res.second = false;
    }
    return res;
}

std::pair<std::vector<Cvor*>,bool> uslov_graf(Graf *g){
    std::pair<std::vector<Cvor*>,bool> res;
    res.second = true;
    for(auto c:g->cvorovi){
        std::pair<int,int> p = stepen(g,c);
        if(p.first % 2 ==1)
            res.first.push_back(c);
    }
    return res;
}


bool je_most(Graf *g,Ivica *iv,std::set<Ivica*>& used){

}

void Algoritam::fleury2()
{
    std::set<Ivica*> used;

    bool digraf = false;
    for(auto e:g->ivice)
        if(e->usmeren==Usmeren)
            digraf = true;

    if(belezi_mostove){
        for(auto c:g->cvorovi){
            std::pair<int,int> st= stepen(g,c);
            if(digraf)
                emit stavi_tekst_cvor(c,QString::number(st.first)+"/"+QString::number(st.second));
            else
                emit stavi_tekst_cvor(c,QString::number(st.first));
        }
    }

    if(graf_povezanost(g,used)==false){
        if(digraf)
            emit error_box("Graf nije jako povezan!");
        else
            emit error_box("Graf nema jednu komponentu povezanosti!");
        emit kraj_animacije();
        return;
    }
    std::pair<std::vector<Cvor*>,bool> tek;
    if(digraf){
        tek = uslov_digraf(g);
        if(tek.second==false){
            emit error_box("Postoje cvorovi kojima su ulazni i izlazni stepeni razliciti");
            emit kraj_animacije();
            return;
        }
        if(!(tek.first.size()==0 || tek.first.size()==2)){
            emit error_box("Postoji vise cvorova kojima su ulazni i izlazni stepen za 1 razliciti");
            emit kraj_animacije();
            return;
        }
    }
    else {
        tek = uslov_graf(g);
        if(!(tek.first.size()==0 || tek.first.size()==2)){
            emit error_box("Postoji vise cvorova neparnog stepena");
            emit kraj_animacije();
            return;
        }
    }

    Cvor *cur = pocetak;
    if(tek.first.size()==2)
        cur = tek.first[0];
    else
    if(pocetak==nullptr && g->cvorovi.size()>0 && tek.first.size()==0)
        cur = g->cvorovi[0];
    else{
        emit kraj_animacije();
        return;
    }
    std::vector<Ivica *> ciscenje;
    while (true) {
        auto susedi = cur->nadji_par_susede();
        //ako ivica nije koriscena i nije most i nije krajnja tacka idemo kroz nju
        Ivica *iv=nullptr;
        Cvor *cv=nullptr;

        bool a = true;
        for(auto s:susedi){
            if(used.find(s.second)==used.end())
                a= false;
        }
        if(a) break;

        for(auto c:ciscenje)
            emit boji_ivica(c,QColor::fromRgb(0,0,0));
        ciscenje.clear();

        for(auto s:susedi){
            if(used.find(s.second)==used.end()){
            used.insert(s.second);

            if(graf_povezanost(g,used)==false){
                emit boji_ivica(s.second,QColor::fromRgb(255,0,0));
                ciscenje.push_back(s.second);
            }

            if(tek.first.size()==2){
                if(tek.first[0]!=s.first && tek.first[1]!=s.first &&
                        graf_povezanost(g,used)==true){
                    iv = s.second;
                    cv = s.first;
                }
            } else {
                if(graf_povezanost(g,used)==true){
                    iv = s.second;
                    cv = s.first;
                }
            }
            used.erase(s.second);
            }
        }
        //ako ivica nije koriscena ali jeste most i nije kranja tacka moramo tuda
        if(iv==nullptr)
        for(auto s:susedi){
            if(used.find(s.second)==used.end()){
            used.insert(s.second);
            if(tek.first.size()==2){
                if(tek.first[0]!=s.first && tek.first[1]!=s.first &&
                        graf_povezanost(g,used)==false){
                    iv = s.second;
                    cv = s.first;
                }
            } else {
                if(graf_povezanost(g,used)==false){
                    iv = s.second;
                    cv = s.first;
                }
            }
            used.erase(s.second);
            }
        }
        //ako je kranja tacka nemamo kuda
        if(iv==nullptr)
        for(auto s:susedi){
            if(tek.first.size()==2){
                if((tek.first[0]!=s.first || tek.first[1]!=s.first) &&
                        used.find(s.second)==used.end()){
                    iv = s.second;
                    cv = s.first;
                }
            } else {
                if(used.find(s.second)==used.end()){
                    iv = s.second;
                    cv = s.first;
                }
            }
        }
        if(animacija)
            for(int i=0;i<brzina;i++){
                QThread::msleep(5);
                emit animiraj_ivicu(iv,cur,cv,QColor::fromRgb(31, 194, 219),float(i)/float(brzina-1));
            }
        else
            emit boji_ivica(iv,QColor::fromRgb(31, 194, 219));
        used.insert(iv);
        cur = cv;
    }
    emit kraj_animacije();
}

void kon_bfs(Cvor *c,std::map<Cvor*,bool>& obidjeni){
    std::queue<Cvor *> red;
    red.push(c);
    obidjeni[c] = true;

    while (red.empty() == false) {
        auto vrh = red.front();
        red.pop();

        auto susedi = vrh->nadji_par_susede();
        for(auto s:susedi){
            if(obidjeni[s.first]==false){
                obidjeni[s.first]=true;
                red.push(s.first);
            }
        }
    }
}

std::vector<std::vector<Cvor *>> komp_povezanosti(std::vector<Cvor *>& cvorovi,std::set<Ivica *>& used){
    std::vector<std::vector<Cvor *>> res;
    std::set<Cvor *> obidjeni;

    for(auto c:cvorovi){
        if(obidjeni.find(c)==obidjeni.end()){
            std::vector<Cvor *> komponenta;
            std::queue<Cvor *> red;
            red.push(c);
            obidjeni.insert(c);

            while (red.empty() == false) {
                auto vrh = red.front();
                red.pop();
                komponenta.push_back(vrh);

                auto susedi = vrh->nadji_par_susede();
                for(auto s:susedi){
                    if(obidjeni.find(s.first)==obidjeni.end() && used.find(s.second)==used.end()){
                        obidjeni.insert(s.first);
                        red.push(s.first);
                    }
                }
            }
            res.push_back(komponenta);
        }
    }


    return res;
}

bool konektovan(Graf *g){
    std::map<Cvor*,bool> obidjeni;
    for(auto c:g->cvorovi)
        obidjeni.insert(std::make_pair(c,false));
    kon_bfs(g->cvorovi[0],obidjeni);
    return std::any_of(obidjeni.begin(),obidjeni.end(),[](auto x){return x.second==false;})==true;
}

bool ima_grane(Cvor *c,std::set<Ivica *>& used){
    auto susedi = c->nadji_par_susede();
    for(auto p:susedi)
        if(used.find(p.second)==used.end())
            return true;
    return false;
}

bool jel_grana_most(Graf *g,Ivica *iv,std::set<Ivica *>& used){
    std::vector<Cvor *> validni;
    for(auto c:g->cvorovi)
        if(ima_grane(c,used))
            validni.push_back(c);

    auto komp1 = komp_povezanosti(validni,used);
    used.insert(iv);
    auto komp2 = komp_povezanosti(validni,used);
    used.erase(iv);
    return komp1.size() != komp2.size();
}

void Algoritam::fleury3()
{
    if(g->cvorovi.size()==0){
        emit kraj_animacije();
        return;
    }

    bool digraf = false;
    for(auto e:g->ivice)
        if(e->usmeren==Usmeren)
            digraf = true;

    if(digraf){
        emit error_box("greska : graf je usmeren!");
        emit kraj_animacije();
        return;
    }

    if(konektovan(g)){
        emit error_box("greska : graf nije konektovan!");
        emit kraj_animacije();
        return;
    }

    if(belezi_stepene){
        for(auto c:g->cvorovi){
            auto st = stepen(g,c);
            emit stavi_tekst_cvor(c,c->tekst.text()+"\ndeg="+QString::number(st.first));
        }
    }

    std::vector<Cvor *> neparni;
    for(auto c:g->cvorovi){
        auto st = stepen(g,c);
        if(st.first % 2 ==1)
            neparni.push_back(c);
    }

    if(!(neparni.size()==0 || neparni.size()==2)){
        emit error_box("greska : treba biti 0 ili 2 cvora neparnog stepena");
        emit kraj_animacije();
        return;
    }

    Cvor * cur = g->cvorovi[0];
    if(neparni.size()==2)
        cur = neparni[0];
    else if(pocetak!=nullptr)
        cur = pocetak;

    std::set<Ivica *> obidjene;
    int i = 1;
    while(ima_grane(cur,obidjene)){
        if(animacija)
        for(auto c:g->cvorovi)
            if(!ima_grane(c,obidjene))
                emit transp_cvor_sve(c,0.3);


        auto susedi = cur->nadji_par_susede2();

        if(belezi_mostove && animacija){
            std::vector<Ivica *> mostovi;
            for(auto s:susedi)
                if(obidjene.find(s.second)==obidjene.end() && jel_grana_most(g,s.second,obidjene))
                    mostovi.push_back(s.second);

            for(auto m:mostovi){
                std::vector<Cvor *> validni;
                for(auto c:g->cvorovi)
                    if(ima_grane(c,obidjene))
                        validni.push_back(c);


                emit boji_ivica(m,QColor::fromRgb(255,0,0));
                obidjene.insert(m);
                auto komp = komp_povezanosti(validni,obidjene);
                obidjene.erase(m);

                    for(auto v:komp[0])
                        emit boji_cvor(v,QColor::fromRgb(230, 190, 46));
                    for(auto v:komp[1])
                        emit boji_cvor(v,QColor::fromRgb(102, 212, 47));

                for(int i=0;i<brzina;i++)
                    QThread::msleep(5);

                emit boji_ivica(m,QColor::fromRgb(0,0,0));
                for(auto& k:komp){
                    for(auto v:k)
                        emit boji_cvor(v,QColor::fromRgb(0,0,0));
                }
            }
        }

        Ivica *pr = nullptr;
        Cvor *cv = nullptr;
        for(auto s:susedi){
            if(s.second->usmeren == Sam_u_sebe && obidjene.find(s.second)==obidjene.end()){
                pr = s.second;
                cv = s.first;
                break;
            }
        }

        if(pr == nullptr)
        for(auto s:susedi){
            if(obidjene.find(s.second)==obidjene.end() && jel_grana_most(g,s.second,obidjene)==false){
                if(neparni.size()!=0){
                    if(s.first!=neparni[0] && s.first!=neparni[1]){
                        pr = s.second;
                        cv = s.first;
                    }
                } else {
                    pr = s.second;
                    cv = s.first;
                }
            }
        }
        //ako nema nista osim mostova moramo to
        if(pr == nullptr)
        for(auto s:susedi){
            if(obidjene.find(s.second)==obidjene.end() && jel_grana_most(g,s.second,obidjene)==true){
                if(neparni.size()!=0){
                    if(s.first!=neparni[0] && s.first!=neparni[1]){
                        pr = s.second;
                        cv = s.first;
                    }
                } else {
                    pr = s.second;
                    cv = s.first;
                }
            }
        }
        //daj sta das sad
        if(pr == nullptr)
        for(auto s:susedi){
            if(obidjene.find(s.second)==obidjene.end()){
                pr = s.second;
                cv = s.first;
            }
        }

        obidjene.insert(pr);
        if(animacija)
            for(int i=0;i<brzina;i++){
                QThread::msleep(5);
                emit animiraj_ivicu(pr,cur,cv,QColor::fromRgb(31, 194, 219),float(i)/float(brzina-1));
            }
        else
            emit boji_ivica(pr,QColor::fromRgb(31, 194, 219));
        cur = cv;
        if(animacija)
        for(auto c:g->cvorovi)
            if(!ima_grane(c,obidjene))
                emit transp_cvor_sve(c,0.3);
        if(labeliraj_ivice)
            emit stavi_tekst_ivica(pr,QString::number(i));

        i++;
    }


    emit kraj_animacije();

}

bool kruskal_validan(Cvor *c,std::set<Ivica *>& korisceni){
    if(c->ivice.size()==0)
        return true;

    auto susedi = c->nadji_par_susede();

    for(auto s:susedi)
        if(korisceni.find(s.second)==korisceni.end())
            return true;

    return false;
}

bool dostizan(Cvor *p,Cvor *d,std::set<Ivica *>& used){
    std::queue<Cvor *> red;
    red.push(p);

    std::set<Cvor *> obidjeni;
    obidjeni.insert(p);

    if(p==d)
        return true;

    while (red.empty() == false) {
        auto vrh = red.front();
        red.pop();

        auto susedi = vrh->nadji_par_susede();
        for(auto s:susedi){
            if(obidjeni.find(s.first) == obidjeni.end() && used.find(s.second) == used.end()){
                obidjeni.insert(s.first);
                red.push(s.first);
                if(s.first == d)
                    return true;
            }
        }
    }
    return false;
}

QString pisi_kruskal_skup(std::set<Ivica*>& sk,QString slovo){
    QString res = slovo + "={";
    for(auto iv:sk)
        res.append("{"+iv->prvi->naziv+","+iv->drugi->naziv+"},");
    if(sk.size()>0)
        res.remove(res.size()-1,1);
    res.append("}");
    return res;
}

void Algoritam::kruskal()
{
    if(g->cvorovi.size()==0){
        emit kraj_animacije();
        return;
    }

    dodaj_text("----------Kruskal----------");

    bool digraf = false;
    for(auto e:g->ivice)
        if(e->usmeren==Usmeren)
            digraf = true;

    if(digraf){
        emit error_box("greska : graf je usmeren!");
        emit kraj_animacije();
        dodaj_text("Graf je usmeren pa ne radi kruskal");
        return;
    }

    std::set<Ivica *> korisceni;
    for(auto i:g->ivice){
        korisceni.insert(i);
        emit transp_ivicu(i,0.3);
    }

    for(auto c:g->cvorovi)
        emit boji_cvor(c,QColor::fromRgb(255,0,0));

    std::map<Cvor *,int> skupovi;
    std::map<int,QColor> boje;

    std::vector<Ivica *> tezine;

    QString f_skup = "F = {";

    std::set<Ivica *> lskup;
    std::set<Ivica *> fskup;

    for(auto i:g->ivice)
        if(i->usmeren!=Sam_u_sebe){
            tezine.push_back(i);
            fskup.insert(i);
        }

    int i=0;
    srand(time(0));
    for(auto c:g->cvorovi){
        srand(rand());
        skupovi[c]=i;

        boje[i]=QColor(rand()%255, rand()%255, rand()%255);
        emit boji_cvor(c,boje[i]);
        i++;

        if(labeliraj_cvorove)
            emit stavi_tekst_cvor(c,"{"+QString::number(i)+"}");
    }

    std::sort(tezine.begin(),tezine.end(),[](auto x,auto y){return x->tezina >= y->tezina;});
    std::set<Ivica *> temp;
    auto komp = komp_povezanosti(g->cvorovi,temp);
    int iter=0;
    while (true) {

        emit dodaj_text("trenutni skupovi:");
        auto sr = pisi_kruskal_skup(lskup,+"L");
        emit dodaj_text(sr);
        sr = pisi_kruskal_skup(fskup,+"F");
        emit dodaj_text(sr);

        //ako u jednoj komponenti povezanosti koja nije velicine 1 imaju 2 cvora razlicitog skupa
        //nije kraj

        bool kraj = true;
        for(auto &vec:komp){
            if(vec.size()>1){
                if(std::any_of(vec.begin(),vec.end(),[&](auto x){ return skupovi[x]!=skupovi[vec[0]];}))
                    kraj = false;

            }
        }


        if(kraj)
            break;

        Ivica *izabrana = nullptr;
        for(int i=0;i<tezine.size();i++){
            if(korisceni.find(tezine[i])!=korisceni.end() && !dostizan(tezine[i]->prvi,tezine[i]->drugi,korisceni)){
                izabrana = tezine[i];
            }
        }
        if(izabrana == nullptr){
            emit error_box("greska : nema ivica");
            emit kraj_animacije();
            return;
        }

        Cvor *p;
        Cvor *d;
        if(kruskal_validan(izabrana->prvi,korisceni)){
            p = izabrana->prvi;
            d = izabrana->drugi;
        } else {
            p = izabrana->drugi;
            d = izabrana->prvi;
        }

        if(animacija)
            for(int i=0;i<brzina;i++){
                QThread::msleep(5);
                emit animiraj_ivicu(izabrana,p,d,QColor::fromRgb(31, 194, 219),float(i)/float(brzina-1));
            }
        else
            emit boji_ivica(izabrana,QColor::fromRgb(31, 194, 219));
        emit transp_ivicu(izabrana,1);

        korisceni.erase(izabrana);

        lskup.insert(izabrana);
        fskup.erase(izabrana);
        emit dodaj_text("Dodajemo u L skup {"+izabrana->prvi->naziv+","+izabrana->drugi->naziv+"} i uklanjamo iz F");


        if(labeliraj_ivice)
            emit stavi_tekst_ivica(izabrana,izabrana->o_tekst + "("+QString::number(iter)+")");
        iter++;

        int skup = skupovi[p];
        int skup2 = skupovi[d];


        for(auto& x:skupovi){
            QColor boja = boje[x.second];
            QRectF rc = x.first->rect();

            if(x.second == skup2){
                x.second = skup;

                if(animacija){
                    for(int i=0;i<brzina;i++){
                        QThread::msleep(5);
                        emit anim_boji_cvor(x.first,boja,boje[x.second],float(i)/float(brzina));
                        emit okviri_cvor(x.first,rc,10,float(i)/float(brzina/2));
                    }
                    if(labeliraj_cvorove)
                        emit stavi_tekst_cvor(x.first,p->tekst.text());
                }
                else
                    emit boji_cvor(x.first,boje[x.second]);
            }
            emit okviri_cvor(x.first,rc,0,0);
        }

        for(auto iv:g->ivice){
            if(skupovi[iv->prvi]==skupovi[iv->drugi] && korisceni.find(iv)!=korisceni.end()
                    && fskup.find(iv)!=fskup.end()){
                emit dodaj_text("Brisemo iz F skupa {"+iv->prvi->naziv+","+iv->drugi->naziv+"} jer nije deo stabla");
                fskup.erase(iv);
            }
        }
        emit dodaj_text("");

    }

    emit kraj_animacije();
}

void Algoritam::dijsktra()
{

    if(pocetak!=nullptr && meta!=nullptr){

        std::set<Ivica *> koriscene;
        if(!dostizan(pocetak,meta,koriscene)){
            emit error_box("Cvor "+meta->naziv+" nije dostizan iz cvora "+pocetak->naziv);
            emit kraj_animacije();
            return;
        }

        emit boji_cvor(pocetak,QColor::fromRgb(0,0,255));
        emit boji_cvor(meta,QColor::fromRgb(255,0,0));

        std::set<Cvor *> komp;
        for(auto c:g->cvorovi){
            if(dostizan(pocetak,c,koriscene)){
                komp.insert(c);

                if(labeliraj_cvorove && animacija){
                    emit stavi_tekst_cvor(c,c->naziv+"\ninf");
                }

            }
        }

        std::set<Cvor *> obidjeni;
        std::map<Cvor *,float> udaljenosti;
        std::map<Cvor *,std::pair<Cvor *,Ivica *>> roditelji;

        for(auto c:g->cvorovi){
            udaljenosti.insert(std::make_pair(c,1000000));
        }

        udaljenosti[pocetak] = 0;

        roditelji[pocetak] = std::make_pair(nullptr,nullptr);

        while(true){

            bool kraj = true;
            for(auto c:komp)
                if(obidjeni.find(c)==obidjeni.end())
                    kraj = false;

            if(kraj)
                break;

            Cvor* it =nullptr;
            float m = 10000000;
            for(auto& s:komp){
                if(udaljenosti[s] <= m && obidjeni.find(s) == obidjeni.end()){
                    it = s;
                    m = udaljenosti[s];
                }
            }

            if(it==nullptr){
                emit error_box("greska u trazenju min elementa!");
                emit kraj_animacije();
            }

            obidjeni.insert(it);

            if(it==meta && !obidji_ceo_graf)
                break;

            QColor boja = it->pen().color();
            QRectF rc = it->rect();

            if(animacija){

                for(int i=0;i<brzina;i++){
                    QThread::msleep(5);

                    if(it != pocetak && it!=meta)
                        emit anim_boji_cvor(it,boja,QColor::fromRgb(161, 31, 148),float(i)/float(brzina));
                   else if(it == pocetak)
                        emit anim_boji_cvor(it,boja,QColor::fromRgb(16, 107, 235),float(i)/float(brzina));
                   else if(it == meta)
                        emit anim_boji_cvor(it,boja,QColor::fromRgb(217, 72, 43),float(i)/float(brzina));

                    emit okviri_cvor(it,rc,10,float(i)/float(brzina/2));

                }

                emit okviri_cvor(it,rc,0,0);
            }

            auto susedi = it->nadji_par_susede();

            for(auto s:susedi){
                if(obidjeni.find(s.first)==obidjeni.end() && koriscene.find(s.second)==koriscene.end()){
                    float dist = udaljenosti[it] + s.second->tezina;
                    if(dist < udaljenosti[s.first]){
                        udaljenosti[s.first] = dist;
                        roditelji[s.first] = std::make_pair(it,s.second);
                        koriscene.insert(s.second);

                        if(animacija)
                            for(int i=0;i<brzina;i++){
                                QThread::msleep(5);
                                emit animiraj_ivicu(s.second,it,s.first,QColor::fromRgb(31, 194, 219),float(i)/float(brzina-1));
                            }

                        if(labeliraj_cvorove && animacija){
                            emit stavi_tekst_cvor(s.first,s.first->naziv+"\n"+QString::number(dist));
                        }
                    } else {
                        for(int i=0;i<brzina;i++){
                            QThread::msleep(5);
                            emit transp_ivicu(s.second,1.0 - (float(i)/float(brzina))*0.7);
                        }
                    }
                }
            }

            if(animacija && it!=pocetak && it!=meta)
                emit boji_cvor(it,boja);

        }

        std::vector<std::pair<Cvor *,Ivica *>> put;
        float cena = 0;
        Cvor * cur = meta;
        while (cur!=nullptr) {
            put.push_back(std::make_pair(cur,roditelji[cur].second));
            cur = roditelji[cur].first;
        }
        std::reverse(put.begin(),put.end());

        for(auto c:put){
            if(animacija){

                QColor boja = c.first->pen().color();
                QRectF rc = c.first->rect();

                if(c.second!=nullptr)
                for(int i=0;i<brzina;i++){
                    QThread::msleep(5);
                    if(c.second->prvi == c.first)
                        emit animiraj_ivicu(c.second,c.second->drugi,c.second->prvi,QColor::fromRgb(18, 99, 9),float(i)/float(brzina-1));
                    else
                        emit animiraj_ivicu(c.second,c.second->prvi,c.second->drugi,QColor::fromRgb(18, 99, 9),float(i)/float(brzina-1));

                }

                for(int i=0;i<brzina;i++){
                    QThread::msleep(5);

                    if(c.first != pocetak && c.first!=meta)
                        emit anim_boji_cvor(c.first,boja,QColor::fromRgb(54, 156, 23),float(i)/float(brzina));
                   else if(c.first == pocetak)
                        emit anim_boji_cvor(c.first,boja,QColor::fromRgb(20, 86, 156),float(i)/float(brzina));
                   else if(c.first == meta)
                        emit anim_boji_cvor(c.first,boja,QColor::fromRgb(135, 41, 18),float(i)/float(brzina));

                    emit okviri_cvor(c.first,rc,10,float(i)/float(brzina/2));
                }

                emit okviri_cvor(c.first,rc,0,0);

            }
        }
    }

    emit kraj_animacije();
}

void Algoritam::primov()
{
    if(pocetak != nullptr){
        std::set<Ivica *> koriscene;
        std::set<Cvor *> komp;
        for(auto c:g->cvorovi){

            if(dostizan(pocetak,c,koriscene)){
                komp.insert(c);

                if(labeliraj_cvorove && animacija){
                    emit stavi_tekst_cvor(c,c->naziv+"\ninf");
                }

            }
        }

        std::set<Cvor *> obidjeni;
        std::map<Cvor *,float> udaljenosti;

        for(auto c:komp){
            udaljenosti.insert(std::make_pair(c,1000000));
        }
        udaljenosti[pocetak] = 0;

        while (true) {
            bool kraj = true;
            for(auto c:komp){
                if(obidjeni.find(c)==obidjeni.end())
                    kraj = false;
            }

            if(kraj){
                emit kraj_animacije();
                return;
            }

            Cvor* it =nullptr;
            float m = 10000000;
            for(auto& s:komp){
                if(udaljenosti[s] <= m && obidjeni.find(s) == obidjeni.end()){
                    it = s;
                    m = udaljenosti[s];
                }
            }

            if(it==nullptr){
                emit error_box("greska u trazenju min elementa!");
                emit kraj_animacije();
            }

            obidjeni.insert(it);

            QColor boja = it->pen().color();
            QRectF rc = it->rect();

            if(animacija){

                for(int i=0;i<brzina;i++){
                    QThread::msleep(5);

                    if(it != pocetak && it!=meta)
                        emit anim_boji_cvor(it,boja,QColor::fromRgb(161, 31, 148),float(i)/float(brzina));
                   else if(it == pocetak)
                        emit anim_boji_cvor(it,boja,QColor::fromRgb(16, 107, 235),float(i)/float(brzina));
                   else if(it == meta)
                        emit anim_boji_cvor(it,boja,QColor::fromRgb(217, 72, 43),float(i)/float(brzina));

                    emit okviri_cvor(it,rc,10,float(i)/float(brzina/2));

                }

                emit okviri_cvor(it,rc,0,0);
            }

            auto susedi = it->nadji_par_susede();

            for(auto s:susedi){
                if(obidjeni.find(s.first)==obidjeni.end() && koriscene.find(s.second)==koriscene.end()){
                    float dist = udaljenosti[it] + s.second->tezina;
                    if(dist < udaljenosti[s.first]){
                        udaljenosti[s.first] = dist;
                        koriscene.insert(s.second);

                        if(animacija)
                            for(int i=0;i<brzina;i++){
                                QThread::msleep(5);
                                emit animiraj_ivicu(s.second,it,s.first,QColor::fromRgb(31, 194, 219),float(i)/float(brzina-1));
                            }

                        if(labeliraj_cvorove && animacija){
                            emit stavi_tekst_cvor(s.first,s.first->naziv+"\n"+QString::number(dist));
                        }
                    } else {
                        for(int i=0;i<brzina;i++){
                            QThread::msleep(5);
                            emit transp_ivicu(s.second,1.0 - (float(i)/float(brzina))*0.7);
                        }
                    }
                }
            }

            if(animacija && it!=pocetak && it!=meta)
                emit boji_cvor(it,boja);

        }
    }

    emit kraj_animacije();
}

bool prolazi_sve(Cvor *pocetni,Graf *g){
    std::queue<Cvor *> red;
    red.push(pocetni);
    std::set<Cvor *> obidjeni;
    obidjeni.insert(pocetni);

    while (red.empty()==false) {
        auto vrh = red.front();
        red.pop();

        auto sus = vrh->nadji_par_susede();

        for(auto s:sus){
            if(obidjeni.find(s.first)==obidjeni.end()){
                red.push(s.first);
                obidjeni.insert(s.first);
            }
        }
    }
    for(auto v:g->cvorovi)
        if(obidjeni.find(v)==obidjeni.end())
            return false;

    return true;
}

Cvor * nadji_pocetni(Graf *g){
    std::set<Ivica *> koriscene;

    for(auto c:g->cvorovi)
        if(prolazi_sve(c,g))
            return c;
    return nullptr;
}

void popuni_indekse(Cvor *pocetni,std::map<Cvor*,int>& indeksi){
    std::queue<Cvor *> red;
    red.push(pocetni);
    std::set<Cvor *> obidjeni;
    obidjeni.insert(pocetni);
    indeksi[pocetni] = 0;

    while (red.empty()==false) {
        auto vrh = red.front();
        red.pop();

        auto sus = vrh->nadji_par_susede();

        for(auto s:sus){
            if(obidjeni.find(s.first)==obidjeni.end()){
                red.push(s.first);
                obidjeni.insert(s.first);
                indeksi[s.first] = indeksi[vrh] + 1;
            }
        }
    }
}

bool ima_cikl(std::set<Ivica *>& e,std::map<Cvor *,int>& grupe){
    std::map<Cvor *,int> dfs_index;
    //postoji cikl samo ako dfs stablo ima povratnu granu u vec obidjen index
    for(auto ev:e){
        dfs_index[ev->prvi] = -1;
        dfs_index[ev->drugi] = -1;
    }

    if(e.size()>0){
        Cvor *tekuci = (*e.begin())->prvi;

        dfs_index[tekuci] = 0;

        std::stack<Cvor *> stek;
        stek.push(tekuci);
        while (stek.empty()==false) {
            auto vrh = stek.top();
            stek.pop();

            auto sus = vrh->nadji_par_susede();
            for(auto s:sus){
                if(e.find(s.second)!=e.end()){
                    if(dfs_index[s.first]!=-1 && dfs_index[s.first]<dfs_index[vrh]
                            && grupe[s.first]!=grupe[vrh])
                        return true;
                    dfs_index[s.first] = dfs_index[vrh]+1;
                }
            }
        }
    }

    return false;
}

std::set<Cvor *> vrati_cikl(std::set<Ivica *>& e,std::map<Cvor *,int>& grupe){
    std::map<Cvor *,int> dfs_index;
    std::set<Cvor *> res;
    //postoji cikl samo ako dfs stablo ima povratnu granu u vec obidjen index
    for(auto ev:e){
        dfs_index[ev->prvi] = -1;
        dfs_index[ev->drugi] = -1;
    }

    if(e.size()>0){
        Cvor *tekuci = (*e.begin())->prvi;

        dfs_index[tekuci] = 0;

        std::stack<Cvor *> stek;
        stek.push(tekuci);
        while (stek.empty()==false) {
            auto vrh = stek.top();
            stek.pop();

            auto sus = vrh->nadji_par_susede();
            for(auto s:sus){
                if(e.find(s.second)!=e.end()){
                    if(dfs_index[s.first]!=-1 && dfs_index[s.first]<dfs_index[vrh]
                            && grupe[s.first]!=grupe[vrh]){
                        res.insert(vrh);
                        res.insert(s.first);
                    }
                    dfs_index[s.first] = dfs_index[vrh]+1;
                }
            }
        }
    }

    return res;
}

std::vector<Ivica *> vrati_ulazne(Cvor *c){
    std::vector<Ivica *> res;
    for(auto e:c->ivice)
        if(e->usmeren_ka==c)
            res.push_back(e);
    return res;
}

Cvor *pocetni_usmeren(Ivica *iv){
    if(iv->usmeren_ka == iv->prvi)
        return iv->drugi;
    else return iv->prvi;
}

QString c_set(std::set<Cvor*>& skup){
    QString res = "";
    for(auto v:skup)
        res.append(v->naziv+",");
    if(skup.size()>0)
        res.remove(res.size()-1,1);
    return res;
}

QString iv_set(std::set<Ivica*>& skup){
    QString res = "";
    for(auto v:skup){
        if(v->usmeren_ka == v->prvi)
            res.append("{"+v->drugi->naziv+"->"+v->prvi->naziv+"},");
        else
            res.append("{"+v->prvi->naziv+"->"+v->drugi->naziv+"},");
    }
    if(skup.size()>0)
        res.remove(res.size()-1,1);
    return res;
}

bool jeste_cikl(Cvor *koren,std::set<Ivica *>& grane){

    std::stack<Cvor *> stek;
    std::set<Cvor *> obidjeni;
    stek.push(koren);
    obidjeni.insert(koren);

    while (stek.empty()==false) {
        auto vrh = stek.top();
        stek.pop();

        auto sus = vrh->nadji_par_susede();

        for(auto s:sus){
            if(s.first == koren && grane.find(s.second)!=grane.end())
                return true;
            if(grane.find(s.second)!=grane.end() && obidjeni.find(s.first)==obidjeni.end()){
                obidjeni.insert(s.first);
                stek.push(s.first);
            }
        }
    }

    return false;
}

void nadji_cikl_pomocna(Cvor *k,std::set<Ivica *>& grane,std::vector<Cvor *>& r1,std::vector<Ivica *>& r2){
    std::map<Cvor *,std::pair<Cvor*,Ivica *>> roditelj;
    std::stack<Cvor *> stek;
    stek.push(k);
    std::set<Cvor *> obidjeni;
    obidjeni.insert(k);

    roditelj[k] = std::make_pair(nullptr,nullptr);

    while (stek.empty() == false) {
        auto vrh = stek.top();
        stek.pop();

        auto sus = vrh->nadji_par_susede();

        for(auto s:sus){
            if(s.first == k && grane.find(s.second)!=grane.end()){

                auto p = vrh;
                auto e = s.second;

                while (p!=nullptr) {
                    r1.push_back(p);
                    r2.push_back(e);
                    e = roditelj[p].second;
                    p = roditelj[p].first;
                }

                break;

            }

            if(grane.find(s.second)!=grane.end() && obidjeni.find(s.first)==obidjeni.end()){
                obidjeni.insert(s.first);
                stek.push(s.first);
                roditelj[s.first] = std::make_pair(vrh,s.second);
            }
        }
    }
}

std::pair<std::vector<Cvor*>,std::vector<Ivica *>> nadji_cikl(std::set<Cvor *>& cvorovi,std::set<Ivica *>& grane){
    std::vector<Cvor*> r1;
    std::vector<Ivica*> r2;

    for(auto v:cvorovi){
        if(jeste_cikl(v,grane)){
            nadji_cikl_pomocna(v,grane,r1,r2);
            return std::make_pair(r1,r2);
        }
    }

    return std::make_pair(r1,r2);
}

void bfs_indeksuj(Cvor *poc,std::map<Cvor*,int>& indeksi){
    std::queue<Cvor*> red;
    std::set<Cvor *> obidjeni;

    red.push(poc);
    obidjeni.insert(poc);
    indeksi[poc] = 0;

    while(red.empty() == false){
        auto vrh = red.front();
        red.pop();

        auto sus = vrh->nadji_par_susede();
        for(auto s:sus){
            if(obidjeni.find(s.first)==obidjeni.end() && s.first->opacity()>0.5){
                obidjeni.insert(s.first);
                indeksi[s.first] = indeksi[vrh]+1;
                red.push(s.first);
            }
        }
    }
}

struct Cikl_edmonds{
    std::set<Cvor*> cv;
    std::set<Ivica*> iv;
    std::set<Ivica*> ulazne;
    Cvor *cikl;
};

void Algoritam::edmonds()
{
    if(g->cvorovi.size()>0){
        Cvor *pocetni = nadji_pocetni(g);
        if(pocetni == nullptr){
            emit error_box("ne postoji cvor iz kog je moguce krenuti!");
            emit kraj_animacije();
            return;
        }
        std::map<Cvor*,int> indeksi;
        bfs_indeksuj(pocetni,indeksi);

        //stavljamo svaki indeks u svoju grupu , koristimo grupe da obelezimo supercvorove
        std::map<Cvor *,int> grupe;
        int gr = 1;
        for(auto v:g->cvorovi)
            grupe.insert(std::make_pair(v,gr++));

        //nalazimo najveci indeks
        int m = indeksi[pocetni];
        Cvor* curr = pocetni;
        for(auto c:indeksi)
            if(m<c.second){
                m = c.second;
                curr = c.first;
            }

        std::set<Ivica *> e;
        std::set<Cvor *> v;
        std::set<Cvor *> w;
        std::set<Ivica *> f;
        w.insert(pocetni);

        emit dodaj_text("Korak 1:");
        QString vstr = c_set(v);
        QString wstr = c_set(w);
        QString fstr = iv_set(f);
        QString estr = iv_set(e);

        emit dodaj_text("W = {"+wstr+"}");
        emit dodaj_text("F = {"+fstr+"}");
        emit dodaj_text("V = {"+vstr+"}");
        emit dodaj_text("E = {"+estr+"}");

        int cnum = 0;

        std::stack<Cikl_edmonds> ciklovi;
        std::map<Ivica*,Ivica*> precrtavanje;

        while(true){
            QThread::msleep(5);
            emit dodaj_text("Korak 2:");


            QString vstr = c_set(v);
            QString wstr = c_set(w);
            QString fstr = iv_set(f);
            QString estr = iv_set(e);

            emit dodaj_text("W = {"+wstr+"}");
            emit dodaj_text("F = {"+fstr+"}");
            emit dodaj_text("V = {"+vstr+"}");
            emit dodaj_text("E = {"+estr+"}");


            bool isti = true;
            for(auto vt:v)
                if(w.find(vt)==w.end())
                    isti = false;
            for(auto vt:w)
                if(v.find(vt)==v.end())
                    isti = false;

            if(isti){
                emit dodaj_text("V i W su isti");
                emit dodaj_text("Korak 8:");

                std::set<Ivica *> ukl;

                for(auto vt:g->cvorovi)
                    emit boji_cvor(vt,QColor::fromRgb(0,0,0));

                for(auto vt:g->ivice)
                    emit boji_ivica(vt,QColor::fromRgb(0,0,0));


                while(ciklovi.empty() == false){
                    auto vrh = ciklovi.top();
                    ciklovi.pop();

                    emit dodaj_text("rasparcavamo supercvor "+vrh.cikl->naziv);

                    std::set<Ivica*> nf;
                    //racunamo koje ivice mogu da se otkriju
                    for(auto iv:vrh.iv)
                        emit transp_ivicu(iv,1);

                    for(auto sus:vrh.cikl->nadji_ed_susede())
                        emit transp_ivicu(precrtavanje[sus.second],1);


                    //ako postoji ivica koja ulazi u isti cvor kao ona a da je u f nmzemo nju da koristimo
                        for(auto sus:vrh.cikl->nadji_ed_susede()){
                            if(sus.second->usmeren_ka==vrh.cikl && f.find(sus.second)!=f.end()){
                                for(auto iv:vrh.iv)
                                    if(iv->usmeren_ka == precrtavanje[sus.second]->usmeren_ka){
                                        emit dodaj_text("grana ("+iv->prvi->naziv+"->"+iv->drugi->naziv+") ima suprotnu pa se ne dodaje");
                                        nf.insert(iv);
                                    }
                            }
                        }
                    for(auto iv:vrh.iv)
                            emit transp_ivicu(iv,0);

                    for(auto sus:vrh.cikl->nadji_ed_susede())
                            emit transp_ivicu(precrtavanje[sus.second],0);




                    if(animacija){
                        QRectF rc = vrh.cikl->rect();

                        for(int i=0;i<brzina;i++){
                             QThread::msleep(5);
                             emit anim_boji_cvor(vrh.cikl,QColor::fromRgb(0,0,0), QColor::fromRgb(200,0,0),float(i)/float(brzina-1));
                             emit okviri_cvor(vrh.cikl,rc,10,float(i)/float((brzina-1)/2));
                        }
                        emit okviri_cvor(vrh.cikl,rc,0,0);

                        //otkrivamo sve ostale cvorove

                        for(int i=0;i<brzina;i++){
                             QThread::msleep(5);

                             emit transp_cvor_sve(vrh.cikl,1-float(i)/float((brzina-1)));
                             for(auto vt:vrh.cikl->nadji_ed_susede())
                                emit transp_ivicu(vt.second,1-float(i)/float((brzina-1)));

                        }

                        for(int i=0;i<brzina;i++){
                             QThread::msleep(5);
                             for(auto vt:vrh.cv)
                                emit transp_cvor_sve(vt,float(i)/float((brzina-1)));
                             for(auto vt:vrh.iv)
                                emit transp_ivicu(vt,float(i)/float((brzina-1)));
                             for(auto vt:vrh.cikl->nadji_ed_susede())
                                emit transp_ivicu(precrtavanje[vt.second],float(i)/float((brzina-1)));
                        }

                        for(int i=0;i<brzina;i++){
                             QThread::msleep(5);
                             for(auto vt:nf)
                                emit transp_ivicu(vt,1-float(i)/float((brzina-1)));
                        }

                    }

                    for(auto vt:vrh.cikl->nadji_ed_susede())
                       if(f.find(vt.second)!=f.end()){
                           f.erase(vt.second);
                           f.insert(precrtavanje[vt.second]);
                       }

                    for(auto vt:vrh.iv)
                        if(nf.find(vt)==nf.end())
                              f.insert(vt);

                    w.erase(vrh.cikl);

                    for(auto vrr:vrh.cv)
                        w.insert(vrr);



                }

                break;
            } else {
                emit dodaj_text("Korak 3:");
                emit dodaj_text("radimo bfs pretragu za x cvor");
                std::map<Cvor*,int> indeksi;
                bfs_indeksuj(pocetni,indeksi);

                //stavljamo svaki indeks u svoju grupu , koristimo grupe da obelezimo supercvorove
                std::map<Cvor *,int> grupe;
                int gr = 1;
                for(auto v:g->cvorovi)
                    grupe.insert(std::make_pair(v,gr++));

                //nalazimo najveci indeks
                int m = indeksi[pocetni];
                Cvor* curr = pocetni;
                for(auto c:indeksi)
                    if(m<c.second && w.find(c.first)==w.end()){
                        m = c.second;
                        curr = c.first;
                    }
                if(curr==pocetni) {
                    curr = nullptr;
                    emit dodaj_text("Ne postoji BFS slobodan cvor pa pokusavamo da rasparcamo ako ima supercvorova");
                    emit dodaj_text("Korak 8:");

                    std::set<Ivica *> ukl;


                    while(ciklovi.empty() == false){
                        auto vrh = ciklovi.top();
                        ciklovi.pop();

                        emit dodaj_text("rasparcavamo supercvor "+vrh.cikl->naziv);

                        std::set<Ivica*> nf;
                        //racunamo koje ivice mogu da se otkriju
                        for(auto iv:vrh.iv)
                            emit transp_ivicu(iv,1);

                        for(auto sus:vrh.cikl->nadji_ed_susede())
                            emit transp_ivicu(precrtavanje[sus.second],1);


                        //ako postoji ivica koja ulazi u isti cvor kao ona a da je u f nmzemo nju da koristimo
                            for(auto sus:vrh.cikl->nadji_ed_susede()){
                                if(sus.second->usmeren_ka==vrh.cikl && f.find(sus.second)!=f.end()){
                                    for(auto iv:vrh.iv)
                                        if(iv->usmeren_ka == precrtavanje[sus.second]->usmeren_ka){
                                            emit dodaj_text("grana ("+iv->prvi->naziv+"->"+iv->drugi->naziv+") ima suprotnu pa se ne dodaje");
                                            nf.insert(iv);
                                        }
                                }
                            }
                        for(auto iv:vrh.iv)
                                emit transp_ivicu(iv,0);

                        for(auto sus:vrh.cikl->nadji_ed_susede())
                                emit transp_ivicu(precrtavanje[sus.second],0);




                        if(animacija){
                            QRectF rc = vrh.cikl->rect();

                            for(int i=0;i<brzina;i++){
                                 QThread::msleep(5);
                                 emit anim_boji_cvor(vrh.cikl,QColor::fromRgb(0,0,0), QColor::fromRgb(200,0,0),float(i)/float(brzina-1));
                                 emit okviri_cvor(vrh.cikl,rc,10,float(i)/float((brzina-1)/2));
                            }
                            emit okviri_cvor(vrh.cikl,rc,0,0);

                            //otkrivamo sve ostale cvorove

                            for(int i=0;i<brzina;i++){
                                 QThread::msleep(5);

                                 emit transp_cvor_sve(vrh.cikl,1-float(i)/float((brzina-1)));
                                 for(auto vt:vrh.cikl->nadji_ed_susede())
                                    emit transp_ivicu(vt.second,1-float(i)/float((brzina-1)));

                            }

                            for(int i=0;i<brzina;i++){
                                 QThread::msleep(5);
                                 for(auto vt:vrh.cv)
                                    emit transp_cvor_sve(vt,float(i)/float((brzina-1)));
                                 for(auto vt:vrh.iv)
                                    emit transp_ivicu(vt,float(i)/float((brzina-1)));
                                 for(auto vt:vrh.cikl->nadji_ed_susede())
                                    emit transp_ivicu(precrtavanje[vt.second],float(i)/float((brzina-1)));
                            }

                            for(int i=0;i<brzina;i++){
                                 QThread::msleep(5);
                                 for(auto vt:nf)
                                    emit transp_ivicu(vt,1-float(i)/float((brzina-1)));
                            }

                        }

                        for(auto vt:vrh.cikl->nadji_ed_susede())
                           if(f.find(vt.second)!=f.end()){
                               f.erase(vt.second);
                               f.insert(precrtavanje[vt.second]);
                           }

                        for(auto vt:vrh.iv)
                            if(nf.find(vt)==nf.end())
                                  f.insert(vt);

                        w.erase(vrh.cikl);

                        for(auto vrr:vrh.cv)
                            w.insert(vrr);



                    }

                    break;
                }
                if(animacija){
                    std::vector<Cvor *> niz;
                    for(auto a:indeksi)
                        niz.push_back(a.first);

                    std::sort(niz.begin(),niz.end(),[&](auto x,auto y){ return indeksi[x]<=indeksi[y];});

                    for(auto n:niz){
                        for(int i=0;i<brzina;i++){
                             QThread::msleep(5);
                             if(w.find(n)!=w.end())
                                 emit anim_boji_cvor(n,QColor::fromRgb(0,0,0), QColor::fromRgb(200,200,0),float(i)/float(brzina-1));
                             else
                                emit anim_boji_cvor(n,QColor::fromRgb(0,0,0), QColor::fromRgb(255,0,0),float(i)/float(brzina-1));
                        }
                        emit stavi_tekst_cvor(n,QString::number( indeksi[n]));
                    }

                    for(int i=0;i<brzina;i++)
                         QThread::msleep(5);

                    for(int i=0;i<brzina;i++){
                         QThread::msleep(5);
                        for(auto n:niz){
                            if(w.find(n)!=w.end())
                                emit anim_boji_cvor(n,QColor::fromRgb(200,200,0), QColor::fromRgb(0,0,0),float(i)/float(brzina-1));
                            else
                                emit anim_boji_cvor(n,QColor::fromRgb(255,0,0), QColor::fromRgb(0,0,0),float(i)/float(brzina-1));
                            emit stavi_tekst_cvor(n,n->naziv);
                        }
                    }
                }

                if(curr!=nullptr)
                    emit dodaj_text("Cvor koji uzimamo x je "+curr->naziv);
                while (true && curr!=nullptr) {
                    QThread::msleep(5);
                    emit dodaj_text("Korak 4:");
                    v.insert(curr);
                    auto ulazne = vrati_ulazne(curr);
                    float mn = 100000;
                    Ivica *mc = ulazne[0];
                    for(auto c:ulazne)
                        if(mn>c->tekst.text().toFloat() && c->opacity()>=0.5){
                            mn = c->tekst.text().toFloat();
                            mc = c;
                        }
                    e.insert(mc);
                    Cvor *poc = pocetni_usmeren(mc);

                    if(animacija){
                           QColor col = curr->pen().color();
                          for(int i=0;i<brzina;i++){
                               QThread::msleep(5);
                               emit anim_boji_cvor(curr,col,QColor::fromRgb(219, 142, 33),float(i)/float(brzina-1));
                           }
                    }

                    QString vstr = c_set(v);
                    QString estr = iv_set(e);

                    emit dodaj_text("V = {"+vstr+"}");
                    emit dodaj_text("E = {"+estr+"}");

                    std::set<Cvor *> vw;
                    for(auto vt:v)
                        vw.insert(vt);
                    for(auto vt:w)
                        vw.insert(vt);

                    if(animacija){
                          for(int i=0;i<brzina;i++){
                               QThread::msleep(5);
                               emit animiraj_ivicu(mc,curr,poc,QColor::fromRgb(219, 142, 33),float(i)/float(brzina-1));
                           }
                    }


                    if(vw.find(poc)==vw.end()){
                        emit dodaj_text("Korak 5:");

                        emit dodaj_text(poc->naziv+" nije u V U W on je tekuci i idemo na Korak 4");
                        curr = poc;

                        continue;
                    }

                    if(v.find(poc)!=v.end()){
                        emit dodaj_text("Korak 6:");
                        emit dodaj_text("Dodavanjem "+poc->naziv+" dobijamo cikl pravimo supercvor");
                        auto cikl = nadji_cikl(v,e);

                        if(animacija){
                              for(auto e:cikl.second){
                                  QColor col1 = e->prvi->pen().color();
                                  QColor col2 = e->drugi->pen().color();
                                  for(int i=0;i<brzina;i++){
                                      QThread::msleep(5);
                                      emit anim_boji_cvor(e->prvi,col1,QColor::fromRgb(230,25,25),float(i)/float(brzina-1));
                                  }


                              for(int i=0;i<brzina;i++){
                                  QThread::msleep(5);
                                  emit animiraj_ivicu(e,e->prvi,e->drugi,QColor::fromRgb(230,25,25),float(i)/float(brzina-1));
                              }

                              for(int i=0;i<brzina;i++){
                                  QThread::msleep(5);
                                  emit anim_boji_cvor(e->drugi,col2,QColor::fromRgb(230,25,25),float(i)/float(brzina-1));

                              }

                              }


                        }

                        Cvor *cikl_cvor=new Cvor();
                        emit dodaj_anim_cvor(cikl_cvor);


                        cikl_cvor->naziv ="c"+QString::number(cnum);


                        emit stavi_tekst_cvor(cikl_cvor,"c"+QString::number(cnum));
                        cnum+=1;


                        Cvor *poz = cikl.first[0];
                        int num = 0;
                        for(auto cc:cikl.first){
                            auto susedi = cc->nadji_ed_susede();
                            int n= 0;
                            for(auto sus:susedi)
                                if(std::find(cikl.first.begin(),cikl.first.end(),sus.first)==cikl.first.end()
                                        && sus.second->opacity()>0.5)
                                    n++;
                            if(n>num){
                                num=n;
                                poz = cc;
                            }

                        }

                        emit pomeri_cvor(cikl_cvor,poz->pos());

                        emit transp_cvor_sve(cikl_cvor,0);

                        std::set<Ivica *> cyc;
                        std::set<Cvor *> cycv;

                        for(auto c:cikl.first){
                            v.erase(c);
                            cycv.insert(c);
                        }

                        for(auto et:cikl.second){
                            cyc.insert(et);
                            e.erase(et);
                        }

                        QString cy = iv_set(cyc);

                        emit dodaj_text("Nasli smo cikl: "+cy);

                        std::set<Ivica*> br;
                        std::set<Ivica*> dod;
                        Cikl_edmonds ckl;
                        std::set<Ivica *> dodate;

                        for(auto vert:cycv){
                            auto susedi = vert->nadji_ed_susede();

                            for(auto sus:susedi)
                                if(sus.second->opacity()>0.8 && cycv.find(sus.first)==cycv.end()){
                                    //ako sused nije deo cikla i nije sakriven
                                    Ivica *nv = new Ivica(sus.first,cikl_cvor);
                                    nv->usmeren = Usmeren;
                                    nv->koristi_tezine = true;
                                    precrtavanje[nv] = sus.second;
                                    br.insert(sus.second);

                                    if(cycv.find(sus.second->usmeren_ka)==cycv.end()){
                                        //ako nije usmeren ka ciklu
                                        nv->usmeren_ka = sus.first;
                                        nv->tezina = sus.second->tezina;
                                    } else {
                                        //ako je usmerena ka ciklu moramo da je azuriramo
                                        nv->usmeren_ka = cikl_cvor;
                                        ckl.ulazne.insert(nv);

                                        float m= -100000;
                                        auto ulazne = vrati_ulazne(sus.second->usmeren_ka);

                                        for(auto ul:ulazne)
                                            if(cycv.find(ul->prvi)!=cycv.end() && cycv.find(ul->prvi)!=cycv.end()){
                                                if(ul->tekst.text().toFloat()>m)
                                                    m=ul->tekst.text().toFloat();
                                            }
                                        if(m!=-100000)
                                            nv->tezina = sus.second->tekst.text().toFloat() - m;
                                        else
                                            nv->tezina = -1;


                                        if(animacija && m!= -100000){
                                            for(int i=0;i<brzina;i++){
                                                QThread::msleep(5);
                                            }
                                            emit stavi_tekst_ivica(sus.second,sus.second->tekst.text()+"-"+QString::number(m));
                                            for(int i=0;i<brzina;i++){
                                                QThread::msleep(5);
                                            }
                                            emit stavi_tekst_ivica(sus.second,
                                                                   QString::number(sus.second->tekst.text().toFloat()-m));
                                        }

                                    }


                                    dodate.insert(nv);
                                    if(e.find(sus.second)!=e.end())
                                        dod.insert(nv);


                                    emit dodaj_anim_ivicu(nv);
                                    emit stavi_tekst_ivica(nv,QString::number(nv->tezina));
                                    emit transp_ivicu(nv,0);
                                }

                        }




                            for(auto ed:br)
                                e.erase(ed);




                           for(int i=0;i<brzina;i++){
                                     QThread::msleep(5);

                                for(auto ed:br)
                                    emit transp_ivicu(ed,1-float(i)/float(brzina-1));

                                for(auto e:cikl.second){
                                    emit transp_cvor_sve(e->prvi,0);
                                    emit transp_cvor_sve(e->drugi,0);
                                    emit transp_ivicu(e,0);
                                }

                                for(auto ed:dodate)
                                   emit transp_ivicu(ed,float(i)/float(brzina-1));

                                emit transp_cvor_sve(cikl_cvor,float(i)/float(brzina-1));

                            }



                        for(auto ed:dod)
                            e.insert(ed);


                        ckl.cikl = cikl_cvor;

                        for(auto ct:cikl.first)
                            ckl.cv.insert(ct);

                        for(auto ct:cikl.second)
                            ckl.iv.insert(ct);

                        ciklovi.push(ckl);

                        emit dodaj_text("Idemo na Korak 4");
                        curr = cikl_cvor;
                        emit transp_cvor_sve(cikl_cvor,1);

                        continue;
                    }
                    emit dodaj_text("Korak 7:");
                    if(w.find(poc)!=w.end()){
                        emit dodaj_text(poc->naziv+" je vec u W");
                        for(auto vt:v)
                            w.insert(vt);
                        for(auto vt:e)
                            f.insert(vt);
                        f.insert(mc);

                        QString wstr = c_set(w);
                        QString fstr = iv_set(f);

                        emit dodaj_text("W = {"+wstr+"}");
                        emit dodaj_text("f = {"+fstr+"}");

                        v.clear();
                        e.clear();

                        emit dodaj_text("Idemo na Korak 2");
                        break;
                    }

                }

            }


        }

        for(auto vt:g->cvorovi){
            if(vt->opacity()>0.5);
            emit boji_cvor(vt,QColor::fromRgb(0,110,0));
        }
        float suma = 0;
        for(auto vt:g->ivice){
            emit stavi_tekst_ivica(vt,QString::number(vt->tezina));
            if(std::find(g->anim_ivice.begin(),g->anim_ivice.end(),vt)==g->anim_ivice.end()){
                if(f.find(vt)!=f.end()){
                    vt->anim_line.hide();
                    emit boji_ivica(vt,QColor::fromRgb(0,0,110));
                    suma += vt->tezina;
                }
                else{
                    emit boji_ivica(vt,QColor::fromRgb(0,0,0));
                    emit transp_ivicu(vt,0.1);
                }
            } else {
                emit transp_ivicu(vt,0);
            }
        }
        QString wwstr = c_set(w);
        QString ffstr = iv_set(f);

        emit dodaj_text("W = {"+wwstr+"}");
        emit dodaj_text("F = {"+ffstr+"}");

        emit dodaj_text("Minimalno razapinjuce stablo je tezine "+QString::number(suma));

    }




    emit kraj_animacije();
}

void Algoritam::ford_fulker()
{

}

QString ed_map_str(std::map<Cvor*,std::tuple<Cvor*,char,float>>& s){
    QString str = "{";
    int i=0;
    for(auto it:s){

            QString rs = "(";
            if(std::get<0>(it.second)!=nullptr)
                rs += std::get<0>(it.second)->naziv;
            rs += std::get<1>(it.second);
            if(std::get<2>(it.second)==std::numeric_limits<float>::infinity())
                rs += ",+inf";
            else rs+=","+QString::number(std::get<2>(it.second));
            rs+=")";

            str.append(it.first->naziv);
            str.append(rs);
            str.append(",");

        i++;
    }

    if(str[str.size()-1]==',')
        str.resize(str.size()-1);
    str+="}";
    return str;
}

QString ed_red_str(std::vector<Cvor *>& l){
    QString res = "{";

    for(auto it:l)
        res+= it->naziv+",";

    if(res[res.size()-1]==',')
        res.resize(res.size()-1);
    res+="}";
    return res;
}

void Algoritam::edmond_carp()
{
    if(g->cvorovi.size()>0){
        emit dodaj_text("---------------------");
        emit dodaj_text("Edmonds Carp Algoritam:");

        while(true){
        emit dodaj_text("Korak 1:");

        std::map<Cvor*,std::tuple<Cvor*,char,float>> s;
        std::set<Cvor*> skups;

        std::vector<Cvor *> l;
        s[izvor] = std::make_tuple(nullptr,'-',std::numeric_limits<float>::infinity());
        l.push_back(izvor);
        skups.insert(izvor);

        bool tek = true;


        while(tek){
            emit dodaj_text("");
            emit dodaj_text("trenutno stanje:");
            emit dodaj_text("S="+ed_map_str(s)+"  L="+ed_red_str(l));

            if(l.empty()==true){
                emit dodaj_text("L je prazan -> STOP");


                for(auto v:g->cvorovi)
                emit boji_cvor(v,QColor::fromRgb(0,0,0));
                for(auto e:g->ivice){
                    e->anim_line.hide();
                    emit boji_ivica(e,QColor::fromRgb(0,0,0));
                }

                std::set<Cvor *> prvi;
                std::set<Cvor *> drugi;

                for(auto it:s)
                    prvi.insert(it.first);
                for(auto it:g->cvorovi)
                    if(prvi.find(it)==prvi.end())
                        drugi.insert(it);

                float sum = 0;
                auto susedi = izvor->nadji_par_susede();
                for(auto sus:susedi)
                    sum += sus.second->dotok;

                emit dodaj_text("max protok F = "+QString::number(sum));
                emit dodaj_text("S = "+c_set(prvi));
                emit dodaj_text("T = "+c_set(drugi));

                for(auto t:prvi)
                    emit boji_cvor(t,QColor::fromRgb(0,255,0));

                for(auto t:drugi)
                    emit boji_cvor(t,QColor::fromRgb(0,0,255));


                emit kraj_animacije();
                return;
            }


            Cvor *u = l.front();
            emit dodaj_text("uzimamo u = "+u->naziv);

            if(u!=ponor)
            if(animacija){
                QRectF rc = u->rect();
                for(int i=0;i<brzina;i++){
                    QThread::msleep(5);
                    emit anim_boji_cvor(u,QColor::fromRgb(0,0,0),QColor::fromRgb(255,0,0),float(i)/float(brzina-1));
                    emit okviri_cvor(u,rc,10,float(i)/float((brzina)/2));
                }
                emit okviri_cvor(u,rc,0,0);
            }

            if(u==ponor){
                emit dodaj_text("u = t pa radimo Korak 7 rekonstrukciju");

                float val = std::get<2>(s[ponor]);

                Cvor *curr = ponor;
                while(curr != izvor){

                    if(animacija){
                        QRectF rt = curr->rect();
                        for(int i=0;i<brzina;i++){
                            QThread::msleep(5);
                            emit anim_boji_cvor(curr,QColor::fromRgb(10, 92, 122),QColor::fromRgb(255,0,0),float(i)/float(brzina-1));
                            emit okviri_cvor(curr,rt,20,float(i)/float((brzina)/2));
                        }
                        emit okviri_cvor(curr,rt,0,0);

                    }

                    auto susedi = curr->nadji_ed_susede();
                    auto tup = s[curr];
                    Cvor *meta = std::get<0>(tup);
                    for(auto s:susedi){
                        if(s.first == meta){
                            if(std::get<1>(tup)=='+')
                                s.second->dotok += val;
                            else
                                s.second->dotok -= val;

                            if(animacija){
                                for(int i=0;i<brzina;i++){
                                    QThread::msleep(5);
                                    emit animiraj_ivicu(s.second,curr,meta,QColor::fromRgb(255,0,0),float(i)/float(brzina-1));
                                }
                                emit stavi_tekst_ivica(s.second,QString::number(s.second->dotok)+"/"+
                                                       QString::number(s.second->protok));
                            }
                        }
                    }
                    curr = meta;

                }

                if(animacija){
                    QRectF rs = izvor->rect();
                    for(int i=0;i<brzina;i++){
                        QThread::msleep(5);
                        emit anim_boji_cvor(izvor,QColor::fromRgb(10, 92, 122),QColor::fromRgb(255,0,0),float(i)/float(brzina-1));
                        emit okviri_cvor(izvor,rs,20,float(i)/float((brzina)/2));
                    }
                    emit okviri_cvor(izvor,rs,0,0);

                    for(int i=0;i<brzina;i++)
                        QThread::msleep(5);

                    for(auto v:g->cvorovi)
                    emit boji_cvor(v,QColor::fromRgb(0,0,0));
                    for(auto e:g->ivice){
                        e->anim_line.hide();
                        emit boji_ivica(e,QColor::fromRgb(0,0,0));
                    }

                    for(int i=0;i<brzina;i++)
                        QThread::msleep(5);



                }




                tek = false;
                break;
            } else {
                while(true){
                    auto susedi = u->nadji_ed_susede();
                    bool uslov = true;
                    for(auto sus:susedi){
                        if(skups.find(sus.first)==skups.end()){
                            if(sus.second->usmeren_ka!=u &&  sus.second->dotok<sus.second->protok)
                                uslov = false;
                            else if(sus.second->usmeren_ka==u &&  sus.second->dotok>0)
                                uslov = false;
                        }

                    }

                    if(uslov){
                        emit dodaj_text("svaki sused od u tj od "+u->naziv+" je nevalidan za izbor");
                        emit dodaj_text("uklanjamo "+u->naziv+" iz L i idemo na Korak 2");
                        l.erase(std::find(l.begin(),l.end(),u));
                        if(animacija)
                        for(int i=0;i<brzina;i++){
                            QThread::msleep(5);
                            emit anim_boji_cvor(u,QColor::fromRgb(255,0,0),QColor::fromRgb(10, 92, 122),float(i)/float(brzina-1));
                        }
                        break;
                    }


                    for(auto sus:susedi){
                        if(skups.find(sus.first)==skups.end()){

                            if(animacija){
                                QRectF rc2 = sus.first->rect();
                                for(int i=0;i<brzina;i++){
                                    QThread::msleep(5);
                                    emit anim_boji_cvor(sus.first,QColor::fromRgb(0,0,0),QColor::fromRgb(148, 14, 153),float(i)/float(brzina-1));
                                    emit okviri_cvor(sus.first,rc2,20,float(i)/float((brzina)/2));
                                }
                                emit okviri_cvor(sus.first,rc2,0,0);
                            }

                            if(sus.second->usmeren_ka != u && sus.second->dotok<sus.second->protok){
                                float pu = std::get<2>(s[u]);
                                float p = sus.second->protok - sus.second->dotok;
                                float pv = std::min(pu,p);
                                emit dodaj_text("Uzeli smo suseda "+sus.first->naziv);
                                emit dodaj_text("nova vrednost je ("+u->naziv+"+,"+QString::number(pv)+")");
                                s[sus.first] = std::make_tuple(u,'+',pv);
                                l.push_back(sus.first);
                                skups.insert(sus.first);


                                if(animacija){
                                    auto cl = sus.second->pen().color();
                                    for(int i=0;i<brzina;i++){
                                        QThread::msleep(5);
                                        emit animiraj_ivicu(sus.second,u,sus.first,QColor::fromRgb(122, 10, 100),
                                                            float(i)/float(brzina-1));
                                    }

                                    emit stavi_tekst_ivica(sus.second,QString::number(sus.second->dotok)+"/"+
                                                           QString::number(sus.second->protok));

                                    for(int i=0;i<brzina;i++){
                                        QThread::msleep(5);
                                        emit anim_boji_cvor(sus.first,QColor::fromRgb(148, 14, 153),QColor::fromRgb(10, 92, 122),float(i)/float(brzina-1));
                                    }

                                    emit boji_ivica(sus.second,QColor::fromRgb(0,0,0));
                                }

                            } else if(sus.second->usmeren_ka == u && sus.second->dotok>0){
                                float pu = std::get<2>(s[u]);
                                float p = sus.second->dotok;
                                float pv = std::min(pu,p);
                                emit dodaj_text("Uzeli smo suseda "+sus.first->naziv);
                                emit dodaj_text("nova vrednost je ("+u->naziv+"-,"+QString::number(pv)+")");
                                s[sus.first] = std::make_tuple(u,'-',pv);
                                l.push_back(sus.first);
                                skups.insert(sus.first);


                                if(animacija){
                                    auto cl = sus.second->pen().color();
                                    for(int i=0;i<brzina;i++){
                                        QThread::msleep(5);
                                        emit animiraj_ivicu(sus.second,u,sus.first,QColor::fromRgb(207, 132, 33),
                                                            float(i)/float(brzina-1));
                                    }

                                    emit stavi_tekst_ivica(sus.second,QString::number(sus.second->dotok)+"/"+
                                                           QString::number(sus.second->protok));

                                    for(int i=0;i<brzina;i++){
                                        QThread::msleep(5);
                                        emit anim_boji_cvor(sus.first,QColor::fromRgb(148, 14, 153),QColor::fromRgb(10, 92, 122),float(i)/float(brzina-1));
                                    }

                                }
                            }
                        }
                    }

                }

            }



        }

    }
    }
    emit kraj_animacije();
}

void Algoritam::run(){
    switch (method) {
    case 0:bfs();
        break;
    case 1:dfs();
        break;
    case 2:fleury3();
        break;
    case 3:kruskal();
        break;
    case 4:dijsktra();
        break;
    case 5:primov();
        break;
    case 6:edmonds();
        break;
    case 7:ford_fulker();
        break;
    case 8:edmond_carp();
        break;
    }
}

Dugmic::Dugmic(tip_dugmeta tip)
{
    if(tip == tip_dugmeta::Tegic){
        setPixmap(QPixmap::fromImage(tegic_slika));
    } else if(tip == tip_dugmeta::Protocni){
        setPixmap(QPixmap::fromImage(protok_slika));
    }
}

int strana_prave(Ivica *iv,QPointF p){
    float x1 = iv->path().elementAt(0).x;
    float x2 = iv->path().elementAt(1).x;

    float y1 = iv->path().elementAt(0).y;
    float y2 = iv->path().elementAt(1).y;

    QPointF AB = QPointF(x2-x1,y2-y1);

    QPointF AP = QPointF(p.x()-x1,p.y()-y1);

    float result = (AB.x() * AP.x() - AB.y()*AP.y());
    if(result > 0)
        return 1;
    if(result < 0)
        return -1;
    return 0;
}

void Dugmic::azuriraj_poziciju(Ivica *iv)
{
        show();
        float x1 = iv->path().elementAt(0).x;
        float x2 = iv->path().elementAt(1).x;

        float y1 = iv->path().elementAt(0).y;
        float y2 = iv->path().elementAt(1).y;

        QPointF vec = QPointF(x2-x1,y2-y1);
        vec = QPointF(-vec.y(),vec.x());
        float s = sqrtf(QPointF::dotProduct(vec,vec));
        vec = QPointF(vec.x()/s,vec.y()/s);

        float w = 40;
        QPointF tp = iv->tekst.pos();
        QPointF pos = QPointF(tp.x() - w*vec.x() , tp.y() - w*vec.y());

        if(strana_prave(iv,pos) == strana_prave(iv,tp)){
            pos = QPointF(tp.x() + 2*w*vec.x() , tp.y() + 2*w*vec.y());
            if(strana_prave(iv,pos) == strana_prave(iv,tp))
                pos = QPointF(tp.x() - 2*w*vec.x() , tp.y() - 2*w*vec.y());
        }


        setPos(pos);
}
