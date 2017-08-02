#include "mapwindow.h"

#include "logger.h"
#include "tile.h"

#include <QApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QStringList>

MapWindow::MapWindow(QWidget *parent) : QOpenGLWidget(parent),
    log(NULL),
    tileArray(),
    mousePressStartIndex(),
    northingOffset_inch(0.0),
    eastingOffset_inch(0.0),
    inchPerPixel(.5),
    maxNorthingUpperOffset_inch(0.0),
    maxNorthingLowerOffset_inch(0.0),
    maxEastingLeftOffset_inch(0.0),
    maxEastingRightOffset_inch(0.0),
    previousMouse_pos(),
    currentMouse_pos(),
    tileWidth_inches(60),
    tileHeight_inches(60),
    marginWidth_pix(200),
    marginHeight_pix(200),
    hoveredTile(NULL),
    selectedTiles(),
    paintCycleTime_s(0.0),
    debugLines(),
    firstPaint_flag(true),
    debugTextPen(),
    debugTextFont()
{
}

bool MapWindow::initWindow(QString /*config_filename*/, logger::Logger *nLog) {
    bool initSuccess_flag = true;

    log = nLog;

    setMouseTracking(true);

    //Set Up Tiles
    changeSize(12, 12, true);

    updateTileLocations();
    updateMaxOffsets();

    //Init Paint Tools
    debugTextPen = QPen(Qt::white);
    debugTextFont.setPixelSize(20);

    backgroundBrush = QBrush(QColor(0, 0, 0));

    return initSuccess_flag;
}
/*
 * Controls
 *
 * L-Click & Drag -> Move
 * Shift L-Click & Drag -> Select Area
 * Ctrl L-Click & Drag -> Add Dragged Tiles to Select
 *
 * Right Click -> Deselect Tiles / Current Tool
 *  Shift Right Click -> Reselect Last Tool
 *
 *
 *
 *
 *
 *
 */

