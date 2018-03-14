#include "catitems.h"
#include "ui_catitems.h"

CatItems::CatItems(ContextManager *context, Ephimeral *ephim, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CatItems)
{
    ui->setupUi(this);
    localContext = context;
    curentReservation = ephim;
    row = 0; col = 0;
    catPath.append("NONE");
    minb.setHeight(100); minb.setWidth(100);
    maxb.setHeight(200); maxb.setWidth(200);
    nonselectedItems = localContext->getExistingItems();
    selectedButton = "background-color: rgb(102, 102, 153); font: 12pt 'Terminal'; color: rgb(255, 255, 255)";
    standardButton = "background-color: rgb(102, 153, 153); font: 12pt 'Terminal'; color: rgb(255, 255, 255)";
    catButtonmeow = "background-color: rgb(255, 204, 0); font: 12pt 'Terminal'; color: rgb(0, 0, 0)";
    unavailButton = "background-color: rgb(0, 0, 0); font: 12pt 'Terminal'; color: rgb(255, 255, 255)";
    ui->selExamp->setStyleSheet(selectedButton);
    ui->unselExamp->setStyleSheet(standardButton);
    ui->catExamp->setStyleSheet(catButtonmeow);
    timeCache = curentReservation->getTimeSpecifiedItems();


    qDebug() << "Available Items: ";
    for(QString key : timeCache.availableItems.keys())
    {
        qDebug() << key << " => " << timeCache.availableItems.value(key).barcode << "\n";
    }

    qDebug() << "UnAvailable Items: ";
    for(QString key : timeCache.unAvailableItems.keys())
    {
        qDebug() << key << " => " << timeCache.unAvailableItems.value(key).barcode << "\n";
    }

    qDebug() << "\n\nEND\n\n";





    loadButtonMap();
    updateGrid();
}

CatItems::~CatItems()
{
    delete ui;
}

void CatItems::shutdownWindow()
{
    emit closeChildren();
    emit catItemsClosed();
    this->close();
}

void CatItems::forceClose()
{
    shutdownWindow();
}

void CatItems::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    shutdownWindow();
}

void CatItems::on_searchButton_clicked()
{

}

void CatItems::on_finalizeButton_clicked()
{

}

void CatItems::on_upButton_clicked()
{
    ui->scrollArea->scroll(0, -40);
}

void CatItems::on_backButton_clicked()
{
    if(catPath.length() == 1)
        return;

    popCatPath();
    updateGrid();
}

void CatItems::on_downButton_clicked()
{
    ui->scrollArea->scroll(0, 40);
}

void CatItems::some_cat_click()
{
    QPushButton *button = (QPushButton*)sender();
    enterCatPath(labelMap.value(button->text()));
    updateGrid();
}

void CatItems::some_item_click()
{
    QPushButton *button = (QPushButton*)sender();
    QString selectedItem = button->text();
    button->setStyleSheet(selectedButton);
    for(auto i = nonselectedItems.begin(); i != nonselectedItems.end(); ++i)
    {
        if((*i).name == labelMap.value(selectedItem))
        {
            itemsSelected.push_back(*i);
        }
    }
}

void CatItems::some_item_unclick()
{
    QPushButton *button = (QPushButton*)sender();
    button->setStyleSheet(standardButton);
    QString selectedItem = button->text();

    itemsSelected.erase(
                std::remove(
                    itemsSelected.begin(),
                    itemsSelected.end(),
                    labelMap.value(selectedItem)
                    ),
                itemsSelected.end());
}

void CatItems::some_unavailable_click()
{
    QPushButton *button = (QPushButton*)sender();
    button->setStyleSheet(standardButton);
    QString selectedItem = button->text();
    button->setStyleSheet(unavailButton);


    qDebug() << "Unavailable item : " << labelMap.value(selectedItem) << " was selected - bring up their info.";

    // Just for show
    QString unavailBarcode = timeCache.unAvailableItems.value(labelMap.value(selectedItem)).barcode;

    // Pull items full schedule from the thing
    schedule itemsSchedule = timeCache.schedUnavail.value(unavailBarcode);

    // Get the actual entry that contains issues for this time frame
    scheduleConflict which_entry = itemsSchedule.checkForConflict(
                curentReservation->getStart(), curentReservation->getEnd()
                );



}

