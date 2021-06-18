#ifndef GRAF_HPP
#define GRAF_HPP

//Qt inkludovi
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QObject>

//inkludovi za std
#include <vector>

#include <QThread>

using namespace std;

enum stanje_scene {Biranje , Dodaj_cvor , Dodaj_ivicu};

class Cvor;
class Graf;
//Razlikujemo 3 tipa ivica protocne su za ford fulkersona kasnije kad imam protok i dotok
enum Tip_Ivice { Obicna , Protocna };
enum usmerenost {Neusmeren , Usmeren , Sam_u_sebe};

class Ivica : public QGraphicsPathItem {
public:
    Ivica(Cvor* prvi,Cvor* drugi);

    QGraphicsSimpleTextItem tekst;
    QGraphicsPolygonItem strelica;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e);

    float tezina = 0;
    float a_tezina = 0;
    float protok = 0;
    float dotok = 0;

    Tip_Ivice tip = Obicna;
    usmerenost usmeren = Neusmeren;

    Cvor *usmeren_ka = nullptr;

    int velicina_teksta = 15;
    float debljina_linije = 5;
    bool ignorisi = false;
    bool koristi_tezine = false;

    QString o_tekst;

    Cvor *prvi;
    Cvor *drugi;

    Graf *roditelj;

    QGraphicsPathItem anim_line;

    void azuriraj_poziciju();

    void postavi_tekst(QString txt);

    void postavi_koristi_tezine(bool t);

};

class Cvor : public QGraphicsEllipseItem {
public:
    Cvor();

    void pomeri(QPointF p);

    QGraphicsSimpleTextItem tekst;

    std::vector<Ivica*> ivice;

    unsigned id;
    unsigned velicina_teksta = 15;
    float velicina_prstena = 5;
    bool ignorisi;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

    void azuriraj_poziciju_ivica();

    Graf *roditelj;

    QPointF drag_start;

    QString naziv;

    std::vector<Cvor *> nadji_susede();
    std::vector<std::pair<Cvor *,Ivica *>> nadji_par_susede();
    std::vector<std::pair<Cvor *,Ivica *>> nadji_ed_susede();
    std::vector<std::pair<Cvor *, Ivica *> >nadji_par_susede2();


};

class Graf :public QGraphicsScene
{
Q_OBJECT
public:
    Graf();
    unsigned curr_id = 0;

    void postavi_stanje(stanje_scene st);

    std::vector<Cvor*> cvorovi;
    std::vector<Cvor*> anim_cvorovi;
    std::vector<Ivica*> ivice;
    std::vector<Ivica*> anim_ivice;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

    void dodaj_cvor(QPointF mis);
    void dodaj_cvor(Cvor* c);
    void dodaj_ivicu();

    unsigned nadji_id();

    void BFS(Cvor *pocetni,bool nivoi);

    void ocisti_animacije();

    void izbrisi_sve();

    void sacuvaj_graf(std::string& putanja);

    void ucitaj_graf(std::string& putanja);

signals:
  void sig_nista();
  void sig_cvor(Cvor *cvor);
  void sig_ivica(Ivica *iv);
};

enum tip_dugmeta { Tegic , Protocni };

class Dugmic : public QGraphicsPixmapItem {
public:
    Dugmic(tip_dugmeta tip);

    void azuriraj_poziciju(Ivica *iv);
};

class Algoritam : public QThread {
Q_OBJECT
public:
    Algoritam(QObject *parent = nullptr);

    Graf * g = nullptr;

    Cvor *pocetak = nullptr;

    Cvor *meta = nullptr;

    Cvor *izvor = nullptr;
    Cvor *ponor = nullptr;

    unsigned brzina = 60;

    void bfs();

    void dfs();

    void fleury();

    void fleury2();

    void fleury3();

    void kruskal();

    void dijsktra();

    void primov();

    void edmonds();

    void ford_fulker();

    void edmond_carp();

    unsigned method = 0;

    bool bfs_nivoi = false;

    bool belezi_mostove = false;

    bool belezi_stepene = false;

    bool labeliraj_ivice = false;

    bool labeliraj_cvorove = false;

    bool obidji_ceo_graf = false;


signals:
  void boji_cvor(Cvor *cv,QColor col);
  void anim_boji_cvor(Cvor *cv,QColor col1,QColor col2,float param);
  void okviri_cvor(Cvor *cv,QRectF rc,float len, float param);
  void boji_ivica(Ivica *iv,QColor col);
  void stavi_tekst_cvor(Cvor *cv,QString txt);
  void stavi_tekst_ivica(Ivica *iv,QString txt);
  void animiraj_ivicu(Ivica *iv,Cvor *p,Cvor *d,QColor col,float param);
  void transp_cvor_sve(Cvor *c,float val);
  void transp_ivicu(Ivica *iv,float val);
  void kraj_animacije();
  void error_box(QString txt);
  void dodaj_text(QString txt);
  void dodaj_anim_cvor(Cvor* c);
  void dodaj_anim_ivicu(Ivica *i);
  void pomeri_cvor(Cvor *c,QPointF p);
protected:
    void run() override;
};

#endif // GRAF_HPP