void MapWindow::mousePressEvent(QMouseEvent *e) {

    if (e->buttons() == Qt::LeftButton) {
        if (QApplication::keyboardModifiers() == Qt::ControlModifier) {

        }
        else if (QApplication::keyboardModifiers() == Qt::ShiftModifier) {

        }
        else if (QApplication::keyboardModifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {

        }
        else {

        }

        int adjustedMousePos_x = e->x() - (this->width() / 2);
        int adjustedMousePos_y = e->y() - (this->height() / 2);
        mousePressStartIndex.row = getRowAt(adjustedMousePos_y);
        mousePressStartIndex.col = getColAt(adjustedMousePos_x);

        setSelectedTiles(mousePressStartIndex, mousePressStartIndex);
    }

    previousMouse_pos = e->pos();

    update();
}

void MapWindow::mouseReleaseEvent(QMouseEvent * /*event*/) {
    previousMouse_pos = QPoint(-1, -1);
    clearSelectedTiles();
}

void MapWindow::mouseMoveEvent(QMouseEvent *e) {


    if (e->buttons() == Qt::LeftButton) {

    }
    else if (e->buttons() == Qt::MidButton) {

    }
    else {

    }

    checkHoveredTile(e->pos());

    previousMouse_pos = e->pos();
    QOpenGLWidget::mouseMoveEvent(e);
    update();
}

void MapWindow::wheelEvent(QWheelEvent *e) {
    double rotDelta = static_cast<float>(e->angleDelta().y());

    inchPerPixel = inchPerPixel - (inchPerPixel * (rotDelta / 1200.0));
    inchPerPixel = floor((inchPerPixel * 100) + 0.5) / 100;

    if (inchPerPixel < 0.06) {
        inchPerPixel = 0.06;
    }

    updateTileLocations();
    updateMaxOffsets();

    checkHoveredTile(previousMouse_pos);

    northingOffset_inch = boundOffset(northingOffset_inch, maxNorthingUpperOffset_inch, maxNorthingLowerOffset_inch);
    eastingOffset_inch = boundOffset(eastingOffset_inch, maxEastingLeftOffset_inch, maxEastingRightOffset_inch);
    update();
}

void MapWindow::handleMouseMove(QMouseEvent *e) {

    currentMouse_pos = e->pos();

    if (e->buttons() == Qt::LeftButton) {

        TileIndex tIndex;
        int adjustedMousePos_x = e->x() - (this->width() / 2);
        int adjustedMousePos_y = e->y() - (this->height() / 2);
        tIndex.row = getRowAt(adjustedMousePos_y);
        tIndex.col = getColAt(adjustedMousePos_x);

        if (tIndex.row == -1 || tIndex.col == -1) {
            log->info(QString("%1 %2").arg(tIndex.row).arg(tIndex.col));
        }
        else {
            setSelectedTiles(mousePressStartIndex, tIndex);
        }
    }

    northingOffset_inch = boundOffset(northingOffset_inch, maxNorthingUpperOffset_inch, maxNorthingLowerOffset_inch);
    eastingOffset_inch = boundOffset(eastingOffset_inch, maxEastingLeftOffset_inch, maxEastingRightOffset_inch);

}

void MapWindow::handleMoveLocation(QMouseEvent * /*e*/) {

}

void MapWindow::handleRightMouseMove(QMouseEvent * /*e*/) {

}

void MapWindow::handleShiftMouseMove(QMouseEvent * /*e*/) {

}

void MapWindow::handleControlMouseMove(QMouseEvent * /*e*/) {

}

void MapWindow::paintEvent(QPaintEvent *e) {

    if (firstPaint_flag) {
        updateTileLocations();
        updateMaxOffsets();
    }

    QPainter painter;
    painter.begin(this);

    painter.setRenderHint(QPainter::Antialiasing);
    paintThis(&painter, e);
    painter.end();

}

void MapWindow::paintThis(QPainter *painter, QPaintEvent* pEvent) {

    QElapsedTimer timer;
    timer.start();

    paintBackground(painter, pEvent);
    paintTiles(painter);
    paintScrollBars(painter);
    paintCenterMark(painter);
    paintDebugText(painter);

    paintCycleTime_s = timer.elapsed();
}

void MapWindow::paintBackground(QPainter *painter, QPaintEvent *pEvent) {
    painter->save();
    painter->fillRect(pEvent->rect(), backgroundBrush);

    painter->restore();
}

void MapWindow::paintDebugText(QPainter *painter) {
    painter->save();

    painter->translate(0, height());

    QString line4 = QString("IPP: %1 TWpx: %2").arg(inchPerPixel).arg(tileWidth_inches / inchPerPixel);
    QString line3 = QString("NumTiles: %1 NumPool: %2").arg(getTileArraySize()).arg(tilePool.length());
    QString line2 = QString("Mouse Position: (%1, %2) North/East: (%3 / %4)").arg(currentMouse_pos.x()).arg(currentMouse_pos.y()).arg(northingOffset_inch).arg(eastingOffset_inch);
    QString line1 = QString("Last Paint Cycle Time: %1 s").arg(paintCycleTime_s / 1000, 0, 'g', 3);

    setDebugLine(1, line1);
    setDebugLine(2, line2);
    setDebugLine(3, line3);
    setDebugLine(4, line4);

    QString dText = debugLines.join("\n");

    QFontMetrics fm(painter->font());
    int pixelHeight = fm.height() * debugLines.count();
    int pixelWidth = fm.width(dText);

    int dWidth          = pixelWidth; //300;
    int dHeight         = pixelHeight; //110;
    int dLeftBorder     = 10;
    int dBottomBorder   = 10;

    QRect dRect(QPoint(dLeftBorder, -dBottomBorder - dHeight), QPoint(dLeftBorder + dWidth, -dBottomBorder));

    painter->drawText(dRect, dText);

    painter->restore();
}

void MapWindow::paintTiles(QPainter *painter) {
    painter->save();
    painter->translate(this->width() / 2, this->height() / 2);

    int leftSide_pix = -this->width() / 2;
    int rightSide_pix = this->width() / 2;

    int topSide_pix = -this->height() / 2;
    int botSide_pix = this->height() / 2;

    QRect viewBounds(QPoint(leftSide_pix, topSide_pix), QPoint(rightSide_pix, botSide_pix));

    painter->setPen(Qt::white);

    int numCols = tileArray.length();
    int numRows = 0;
    if (numCols > 0) {
        numRows = tileArray.at(0).length();
    }

    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < numRows; j++) {

            if (boxWithinView(tileArray.at(i).at(j)->getBoundingBox(), viewBounds)) {
                tileArray.at(i).at(j)->paintThis(painter);
            }
        }
    }

    painter->restore();
}

