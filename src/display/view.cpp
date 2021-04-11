#include <assert.h>
#include <optional>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <iomanip>

#include <display/view.h>
#include <display/display.h>
#include <engine/engine.h>





using namespace std;

TView::TView(TDisplay *display_arg)
        : display(display_arg)
        , turn_flag(false){
}

TView::~TView() {
}

pair<optional<EKey>, bool> TView::GetKeyPressed() {
    // второй элемент - нажата ли кнопка, 
    // первый элемент - кнопка, которая нажата, если она единственна, иначе nullopt
    
    unordered_map<EKey, bool> keys;
    keys[EKey::KEY_LEFT]    = display->IsKeyPressed(EKey::KEY_LEFT);
    keys[EKey::KEY_RIGHT]   = display->IsKeyPressed(EKey::KEY_RIGHT);
    keys[EKey::KEY_DOWN]    = display->IsKeyPressed(EKey::KEY_DOWN);
    keys[EKey::KEY_UP]      = display->IsKeyPressed(EKey::KEY_UP);
    
    bool one = false; 
    bool more_than_one = false;
    optional<EKey> result;
    
    // в result первое и единственное значение, иначе там nullopt
    for (auto val : keys) {
        if (val.second) {
            if (!one) {
                one = true;
                result = val.first;
            } else {
                more_than_one = true;
                result = nullopt;
                break;
            }
        }
    }
    
    return make_pair(result, one);
}

ETurnDirection TView::KeyToDirection(EKey key) {
    switch(key) {
        case EKey::KEY_LEFT:
            return ETurnDirection::LEFT;
            break;
            
        case EKey::KEY_RIGHT:
            return ETurnDirection::RIGHT;
            break;
            
        case EKey::KEY_UP:
            return ETurnDirection::UP;
            break;
        
        case EKey::KEY_DOWN:
            return ETurnDirection::DOWN;
            break;
        
        default:
            throw runtime_error("Unknown key");
            break;
    }
}

optional<ETurnDirection> TView::GetTurn() {
    // возвращает ход, если он есть
    
    auto key = GetKeyPressed();
    
    if (!key.second) { // если нет нажатых кнопок, сбрасываем флаг
        turn_flag = false;
        return nullopt;
    } else { // если есть хотя бы одна нажатая кнопка 
        if (key.first and !turn_flag) { // и она одна, и ход может произойти
            auto current_key = *(key.first);
            ETurnDirection result = KeyToDirection(current_key);
            turn_flag = true;
            return make_optional(result);
            
        } else { // если кнопка не одна, или ход уже был
            return nullopt;
        }
    }
}

void TView::DrawField(const TEngine &engine) {
    // отрисовывает поле
    for (int i = 0; i < engine.GetXSize(); i++) {
        for (int j = 0; j < engine.GetYSize(); j++) {
            auto tile = TView::TileToTile(engine(i, j));
            if (tile) {
                display->DrawTile(i, j, *tile);
            }
        }
    }
}

void TView::Render(const TEngine &engine) {
    // отрисовывает поле и рендерит
    DrawField(engine);
    display->Render();
}

int TView::GetLengthOfMotion(const vector<TShiftOfTile> shifts) {
    // возвращает максимальное число тайлов, на которое происходит перемещение (при стандартном поле это целое число от 1 до 3)
    int result = 0;
    for (auto val : shifts) {
        result = max(result, max(abs(val.x_old - val.x_new),
                                 abs(val.y_old - val.y_new)));
    }
    
    return result;
}

float TView::GetTransparency(double elapsed_time, double current_move_time, const TShiftOfTile &shift, const pair<double, double> &coordinates) {
    if (shift.x_old == shift.x_new &&
        shift.y_old == shift.y_new) {
        return 1.0f;
    }
    
    int distance = max(abs(shift.x_old - shift.x_new), abs(shift.y_old - shift.y_new));
    
    double result;
    
    if (shift.x_old == shift.x_new) {
        result = abs((coordinates.second - shift.y_new) / (shift.y_old - shift.y_new));
    } else {
        result = abs((coordinates.first - shift.x_new) / (shift.x_old - shift.x_new));
    } 
    
    return static_cast<float>(result);
}

