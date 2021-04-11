#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_set>

#include <stdexcept>
#include <assert.h>

#include <cstdlib>
#include <ctime>

#include "engine.h"

using namespace std;


void TEngine::AddTile(int x, int y, EEngineTileType tile) {
    if (state[x][y] == EEngineTileType::TILE_0) {
        state[x][y] = tile;
    } else {
        throw runtime_error("Tried to add tile on the tile");
    }
}

pair<int, int> TEngine::AddRandomTile(bool only_2 = false) {
    vector<pair<int, int>> free_cells;
    
    for (int i = 0; i < GetXSize(); i++) {
        for (int j = 0; j < GetYSize(); j++) {
            if (state[i][j] == EEngineTileType::TILE_0) {
                free_cells.push_back(make_pair(i, j));
            }
        }
    }
    
    
    if (free_cells.size() == 0) {
        throw runtime_error("can't add new tile");
    } else {
        int random_number = rand() % free_cells.size();
        
        auto random_cell = free_cells[random_number];
        
        int x = random_cell.first;
        int y = random_cell.second;
        
        if (!only_2 && !(rand() % 10)) { // с вероятностью 10% тайл 4
            AddTile(x, y, EEngineTileType::TILE_4);
        } else {
            AddTile(x, y, EEngineTileType::TILE_2);
        }
        return make_pair(x, y);
    }
    
}

EEngineTileType TEngine::GetDoubleTile(EEngineTileType tile) {
    // возвращает удвоенный тайл
    return static_cast <EEngineTileType> (static_cast <int> (tile) + 1);
}

EEngineTileType TEngine::GetWinTile() {
    // выигрышный тайл
    //return EEngineTileType::TILE_32;
    return EEngineTileType::TILE_2048;
}

vector<EEngineTileType> TEngine::GetLine(bool vertical, int line_number) const{
    vector<EEngineTileType> result;
    if (vertical) {
        for (int i = 0; i < SIZE_OF_FIELD_Y; i++) {
            result.push_back(state[i][line_number]);
        }
    } else {
        for (int i = 0; i < SIZE_OF_FIELD_X; i++) {
            result.push_back(state[line_number][i]);
        }
    }
    
    return result;
}

void TEngine::WriteLine(bool vertical, int line_number, const vector<EEngineTileType> &new_line) {
    if (vertical) {
        assert(new_line.size() == SIZE_OF_FIELD_Y);
    } else {
        assert(new_line.size() == SIZE_OF_FIELD_X);
    }
    
    for (int i = 0; i < new_line.size(); i++) {
        if (vertical) {
            state[i][line_number] = new_line[i];
        } else {
            state[line_number][i] = new_line[i];
        }
    }
}


void TEngine::Transpose(TShiftOfTile &s) {
    swap(s.x_old, s.y_old);
    swap(s.x_new, s.y_new);
}

