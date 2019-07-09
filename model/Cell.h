#ifndef SLITHER_IO_CELL_H
#define SLITHER_IO_CELL_H


class Cell {
public:
    Cell(const int x, const int y, const bool wall, int foodBenefit, int clientInGameId,
         const bool portal, const int portalX, const int portalY);

    virtual ~Cell();

    int getFoodBenefit() const;
    void setFoodBenefit(int foodBenefit);

    int getClientInGameId() const;
    void setClientInGameId(int clientInGameId);

    const int getX() const;
    const int getY() const;

    const bool isWall() const;

    const bool isPortal() const;
    const int getPortalX() const;
    const int getPortalY() const;

private:
    const int x, y;

    const bool wall;

    const bool portal;
    const int portalX, portalY;

    int foodBenefit;

    int clientInGameId;
};


#endif //SLITHER_IO_CELL_H