void MapWindow::paintCenterMark(QPainter *painter) {
    painter->save();

    painter->translate(this->width() / 2, this->height() / 2);
    painter->setPen(debugTextPen);

    painter->drawLine(QPoint(-10, 0), QPoint(-5, 0));
    painter->drawLine(QPoint(10, 0), QPoint(5, 0));
    painter->drawLine(QPoint(0, -10), QPoint(0, -5));
    painter->drawLine(QPoint(0, 10), QPoint(0, 5));

    painter->restore();
}

void MapWindow::paintScrollBars(QPainter *painter) {
    int tileArrayWidth_pix = getTileArrayWidthPix();
    int totalWidth_pix = tileArrayWidth_pix + ((maxEastingRightOffset_inch - maxEastingLeftOffset_inch) / inchPerPixel);

    float percent = float(this->width()) / float(totalWidth_pix);

}

bool MapWindow::changeSize(int nWidth, int nHeight, bool force_flag) {

    bool sizeChanged_flag = false;

    if (nWidth != 0 && nHeight != 0) {
        bool continue_flag = true;
        if (!force_flag) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Continue?", "Resize may delete some existing tiles. Continue?", QMessageBox::Yes | QMessageBox::No);\

            if (reply != QMessageBox::Yes) {
                continue_flag = false;
            }
        }

        if (continue_flag) {
            sizeChanged_flag = setDimensions(nHeight, nWidth);
        }
    }
    else {
        log->warn("Unable to change MapWindow Size. Invalid entry '0'.");
        sizeChanged_flag = false;
    }

    return sizeChanged_flag;
}

int MapWindow::getTileArraySize() {
    int numTiles = 0;
    for (int i =0; i < tileArray.length(); i++) {
        numTiles = numTiles + tileArray.at(i).length();
    }
    return numTiles;
}

int MapWindow::getTileArrayWidthPix() {
    int numCols = tileArray.length();
    double rWidth = (float (numCols) * tileWidth_inches) / inchPerPixel;

    return rWidth;
}

int MapWindow::getTileArrayHeightPix() {

    int numRows = 0;
    double rHeight = 0;

    if (!tileArray.isEmpty()) {
        numRows = tileArray.at(0).length();
    }

    for (int i = 0; i < tileArray.length(); i++) {
        if (tileArray.at(i).length() != numRows) {
            log->warn(QString("Array size MisMatch. Column %1 length %2 does not match %3").arg(i).arg(tileArray.at(i).length()).arg(numRows));
        }
    }

    rHeight = (float (numRows) * tileHeight_inches) / inchPerPixel;

    return rHeight;

}

int MapWindow::getTileArrayColumnCount() {
    return tileArray.length();
}

int MapWindow::getTileArrayRowCount() {
    int rCount = -1;
    bool continueFlag = true;

    for (int i = 0; i < tileArray.length() && continueFlag; i++) {
        if ((tileArray.at(i).length()) != rCount && (rCount != -1)) {
            log->warn("MapWindow.getTileArrayRowCount Number of Rows does not match.");
            rCount = -1;
            continueFlag = false;
        }

        rCount = tileArray.at(i).length();
    }

    return rCount;
}

