#pragma once

#include <engine/engine.h>
#include <display/view.h>
#include <display/display.h>

#include <utility>
#include <optional>
#include <algorithm>

const double MOVE_TIME = 0.7; // максимальное время перемещения
const double RANDOM_TILE_TIME = 0.15;
const int MAX_MOTION_LENGTH = std::max(SIZE_OF_FIELD_X, SIZE_OF_FIELD_Y) - 1;


class TView {
    public:
        TView(TDisplay *display_arg);
        ~TView();
        
        std::optional<ETurnDirection> GetTurn();
        
        virtual void Render(const TEngine &engine);

        virtual void Animate(const std::vector<TShiftOfTile> shifts, const std::vector<SNewTile> new_tiles, const TEngine &engine);
        void AnimateRandomTile(int x, int y, const TEngine &engine);

        virtual void WinScreen(const TEngine &engine);
        virtual void LoseScreen(const TEngine &engine);
        
        //static void PrintShifts(const std::vector<TShiftOfTile> shifts);

    private:
        TDisplay *display;
        
        bool turn_flag; // true если ход сделан, а кнопки ещё не отжаты
        
        static ETurnDirection KeyToDirection(EKey key);
        static std::optional<ETileType> TileToTile(EEngineTileType tile);
        
        
        void DrawField(const TEngine &engine);
        
        std::pair<std::optional<EKey>, bool> GetKeyPressed();
        
        //АНИМАЦИЯ
        static int GetLengthOfMotion(const std::vector<TShiftOfTile> shifts);
        std::pair<double, double> GetCoordinates(double elapsed_time, double full_move_time, int length_of_motion, const TShiftOfTile &shift);
        float GetTransparency(double elapsed_time, double current_move_time, const TShiftOfTile &shift, const std::pair<double, double> &coordinates);
};