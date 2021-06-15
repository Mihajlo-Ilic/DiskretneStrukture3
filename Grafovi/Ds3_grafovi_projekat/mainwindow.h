#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Graf/graf.hpp"

#include <QMainWindow>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void kraj_animacije();

    void boji_cvor(Cvor *cv,QColor col);

    void boji_ivicu(Ivica *iv,QColor col);

    void on_actionBiranje_triggered();

    void on_actionCvorovi_triggered();

    void on_actionIvice_triggered();

    void izaberi_cvor(Cvor *cvor);

    void izaberi_prazno();

    void izaberi_ivicu(Ivica *ivica);

    void on_metodaComboBox_currentIndexChanged(int index);

    void on_actionPusti_triggered();

    void on_actionAnimacija_triggered();

    void on_velicinaTekstaSpinBox_valueChanged(int arg1);

    void on_debljinaPrstenaDoubleSpinBox_valueChanged(double arg1);

    void on_velicinaCvoraDoubleSpinBox_valueChanged(double arg1);

    void on_tezinaDoubleSpinBox_valueChanged(double arg1);

    void on_protokDoubleSpinBox_valueChanged(double arg1);

    void on_dotokDoubleSpinBox_valueChanged(double arg1);

    void stavi_tekst_cvor(Cvor *cv,QString txt);

    void animiraj_ivicu(Ivica *iv,Cvor *p,Cvor *d,QColor col,float param);

    void on_koristiTezineCheckBox_stateChanged(int arg1);

    void on_usmerenostComboBox_currentIndexChanged(int index);

    void on_actionOcisti_triggered();

    void on_actionIzbrisi_triggered();

    void on_skalaDoubleSpinBox_valueChanged(double arg1);

    void error_box(QString txt);

    void transp_cvor_sve(Cvor *c,float val);

    void stavi_tekst_ivica(Ivica *iv,QString txt);

    void transp_ivicu(Ivica *iv,float val);

    void anim_boji_cvor(Cvor *cv,QColor col1,QColor col2,float param);

    void okviri_cvor(Cvor *cv,QRectF rc,float len, float param);

    void dodaj_text(QString txt);

    void on_pushButton_clicked();

    void on_nazivCvoraLineEdit_textChanged(const QString &arg1);

    void dodaj_anim_cvor(Cvor* c);

    void dodaj_anim_ivicu(Ivica *i);

    void pomeri_cvor(Cvor *c,QPointF p);
    void on_tipIviceComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