void MapWindow::checkHoveredTile(QPoint position) {
    int adjustedMousePos_x = position.x() - (this->width() / 2);
    int adjustedMousePos_y = position.y() - (this->height() / 2);

    bool accepted_flag = false;
    if (hoveredTile != NULL) {
        if (hoveredTile->contains(adjustedMousePos_x, adjustedMousePos_y)) {
            accepted_flag = true;
        }
        else {
            hoveredTile->setHovered(false);
            hoveredTile = NULL;
        }
    }
    if (!accepted_flag) {

        hoveredTile = getTileAt(adjustedMousePos_x, adjustedMousePos_y);

        if (hoveredTile != NULL) {
            hoveredTile->setHovered(true);
        }
//        for (int i = 0; i < tileArray.length() && !accepted_flag; i++) {
//            if (!tileArray.at(i).isEmpty()) {

//                int leftBound = tileArray.at(i).at(0)->getBoundingBox().left();
//                int rightBound = tileArray.at(i).at(0)->getBoundingBox().right();

//                if ((adjustedMousePos_x > leftBound) && (adjustedMousePos_x <= rightBound) ) {
//                    for (int j = 0; j < tileArray.at(i).length() && !accepted_flag; j++) {
//                        Tile *cTile = tileArray.at(i).at(j);
//                        if (cTile->contains(adjustedMousePos_x, adjustedMousePos_y)) {
//                            hoveredTile = cTile;
//                            hoveredTile->setHovered(true);
//                            accepted_flag = true;
//                        }
//                    }
//                }
//            }
//        }
    }
}

void MapWindow::setSelectedTiles(TileIndex startIndex, TileIndex endIndex) {
    clearSelectedTiles();

    selectedTiles = getTilesInArea(startIndex, endIndex);

    for (int i = 0; i < selectedTiles.count(); i++) {
        selectedTiles.at(i)->setSelected(true);
    }
}

void MapWindow::clearSelectedTiles() {
    for (int i = 0; i < selectedTiles.count(); i++) {
        selectedTiles.at(i)->setSelected(false);
    }

    selectedTiles.clear();
}

QVector<Tile*> MapWindow::getTilesInArea(TileIndex startIndex, TileIndex endIndex) {

    int lowestCol = startIndex.col;
    int lowestRow = startIndex.row;

    int highestCol = startIndex.col;
    int highestRow = startIndex.row;

    QVector<Tile*> rTiles;

    if (startIndex.row > endIndex.row) {
        lowestRow = endIndex.row;
    }
    else {
        highestRow = endIndex.row;
    }

    if (startIndex.col > endIndex.col) {
        lowestCol = endIndex.col;
    }
    else {
        highestCol = endIndex.col;
    }

    if (tileArray.count() >= highestCol) {
        for (int i = lowestCol; i < highestCol; i++) {
            if (tileArray.at(i).count() > highestRow) {
                for (int j = lowestRow; j < highestRow; j++) {
                    rTiles.append(tileArray.at(i).at(j));
                }
            }
        }
    }

    return rTiles;

}

bool MapWindow::setDimensions(int nRows, int nCols) {
    bool sizeChanged_flag = true;

    //Add Col if necessary
    while (tileArray.length() < nCols) {
        QVector<Tile*> nColumn;
        tileArray.append(nColumn);
    }

    //Remove Cols if necessary
    while (tileArray.length() > nCols) {
        QVector <Tile*> oldColumn = tileArray.takeLast();
        while(!oldColumn.isEmpty()) {
            returnTile(oldColumn.takeLast());
        }
    }

    //Adjust Row Length
    for (int i = 0; i < tileArray.length(); i++) {
        QVector<Tile*> &currentColumn = tileArray[i];

        //Add Col if necessary
        while (currentColumn.length() < nRows) {
            Tile* nRow = getNewTile();
            currentColumn.append(nRow);
        }

        //Remove Cols if necessary
        while (currentColumn.length() > nRows) {
            returnTile(currentColumn.takeLast());
        }

    }

    return sizeChanged_flag;
}

