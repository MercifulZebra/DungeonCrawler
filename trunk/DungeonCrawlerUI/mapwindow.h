#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QWidget>
#include <QVector>
#include <QOpenGLWidget>

#include <QPen>
#include <QBrush>

#include "tileindex.h"
#include "tooltypes.h"

namespace logger {
class Logger;
}

class ConfigAccessor;

class Tile;

enum ActionModifier {
    NoModifier,
    ShiftModifier,
    CtrlModifier,
    AltModifier,
    ShiftCtrlModifier
};

enum Action {
    NoAction,
    MoveAction,
    SelectBoxAction,
    SelectLocalAction,
    PaintAction
};

class MapWindow : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MapWindow(QWidget *parent = 0);

    bool initWindow(ConfigAccessor *accessor, logger::Logger *nLog);

    //Interaction Functions
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

    void handleMousePressLeft(QMouseEvent *e);
    void handleMousePressRight(QMouseEvent *e);
    void handleMouseReleaseLeft(QMouseEvent *e);
    void handleMouseReleaseRight(QMouseEvent *e);

    void handleMouseMove(QMouseEvent *e);
    void handleRightMouseMove(QMouseEvent *e);
    void handleMoveLocation(QMouseEvent *e);

    void setActiveMouseModifiers(QMouseEvent *e);

    void startSelectBoxAction(QMouseEvent *e);
    void startSelectLocalAction(QMouseEvent *e);
    void startMoveAction(QMouseEvent *e);
    void startPaintAction(QMouseEvent *e);

    void updateSelectBoxAction(QMouseEvent *e);
    void updateSelectLocalAction(QMouseEvent *e);
    void updateMoveAction(QMouseEvent *e);
    void updatePaintAction(QMouseEvent *e);

    void cancelCurrentAction();
    void cancelSelectAction();
    void cancelMoveAction();
    void cancelPaintAction();

    //Interaction Functions
    void setCurrentTool(ToolTypes type);

    //Painting Functions
    void paintEvent(QPaintEvent *e) override;
    void paintThis(QPainter *painter, QPaintEvent *pEvent);
    void paintBackground(QPainter *painter, QPaintEvent *pEvent);
    void paintDebugText(QPainter *painter);
    void paintTiles(QPainter *painter);
    void paintCenterMark(QPainter *painter);
    void paintScrollBars(QPainter *painter);

    //TileArray Management functions
    bool changeSize(int nWidth, int nHeight, bool force_flag = false);
    int getTileArraySize();
    int getTileArrayWidthPix();
    int getTileArrayHeightPix();
    int getTileArrayColumnCount();
    int getTileArrayRowCount();

    void checkHoveredTile(QPoint position);

    void setSelectedTiles(TileIndex startIndex, TileIndex endIndex);
    void clearSelectedTiles();
    QVector<Tile*> getTilesInArea(TileIndex startIndex, TileIndex endIndex);

    // Coordinate Helper Functions
    void updateMaxOffsets();
    void updateTileLocations();
    double boundOffset(double offset, double lowerBound, double upperBound);
    bool boxWithinView(QRect box, QRect view);
    QRect getTileRect(int rowIndex, int columnIndex);
    Tile* getTileAt(int x_pix, int y_pix);
    int getRowAt(int y_pix);
    int getColAt(int x_pix);

    // Debugging
    void setDebugLine(int row, QString text);
    QString actionToString(Action cAction);
    QString toolToString(ToolTypes type);

private:
    bool setDimensions(int nRows, int nCols);

    Tile* getNewTile();
    void returnTile(Tile* oldTile);

private:
    logger::Logger *log;

    QVector<QVector<Tile*>> tileArray;
    QVector<Tile*>         tilePool;

    QPoint lastMousePosition;
    TileIndex mousePressStartIndex;

    ToolTypes       currentTool;
    ActionModifier  currentMouseModifier;
    Action          currentAction;

    bool leftMousePressed_flag;
    bool rightMousePressed_flag;

    //Coordinate System
    double northingOffset_inch;
    double eastingOffset_inch;
    double inchPerPixel;

    double maxNorthingUpperOffset_inch;
    double maxNorthingLowerOffset_inch;
    double maxEastingLeftOffset_inch;
    double maxEastingRightOffset_inch;

    QPoint previousMouse_pos;
    QPoint currentMouse_pos;

    double tileWidth_inches;
    double tileHeight_inches;

    int marginWidth_pix;
    int marginHeight_pix;

    //Tile Status
    Tile *hoveredTile;
    QVector<Tile*> selectedTiles;

    //Debugging
    double paintCycleTime_s;
    QStringList debugLines;

    //Painting Tools
    bool firstPaint_flag;

    QPen    debugTextPen;
    QFont   debugTextFont;

    QBrush  backgroundBrush;

signals:

public slots:
};

#endif // MAPWINDOW_H
