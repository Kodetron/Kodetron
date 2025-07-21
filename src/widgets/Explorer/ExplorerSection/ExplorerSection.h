#ifndef EXPLORERSECTION_H
#define EXPLORERSECTION_H

#include "../ExplorerCard/ExplorerCard.h"
#include <QVBoxLayout>
#include <QWidget>

class ExplorerSection : public QWidget {
    Q_OBJECT

  public:
    explicit ExplorerSection(QWidget *parent = nullptr);
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();

  private:
    ExplorerCard *explorer_card;
    QVBoxLayout *layout;

    // Add any private members or methods needed for the ExplorerSection
    // For example, you might want to add buttons or a layout here
};

#endif // EXPLORERSECTION_H