Tile* MapWindow::getNewTile() {
    Tile* rTile = NULL;

    if (!tilePool.isEmpty()) {
        rTile = tilePool.takeLast();
    }

    if (rTile == NULL) {
        rTile = new Tile(this);
    }

    return rTile;
}

void MapWindow::returnTile(Tile *oldTile) {
    tilePool.append(oldTile);
}

void MapWindow::updateMaxOffsets() {

    int numTilesLeft = static_cast<int> ((static_cast<double>(tileArray.length())/2.0) + 0.5);
    int numTilesRight = tileArray.length() - numTilesLeft;

    int numTilesTop = 0;
    int numTilesBot = 0;

    if (tileArray.length() > 0) {
        numTilesTop = static_cast<int> ((static_cast<double>(tileArray.at(0).length())/2.0) + 0.5);
        numTilesBot = tileArray.at(0).length() - numTilesTop;
    }

    QRect upperLeftBox = getTileRect(0, 0);
    QRect centerUpperLeftBox = getTileRect(numTilesTop - 1, numTilesLeft - 1);
    QRect lowerRightBox = getTileRect(getTileArrayRowCount() - 1, getTileArrayColumnCount() - 1);
    QRect centerLowerRightBox = getTileRect(numTilesBot, numTilesRight);

    int leftArrayDist = (upperLeftBox.left() - centerUpperLeftBox.right());
    int rightArrayDist = (lowerRightBox.right() - centerLowerRightBox.left());
    int upperArrayDist = (upperLeftBox.top() - centerUpperLeftBox.bottom());
    int lowerArrayDist = (lowerRightBox.bottom() - centerLowerRightBox.top());

    int halfWidth = this->width()/2;
    int halfHeight = this->height()/2;

    int totalWidth = (-leftArrayDist) + rightArrayDist;
    int totalHeight = (-upperArrayDist) + lowerArrayDist;

    int tMarginWidth = marginWidth_pix;
    int tMarginHeight = marginHeight_pix;

    if (totalWidth > this->width()) {
        // Give Full Margin
        int eastingLeftWidthDif_pix = (leftArrayDist - marginWidth_pix) + halfWidth;
        maxEastingLeftOffset_inch = eastingLeftWidthDif_pix * inchPerPixel;

        int eastingRightWidthDif_pix = (rightArrayDist + marginWidth_pix) - halfWidth;
        maxEastingRightOffset_inch = eastingRightWidthDif_pix * inchPerPixel;
    }
    else if ((totalWidth + 2*tMarginWidth) > this->width()) {
        // Give Suppressed Margin
        int eastingLeftWidthDif_pix = -(tMarginWidth/2 - (leftArrayDist + halfWidth));
        maxEastingLeftOffset_inch = eastingLeftWidthDif_pix * inchPerPixel;

        int eastingRightWidthDif_pix = tMarginWidth/2 - (halfWidth - rightArrayDist);
        maxEastingRightOffset_inch = eastingRightWidthDif_pix * inchPerPixel;
    }
    else {
        maxEastingLeftOffset_inch = 0;
        maxEastingRightOffset_inch = 0;
    }

    if (totalHeight > this->height()) {
        int northingUpperHeightDif_pix = (upperArrayDist - marginHeight_pix) + halfHeight;
        maxNorthingUpperOffset_inch = northingUpperHeightDif_pix * inchPerPixel;

        int northingLowerHeightDif_pix = (lowerArrayDist + marginHeight_pix) - halfHeight;
        maxNorthingLowerOffset_inch = northingLowerHeightDif_pix * inchPerPixel;
    }
    else if ((totalHeight + 2*tMarginHeight) > this->height()) {
        // Give Suppressed Margin
        int northingUpperHeightDif_pix = -(tMarginHeight/2 - (upperArrayDist + halfHeight));
        maxNorthingUpperOffset_inch = northingUpperHeightDif_pix * inchPerPixel;

        int northingLowerHeightDif_pix = (tMarginHeight/2 - (halfHeight - lowerArrayDist));
        maxNorthingLowerOffset_inch = northingLowerHeightDif_pix * inchPerPixel;
    }
    else {
        maxNorthingUpperOffset_inch = 0;
        maxNorthingLowerOffset_inch = 0;
    }

    if (maxEastingLeftOffset_inch > 0) {
        maxEastingLeftOffset_inch = 0;
    }

    if (maxEastingRightOffset_inch < 0) {
        maxEastingRightOffset_inch = 0;
    }

    if (maxNorthingUpperOffset_inch > 0) {
        maxNorthingUpperOffset_inch = 0;
    }

    if (maxNorthingLowerOffset_inch < 0) {
        maxNorthingLowerOffset_inch = 0;
    }

}