bool TEngine::MakeTurnLine(bool vertical, int line_number, ETurnDirection turn, vector<TShiftOfTile> &shifts, vector<SNewTile> &appear_tiles) {
    // делает ход на одной линии
    // return true если что-то изменилось
    // shifts - вектор сдвигов, какой тайл в какую позицию
    
    const int line_size = vertical ? SIZE_OF_FIELD_Y : SIZE_OF_FIELD_X;
    
    const bool reverse_flag = !((turn == ETurnDirection::LEFT) || (turn == ETurnDirection::UP));
    
    vector<EEngineTileType> current_line = GetLine(vertical, line_number), new_line;

    if (reverse_flag) {
        reverse(begin(current_line), end(current_line));
    }
    
    auto previous = EEngineTileType::TILE_0; // последний встреченный ненулевой тайл
    int previous_position = -1; // его позиция
        
    bool is_tile_before = false;
    
    bool changed = false; // true если произошли изменения
    
    bool after_free_space = false; // true если после пустых клеток
    
    int start_i;
    
    for (int i = 0; i < current_line.size(); i++) {
        auto val = current_line[i];
        
        if (val == EEngineTileType::TILE_0) {
            after_free_space = true;
            continue;
        } else {
            if (after_free_space) {
                changed = true; // изменения произошли если встретился ненулевой за нулевым
                after_free_space = false;
            }
            if (val == previous) { // если объединяется с предыдущим
                new_line.push_back(GetDoubleTile(previous));
                previous = EEngineTileType::TILE_0;
                is_tile_before = false;
                changed = true;
                
                int new_coordinate = new_line.size() - 1;
                
                TShiftOfTile t1, t2; // t1 - предыдущий тайл, t2 последний
                // по умолчанию по горизонтали, если по вертикали, меняет
                t1.y_old = t1.y_new = t2.y_old = t2.y_new = line_number;
                t1.x_old = previous_position;
                t1.x_new = new_coordinate;
                
                t2.x_old = i;
                t2.x_new = new_coordinate;
                
                t1.type = t2.type = val;
                t1.unite_flag = t2.unite_flag = true;
                
                int cells_to_appear = max(abs(t1.x_old - t1.x_new), abs(t2.x_old - t2.x_new)); // сколько клеток должно пройти, чтобы закончилось движение
                
                if (reverse_flag) {
                    t1.x_old = line_size - t1.x_old - 1;
                    t2.x_old = line_size - t2.x_old - 1;
                    t1.x_new = line_size - t1.x_new - 1;
                    t2.x_new = line_size - t2.x_new - 1;
                }
                
                if (vertical) {
                    Transpose(t1);
                    Transpose(t2);
                }
                
                shifts.push_back(t1);
                shifts.push_back(t2);
                
                
                SNewTile new_tile({t2.x_new, t2.y_new, GetDoubleTile(val), cells_to_appear});
                
                appear_tiles.push_back(new_tile);
                
            } else { // если не объединяется с предыдущим
                if (is_tile_before) {
                    new_line.push_back(previous);

                    int new_coordinate = new_line.size() - 1;
                    
                    TShiftOfTile t; 
                    
                    t.y_old = t.y_new = line_number;
                    t.x_old = previous_position;
                    t.x_new = new_coordinate;
                    
                    t.type = previous;
                    //t.unite_flag = true;
                    
                    if (reverse_flag) {
                        t.x_old = line_size - t.x_old - 1;
                        t.x_new = line_size - t.x_new - 1;
                    }
                    
                    if (vertical) {
                        Transpose(t);
                    }
                    
                    shifts.push_back(t);
                }
                
                previous = val;
                previous_position = i;
                
                is_tile_before = true;
            }
        }
    }
    
    if (previous != EEngineTileType::TILE_0) {
        new_line.push_back(previous);
        
        int new_coordinate = new_line.size() - 1;
        
        TShiftOfTile t; // t1 - предыдущий тайл, t2 последний
                    // по умолчанию по горизонтали, если по вертикали, меняет
        t.y_old = t.y_new = line_number;
        t.x_old = previous_position;
        t.x_new = new_coordinate;
        
        t.type = previous;
        
        if (reverse_flag) {
            t.x_old = line_size - t.x_old - 1;
            t.x_new = line_size - t.x_new - 1;
        }
        
        if (vertical) {
            Transpose(t);
        }
        
        shifts.push_back(t);
    }
    
    int need_to_add = line_size - new_line.size();
    for (int i = 0; i < need_to_add; i++) {
        new_line.push_back(EEngineTileType::TILE_0);
    }
    
    if (reverse_flag) {
        reverse(begin(new_line), end(new_line));
    }
    
    WriteLine(vertical, line_number, new_line);
    
    return changed;
}

TEngine::TEngine()
        : state(vector<vector<EEngineTileType>> (SIZE_OF_FIELD_X, vector<EEngineTileType>(SIZE_OF_FIELD_Y, EEngineTileType::TILE_0)))
        , win_flag(false)
        , lose_flag(false) {

    srand(time(NULL));
    
    InitializeField();
    
    RefreshWinLoseState();
}

TEngine::TEngine(const vector<vector<EEngineTileType>> &field)
        : state(vector<vector<EEngineTileType>> (SIZE_OF_FIELD_X, vector<EEngineTileType>(SIZE_OF_FIELD_Y, EEngineTileType::TILE_0)))
        , win_flag(false)
        , lose_flag(false) {
    // конструктор произвольной конфигурации поля
    
    assert(field.size() == GetXSize());
    for (auto val : field) {
        assert(val.size() == GetYSize());
    }
    
    for (size_t i = 0; i < field.size(); i++) {
        for (size_t j = 0; j < field[i].size(); j++) {
            state[i][j] = field[i][j];
        }
    }
    
    RefreshWinLoseState();
}


