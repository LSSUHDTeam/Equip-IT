#include "itemconfirmationdialog.h"
#include "ui_itemconfirmationdialog.h"

ItemConfirmationDialog::ItemConfirmationDialog(ContextManager * context,
                                               std::vector<reservableItems> selected,
                                               QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ItemConfirmationDialog)
{
    ui->setupUi(this);
    localContext = context;
    selectedItems = selected;

    pvalidate = new PeripheralValidator(this);
    connect(this, SIGNAL(pushCurrentItemToPeripheralValidator(std::vector<peripherals>)),
            pvalidate, SLOT(updateWindow(std::vector<peripherals>)));
    connect(pvalidate, SIGNAL(periphsValidated(std::vector<peripherals>)),
            this, SLOT(periphsUpdatedByValidator(std::vector<peripherals>)));
    connect(this, SIGNAL(closeChildren()),
            pvalidate, SLOT(forceClose()));
    pvalidate->setAttribute(Qt::WA_DeleteOnClose, true);
    periphWindow = ui->mdiArea->addSubWindow(pvalidate);
    periphWindow->setWindowFlags(Qt::FramelessWindowHint);
    periphWindow->showMaximized();

    buildItemButtons();
}

ItemConfirmationDialog::~ItemConfirmationDialog()
{
    delete periphWindow;
    delete ui;
}

void ItemConfirmationDialog::forceClose()
{
    shutdownWindow();
}

void ItemConfirmationDialog::periphsUpdatedByValidator(std::vector<peripherals> periphs)
{
    reservableItems item = buttonTextToItemMap.value(currentUserSelection);
    item.periphs = periphs;

    buttonTextToItemMap.remove(currentUserSelection);
    buttonTextToItemMap.insert(currentUserSelection, item);
}

void ItemConfirmationDialog::on_scrollUp_clicked()
{
    ui->scrollArea->scroll(0, -40);
}

void ItemConfirmationDialog::on_scrollDown_clicked()
{
    ui->scrollArea->scroll(0, 40);
}

void ItemConfirmationDialog::on_item_button_clicked()
{
    QPushButton *button = (QPushButton*)sender();
    currentUserSelection = button->text();
    loadPeripherals();
}

void ItemConfirmationDialog::on_complete_clicked()
{
    emit confirmed(selectedItems);
    shutdownWindow();
}

void ItemConfirmationDialog::buildItemButtons()
{
    int row = 0;
    QSize minb(100, 100); QSize maxb(200,200);
    for(auto i = selectedItems.begin(); i != selectedItems.end(); ++i)
    {
        QString mapah = (*i).name;
        buttonTextToItemMap.insert(mapah.replace(" ", "\n"), (*i));

        // Load all items
        QPushButton * iteeem = new QPushButton();
        iteeem->setText(mapah);
        iteeem->setMinimumSize(minb);
        iteeem->setMaximumSize(maxb);
        connect(iteeem, SIGNAL(pressed()), this, SLOT(on_item_button_clicked()));
        ui->gridLayout->addWidget(iteeem, row++, 0);
    }

    QPushButton * finalize = new QPushButton();
    finalize->setText("Complete");
    finalize->setMinimumSize(minb);
    finalize->setMaximumSize(maxb);
    connect(finalize, SIGNAL(pressed()), this, SLOT(on_complete_clicked()));
    ui->gridLayout->addWidget(finalize, row++, 0);
}

void ItemConfirmationDialog::loadPeripherals()
{
    emit pushCurrentItemToPeripheralValidator(
                buttonTextToItemMap.value(currentUserSelection).periphs
                );
}

void ItemConfirmationDialog::shutdownWindow()
{
    emit closeChildren();
    ui->mdiArea->removeSubWindow(periphWindow);
    this->close();
}