void MapWindow::updateTileLocations() {

    int northingOffset_pix = int ((-northingOffset_inch / inchPerPixel) + 0.5);
    int eastingOffset_pix = int ((-eastingOffset_inch / inchPerPixel) + 0.5);

    int tileWidth = int((tileWidth_inches / inchPerPixel) + 0.5);
    int tileHeight = int((tileHeight_inches / inchPerPixel) + 0.5);

    int numTilesLeft = static_cast<int> ((static_cast<double>(tileArray.length())/2.0) + 0.5);
    int numTilesRight = tileArray.length() - numTilesLeft;

    int numTilesTop = 0;
    int numTilesBot = 0;

    if (tileArray.length() > 0) {
        numTilesTop = static_cast<int> ((static_cast<double>(tileArray.at(0).length())/2.0) + 0.5);
        numTilesBot = tileArray.at(0).length() - numTilesTop;
    }

    //Draw Left Half
    for (int i = 0; i < numTilesLeft; i++) {
        //Top Half
        for (int j = 0; j < numTilesTop; j++) {

            int tileRight_pos = -(i * tileWidth) + eastingOffset_pix;
            int tileBot_pos = -(j * tileHeight) + northingOffset_pix;

            int tileLeft_pos = tileRight_pos - tileWidth;
            int tileTop_pos = tileBot_pos - tileHeight;

            QRect tileBounds(QPoint(tileLeft_pos, tileTop_pos), QPoint(tileRight_pos, tileBot_pos));
            tileArray.at(numTilesLeft - (i + 1)).at(numTilesTop - (j + 1))->setBoundingBox(tileBounds);
        }

        //Bot Half
        for (int j = 0; j < numTilesBot; j++) {
            int tileRight_pos = -(i * tileWidth) + eastingOffset_pix;
            int tileTop_pos = (j * tileHeight) + northingOffset_pix;

            int tileLeft_pos = tileRight_pos - tileWidth;
            int tileBot_pos = tileTop_pos + tileHeight;

            QRect tileBounds(QPoint(tileLeft_pos, tileTop_pos), QPoint(tileRight_pos, tileBot_pos));
            tileArray.at(numTilesLeft - (i + 1)).at(j + numTilesTop)->setBoundingBox(tileBounds);

        }
    }

    //Right Half
    for (int i = 0; i < numTilesRight; i++) {
        //Top Half
        for (int j = 0; j < numTilesTop; j++) {
            int tileLeft_pos = (i * tileWidth) + eastingOffset_pix;
            int tileBot_pos = -(j * tileHeight) + northingOffset_pix;

            int tileRight_pos = tileLeft_pos + tileWidth;
            int tileTop_pos = tileBot_pos - tileHeight;

            QRect tileBounds(QPoint(tileLeft_pos, tileTop_pos), QPoint(tileRight_pos, tileBot_pos));
            tileArray.at(i + numTilesLeft).at(numTilesTop - (j + 1))->setBoundingBox(tileBounds);
        }

        //Bot Half
        for (int j = 0; j < numTilesBot; j++) {
            int tileLeft_pos = (i * tileWidth) + eastingOffset_pix;
            int tileTop_pos = (j * tileHeight) + northingOffset_pix;

            int tileRight_pos = tileLeft_pos + tileWidth;
            int tileBot_pos = tileTop_pos + tileHeight;

            QRect tileBounds(QPoint(tileLeft_pos, tileTop_pos), QPoint(tileRight_pos, tileBot_pos));
            tileArray.at(i + numTilesLeft).at(j + numTilesTop)->setBoundingBox(tileBounds);
        }
    }
}

