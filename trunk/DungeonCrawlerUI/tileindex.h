#ifndef TILEINDEX_H
#define TILEINDEX_H

class TileIndex
{
public:
    explicit TileIndex(int row = 0, int col = 0);

    bool operator==(const TileIndex& t);
    bool operator!=(const TileIndex& t);

    int row;
    int col;
};

#endif // TILEINDEX_H