void TEngine::InitializeField() {
    // инициализирует поле
    /*state[2][3] = EEngineTileType::TILE_32;
    state[0][0] = EEngineTileType::TILE_2;
    state[0][1] = EEngineTileType::TILE_4;
    state[0][3] = EEngineTileType::TILE_8;*/
    /*state[0][1] = EEngineTileType::TILE_8;
    state[0][3] = EEngineTileType::TILE_8;*/
    /*state[0][0] = EEngineTileType::TILE_2;
    state[0][1] = EEngineTileType::TILE_2;
    state[0][2] = EEngineTileType::TILE_2;
    state[0][3] = EEngineTileType::TILE_2;*/
    
    
    for (int i = 0; i < TILES_AT_START; i++) {
        AddRandomTile(true); // добавляем только двойки на старте 
    }
}

bool TEngine::IsEnd() const {
    // произошёл ли конец игры
    return win_flag || lose_flag;
}

bool TEngine::IsWin() const {
    return win_flag;
}

bool TEngine::IsLose() const {
    return lose_flag;
}

void TEngine::RefreshWinLoseState() {
    // если появился выигрышный тайл, выигрыш независимо от возможности хода
    bool has_free_space = false;
    for (int i = 0; i < GetXSize(); i++) {
        for (int j = 0; j < GetYSize(); j++) {
            auto tile = (*this)(i, j);
            if (tile == GetWinTile()) {
                win_flag = true;
            }
            
            if (tile == EEngineTileType::TILE_0) {
                has_free_space = true;
            }
        }
    }
    
    if (!win_flag && !has_free_space) { // если есть вероятность проигрыша
        lose_flag = true;
        
        for (int i = 1; i < GetXSize() - 1; i++) {
            for (int j = 1; j < GetYSize() - 1; j++) {
                // проверяем, можно ли успешно сдвинуть
                auto tile = (*this)(i, j);
                auto t1 = (*this)(i + 1, j);
                auto t2 = (*this)(i, j + 1);
                auto t3 = (*this)(i - 1, j);
                auto t4 = (*this)(i, j - 1);
                
                if (tile == t1 || tile == t2 ||
                    tile == t3 || tile == t4) {
                        lose_flag = false;
                        break;
                }
            }
            
            if (!lose_flag) {
                break;
            }
        }
    }
}

optional<pair<vector<TShiftOfTile>, vector<SNewTile>>> TEngine::MakeTurn(ETurnDirection turn) {
    // делает один ход, без создания новых тайлов
    if (!IsEnd()) {
        bool vertical = (turn == ETurnDirection::UP) || (turn == ETurnDirection::DOWN);
        
        int lim;
        if (vertical) {
            lim = SIZE_OF_FIELD_X; // количество столбцов
        } else {
            lim = SIZE_OF_FIELD_Y; // количество строк 
        }
        
        bool result = false;
        
        vector<TShiftOfTile> shifts;
        vector<SNewTile> new_tiles;
        
        for (int i = 0; i < lim; i++) {
            result = MakeTurnLine(vertical, i, turn, shifts, new_tiles) or result;
        }    
        
        if (result) {
            return make_optional(make_pair(shifts, new_tiles));
        } else {
            return nullopt;
        }
    } else {
        throw runtime_error("Tried to move when game is finished");
    }
}

pair<int, int> TEngine::AfterTurn() {
    // после перемещения - добавляет новый тайл и обновляет состояние
    auto result = AddRandomTile();
    RefreshWinLoseState();
    return result;
}

EEngineTileType TEngine::operator()(int x, int y) const {
    assert(x >= 0 && x <= SIZE_OF_FIELD_X);
    assert(y >= 0 && y <= SIZE_OF_FIELD_Y);
    
    return state[x][y];
}

int TEngine::GetXSize() const {
    return SIZE_OF_FIELD_X;
}

int TEngine::GetYSize() const {
    return SIZE_OF_FIELD_Y;
}