double MapWindow::boundOffset(double offset, double lowerBound, double upperBound) {
    double rOffset = offset;
    if (rOffset < lowerBound) {
        rOffset = lowerBound;
    }

    if (rOffset > upperBound) {
        rOffset = upperBound;
    }

    return rOffset;
}

bool MapWindow::boxWithinView(QRect box, QRect view) {
    bool within_flag = false;

    if (((box.left() < view.right() && box.left() > view.left()) ||
        (box.right() < view.right() && box.right() > view.left() )) &&
        ((box.top() < view.bottom() && box.top() > view.top()) ||
        (box.bottom() < view.bottom() && box.bottom() > view.top() ))) {
        within_flag = true;
    }

    return within_flag;
}

QRect MapWindow::getTileRect(int rowIndex, int columnIndex) {

    QRect rRect;
    if (tileArray.length() > columnIndex) {
        if (tileArray.at(columnIndex).length() > rowIndex) {
            rRect = tileArray.at(columnIndex).at(rowIndex)->getBoundingBox();
        }
        else {
            log->warn(QString("MapWindow.getTileRect Unable to get Tile Rect. Row Index %1 larger than size %2").arg(rowIndex).arg(tileArray.at(columnIndex).length()));
        }
    }
    else {
        log->warn(QString("MapWindow.getTileRect Unable to get Tile Rect. Column Index %1 larger than size %2").arg(columnIndex).arg(tileArray.length()));
    }

    return rRect;
}

Tile* MapWindow::getTileAt(int x_pix, int y_pix) {

    bool accepted_flag = false;
    Tile* rTile = NULL;

    for (int i = 0; i < tileArray.length() && !accepted_flag; i++) {
        if (!tileArray.at(i).isEmpty()) {

            int leftBound = tileArray.at(i).at(0)->getBoundingBox().left();
            int rightBound = tileArray.at(i).at(0)->getBoundingBox().right();

            if ((x_pix > leftBound) && (x_pix <= rightBound) ) {
                for (int j = 0; j < tileArray.at(i).length() && !accepted_flag; j++) {
                    Tile *cTile = tileArray.at(i).at(j);
                    if (cTile->contains(x_pix, y_pix)) {
                        rTile = cTile;
                        accepted_flag = true;
                    }
                }
            }
        }
    }

    return rTile;
}

int MapWindow::getRowAt(int y_pix) {
    bool accepted_flag = false;
    int rIndex = -1;

    if (tileArray.count() > 0) {
        for (int i = 0; i < tileArray.at(0).count() && !accepted_flag; i++) {
            int upperBound = tileArray.at(0).at(i)->getBoundingBox().top();
            int lowerBound = tileArray.at(0).at(i)->getBoundingBox().bottom();

            if ((y_pix > upperBound) && (y_pix <= lowerBound)) {
                rIndex = i;
                accepted_flag = true;
            }
        }
    }

    return rIndex;
}

int MapWindow::getColAt(int x_pix) {
    bool accepted_flag = false;
    int rIndex = -1;

    for (int i = 0; i < tileArray.count() && !accepted_flag; i++) {
        if (!tileArray.at(i).isEmpty()) {

            int leftBound = tileArray.at(i).at(0)->getBoundingBox().left();
            int rightBound = tileArray.at(i).at(0)->getBoundingBox().right();

            if ((x_pix > leftBound) && (x_pix <= rightBound)) {
                rIndex = i;
                accepted_flag = true;
            }
        }
    }

    return rIndex;
}

void MapWindow::setDebugLine(int row, QString text) {
    while ((debugLines.count() < row)) {
        debugLines.prepend("");
    }

    debugLines[debugLines.count() - (row)] = text;
}
