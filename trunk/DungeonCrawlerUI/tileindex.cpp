#include "tileindex.h"

TileIndex::TileIndex(int row, int col) :
    row(row),
    col(col)
{

}

bool TileIndex::operator==(const TileIndex &t) {
    if ((t.col == col) && (t.row == row)) {
        return true;
    }

    return false;
}

bool TileIndex::operator!=(const TileIndex &t) {
    return !operator ==(t);
}