pair<double, double> TView::GetCoordinates(double elapsed_time, double full_move_time, int length_of_motion, const TShiftOfTile &shift) {
    // возвращает текущие координаты тайла данного сдвига в данный момент
    // не поддерживает диагональные сдвиги
    
    if (shift.x_old == shift.x_new &&
        shift.y_old == shift.y_new) {
        return pair<double, double>(shift.x_new, shift.y_new);
    }
    
    double x, y;
    
    const int length_of_this_motion = max(abs(shift.x_old - shift.x_new),
                                          abs(shift.y_old - shift.y_new)); // длина движения данного тайла
    const double time_of_this_motion = full_move_time * length_of_this_motion / length_of_motion; // сколько длится движение данного тайла
    
    if (elapsed_time >= time_of_this_motion) { //
        return pair<double, double>(shift.x_new, shift.y_new);
    } else {
        if (shift.y_old == shift.y_new) {
            x = shift.x_old + (shift.x_new - shift.x_old) * elapsed_time / time_of_this_motion;
            y = shift.y_new;
        } else {
            y = shift.y_old + (shift.y_new - shift.y_old) * elapsed_time / time_of_this_motion;
            x = shift.x_new;
        }
    }
    
    return pair<double, double>(x, y);
}


void TView::Animate(const vector<TShiftOfTile> shifts, const vector<SNewTile> new_tiles, const TEngine &engine) {
    // Анимирует переданные сдвиги 
    
    const int length_of_motion = GetLengthOfMotion(shifts);
    const double current_move_time = MOVE_TIME * length_of_motion / MAX_MOTION_LENGTH; // время всего движения
    
    
    
    
    double elapsed_time;
    
    const double old_time = display->GetTime();
    
    int i = 0;
    while ((elapsed_time = (display->GetTime() - old_time)) < current_move_time) {
        for (auto const &shift : shifts) {
            pair<double, double> coordinates;
            coordinates = GetCoordinates(elapsed_time, current_move_time, length_of_motion, shift);
            
            float transparency;
            if (shift.unite_flag) {
                transparency = GetTransparency(elapsed_time, current_move_time, shift, coordinates);
            } else {
                transparency = 1.0f;
            }
            
            auto x = coordinates.first;
            auto y = coordinates.second;
            
            display->DrawTile(y, x, *TileToTile(shift.type), transparency);
        }
        
        // на сколько клеток сдвинулся самый "долгий" тайл
        double cells_moved = elapsed_time * length_of_motion / current_move_time;
        
        // прорисовка появляющихся клеток
        for (auto const &new_tile : new_tiles) {
            if (cells_moved + 1.5 > new_tile.cells_to_appear) {
                display->DrawTile(new_tile.y, new_tile.x, *TileToTile(new_tile.type), min(1.0f, static_cast<float>(cells_moved - new_tile.cells_to_appear + 1)));
            }
        }
        
        
        
        //cout << "current_i:" << i++ << endl;
        display->Render();
    }
    
}

void TView::AnimateRandomTile(int x, int y, const TEngine &engine) {
    const double old_time = display->GetTime();
    
    double elapsed_time;
    while ((elapsed_time = (display->GetTime() - old_time)) < RANDOM_TILE_TIME) {
        for (int i = 0; i < engine.GetXSize(); i++) {
            for (int j = 0; j < engine.GetYSize(); j++) {
                if (i == x && j == y) {
                    auto new_tile = engine(i, j);
                    display->DrawTile(y, x, *TileToTile(new_tile), static_cast<float>(elapsed_time / RANDOM_TILE_TIME)); 
                } else {
                    auto old_tile = engine(i, j);
                    if (old_tile != EEngineTileType::TILE_0) {
                        display->DrawTile(y, x, *TileToTile(old_tile));
                    }
                }
            }
        }
        display->Render();
       
    }
}

void TView::WinScreen(const TEngine &engine) {
    DrawField(engine);
    display->DrawWinMessage();
    display->Render();
}

void TView::LoseScreen(const TEngine &engine) {
    DrawField(engine);
    display->DrawLoseMessage(); // TODO сделать lose screen
    display->Render();
}

optional<ETileType> TView::TileToTile(EEngineTileType tile) {
    // переводит тайл движка игры в тайл Display
    int number = static_cast <int> (tile);
    if (number) {
        return make_optional(static_cast <ETileType> (number - 1));
    } else {
        return nullopt;
    }
}

/*void TView::PrintShifts(const std::vector<TShiftOfTile> shifts) {
    cout << "===========" << endl;
    
    for (auto val : shifts) {
        cout << "TILE: " << static_cast<int>(val.type) << endl;
        cout << "Old: " << val.x_old << " " << val.y_old << endl;
        cout << "New: " << val.x_new << " " << val.y_new << endl;
        if (val.unite_flag) {
            cout << "UNITE" << endl;
        }
    }
    
    cout << "===========" << endl;
}*/