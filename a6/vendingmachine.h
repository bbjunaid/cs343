#ifndef _VENDINGMACHINE_H__
#define _VENDINGMACHINE_H__

// forward declarations
_Monitor Printer;
_Task NameServer;
class WATCard;

_Task VendingMachine {
    void main();
  public:
    // flavours of sodas
    enum Flavours { BLACK_CHERRY, CREAM_SODA, ROOT_BEER, JAZZ_LIME, NUM_FLAVOURS };
    // purchase status: successful buy, out of stock, insufficient funds
    enum Status { BUY, STOCK, FUNDS };     
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id,\
                    unsigned int sodaCost, unsigned int maxStockPerFlavour );
    Status buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();

  private:
    Printer &m_prt;
    NameServer &m_nameServer;
    unsigned int m_id;
    unsigned int m_sodaCost;
    unsigned int m_maxStockPerFlavour;
    bool m_restocking;
    unsigned int m_stockedSoda[NUM_FLAVOURS];
};

#endif // _VENDINGMACHINE_H__

