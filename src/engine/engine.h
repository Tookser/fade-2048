#pragma once

#include <utility>
#include <vector>
#include <optional>

// TEngine - логика игры

enum EEngineSettings {
    TILES_AT_START = 2,
    SIZE_OF_FIELD_X = 4,
    SIZE_OF_FIELD_Y = 4
};

//const int APPEAR_CONST = 1;

enum class ETurnDirection {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum class EEngineTileType {
    TILE_0,
    TILE_1,
    TILE_2,
    TILE_4,
    TILE_8,
    TILE_16,
    TILE_32,
    TILE_64,
    TILE_128,
    TILE_256,
    TILE_512,
    TILE_1024,
    TILE_2048,
};



struct TShiftOfTile {
    int x_old, y_old;
    int x_new, y_new;
    EEngineTileType type;
    bool unite_flag = false;
};

struct SNewTile {
    int x, y;
    EEngineTileType type;
    int cells_to_appear;
};

class TEngine {
    public:
        TEngine();
        TEngine(const std::vector<std::vector<EEngineTileType>> &field);
        
        void InitializeField();
        
        
        std::optional<std::pair<std::vector<TShiftOfTile>, std::vector<SNewTile>>> MakeTurn(ETurnDirection turn);
        std::pair<int, int> AfterTurn();
        
        EEngineTileType operator()(int x, int y) const; // возвращение тайла
        
        int GetXSize() const;
        int GetYSize() const;
        
        bool IsEnd() const;
        
        bool IsWin() const;
        bool IsLose() const;
        
    private:
        std::vector<std::vector<EEngineTileType>> state; 
        
        bool win_flag, lose_flag;
        
        
        void AddTile(int x, int y, EEngineTileType tile);
        std::pair<int, int> AddRandomTile(bool only_2);
    
        static EEngineTileType GetDoubleTile(EEngineTileType tile);
        static EEngineTileType GetWinTile();
        
        static void Transpose(TShiftOfTile &s);
        
        std::vector<EEngineTileType> GetLine(bool vertical, int line_number) const;
        void WriteLine(bool vertical, int line_number, const std::vector<EEngineTileType> &new_line);
        
        bool MakeTurnLine(bool vertical, int line_number, ETurnDirection turn, std::vector<TShiftOfTile> &shifts, std::vector<SNewTile> &appear_tiles);
        
        void RefreshWinLoseState();
};

