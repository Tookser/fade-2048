#include <stdexcept>
#include <cmath>
#include <optional>

#include <display/display.h>
#include <display/view.h>
#include <engine/engine.h>
#include <motor/motor.h>

using namespace std;

void TMotor::Run() {
    TDisplay display;
    TEngine engine;
    TView view(&display);
    
    while (!display.Closed()) {
        display.ProcessEvents();
        
        if (!engine.IsEnd()) {
            auto turn = view.GetTurn();
            
            if (turn) {
                auto shifts = engine.MakeTurn(*turn);
                if (shifts) {
                    engine.AfterTurn();
                    view.Animate((*shifts).first, (*shifts).second, engine);
                    
                    
                    /*int x = random_tile.first;
                    int y = random_tile.second;
                    view.AnimateRandomTile(x, y, engine);*/
                }
            }
            
            view.Render(engine);
        } else if (engine.IsWin()) {
            view.WinScreen(engine);
        } else if (engine.IsLose()) {
            view.LoseScreen(engine);
        } else {
            throw runtime_error("Inconsistent IsEnd, IsWin, IsLose result");
        }
    }
}