void CatItems::loadButtonMap()
{
    buttonMap.clear();
    std::vector<itemCategories> cats = localContext->getExistingCats();
    for(auto i = cats.begin(); i != cats.end(); ++i)
    {
        catWrap saran;
        saran.catDefinition = (*i);
        foreach(QString barcode, (*i).barcodes)
        {
            saran.itemsInCat.push_back(localContext->getItemByBarcode(barcode));
        }
        for(auto j = cats.begin(); j != cats.end(); ++j)
        {
            if((*j).parent == (*i).label)
            {
                saran.subCats.push_back((*j));
            }
        }
        buttonMap.insert((*i).label, saran);
    }
}

void CatItems::updateGrid()
{
    labelMap.clear();
    while ( QLayoutItem* item = ui->gridLayout->takeAt( 0 ) )
    {
        Q_ASSERT(!item->layout());
        delete item->widget();
        delete item;
    }

    // Spawn better, more relevant ones
    row = 0; col = 0;

    for(auto i = buttonMap[catPath.last()].subCats.begin();
        i != buttonMap[catPath.last()].subCats.end(); ++i)
    {
        if((*i).label != "NONE")
        {
            QString mapah = (*i).label;
            labelMap.insert(mapah.replace(" ", "\n"), (*i).label);

            // Load all sub categories
            QPushButton * categorrr = new QPushButton();
            categorrr->setText(mapah);
            categorrr->setMinimumSize(minb);
            categorrr->setMaximumSize(maxb);
            categorrr->setStyleSheet(catButtonmeow);
            connect(categorrr, SIGNAL(pressed()), this, SLOT(some_cat_click()));

            if(0 == col % 5)
            {
                row++;
                col = 0;
            }
            ui->gridLayout->addWidget(categorrr, row, col++);
        }
    }

    // Move one row down, and reset col for items
    col = 0; row++;
    for(auto i = buttonMap[catPath.last()].itemsInCat.begin();
        i != buttonMap[catPath.last()].itemsInCat.end(); ++i)
    {

        QString mapah = (*i).name;
        labelMap.insert(mapah.replace(" ", "\n"), (*i).name);

        // Load all category's items
        QPushButton * items = new QPushButton();
        items->setText(mapah);
        items->setMinimumSize(minb);
        items->setMaximumSize(maxb);

        if(timeCache.availableItems.contains((*i).name))
        {
            bool selected = false;
            for(auto k = itemsSelected.begin(); k != itemsSelected.end() && !selected; ++k)
            {
                if((*k).name == (*i).name)
                {
                    // Available - selected
                    selected = true;
                    items->setStyleSheet(selectedButton);
                    connect(items, SIGNAL(pressed()), this, SLOT(some_item_unclick()));
                }
            }
            if(!selected){

                // Available - unselected
                items->setStyleSheet(standardButton);
                connect(items, SIGNAL(pressed()), this, SLOT(some_item_click()));
            }
        } else {

            // Unavailable items
            items->setStyleSheet(unavailButton);
            connect(items, SIGNAL(pressed()), this, SLOT(some_unavailable_click()));
        }

        // Handle the row/ col
        if(0 == col % 5)
        {
            row++;
            col = 0;
        }
        ui->gridLayout->addWidget(items, row, col++);
    }
}

void CatItems::enterCatPath(QString path)
{
    catPath.append(path);
    updateCatPath();
}

void CatItems::popCatPath()
{
    catPath.pop_back();
    updateCatPath();
}

void CatItems::updateCatPath()
{
    ui->infoLabel->clear();
    if(catPath.length() > 5)
    {
        for(int i =0; i < catPath.length(); i++)
        {
            if(i ==0)
                ui->infoLabel->setText("\\root");
            else if( i == catPath.length()-1 || i == catPath.length())
                ui->infoLabel->setText(ui->infoLabel->text() + "\\" + catPath[i]);
            else
                ui->infoLabel->setText(ui->infoLabel->text()+"\\..");
        }
    }
    else
    {
        foreach(QString cat, catPath){
            if(cat == "NONE")
                ui->infoLabel->setText("\\root");
            else
                ui->infoLabel->setText(ui->infoLabel->text()+ "\\" + cat);
        }
    }
}
