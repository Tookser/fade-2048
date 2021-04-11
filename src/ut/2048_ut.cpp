#include <vector>
#include <optional>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <display/display.h>
#include <display/view.h>
#include <engine/engine.h>
#include <motor/motor.h>

using namespace std;

using ::testing::_;
using ::testing::AtLeast;
using ::testing::NiceMock;

enum {MAX_FIELD_SIZE = 100};

const int MINIMAL_FRAME_PER_MOTION = 10; // столько должно быть отрисовок анимации минимум за максимально длинное перемещение

auto t0 = EEngineTileType::TILE_0;
auto t2 = EEngineTileType::TILE_2;
auto t4 = EEngineTileType::TILE_4;
auto t8 = EEngineTileType::TILE_8;
auto t16 = EEngineTileType::TILE_16;
auto t32 = EEngineTileType::TILE_32;
auto t64 = EEngineTileType::TILE_64;
auto t128 = EEngineTileType::TILE_128;
auto t256 = EEngineTileType::TILE_256;
auto t512 = EEngineTileType::TILE_512;
auto t1024 = EEngineTileType::TILE_1024;
auto t2048 = EEngineTileType::TILE_2048;

bool IsEqual(const TEngine &engine, vector<vector<EEngineTileType>> v) {
    // сравнение двумерного вектора и TEngine
    bool result = true;
    for (size_t i; i < engine.GetXSize(); i++) {
        for (size_t j; j < engine.GetXSize(); j++) {
            if (engine(i,j) != v[i][j]) {
                result = false;
                break;
            }
        }
        
        if (!result) {
            break;
        }
    }
    
    return result;
}

class MockDisplay : public TDisplay {
    public:
        MOCK_METHOD(void, DrawTile, (float, float, ETileType, float), (override));
        MOCK_METHOD(void, DrawWinMessage, (), (override));
        MOCK_METHOD(void, DrawLoseMessage, (), (override));
        //MOCK_METHOD(optional<ETurnDirection>, GetTurn, (), (override));
        //MOCK_METHOD(double, GetTime, (), (override, const));
};

/*
class Testcl {
    public:
        Testcl() {};
        virtual ~Testcl() {};
    
        virtual int Hello(int a) { return 42; };
};

class MockTestcl : public Testcl {
    public:
        MOCK_METHOD1(Hello, int(int));
};*/


TEST(EngineTest, EmptyField) {
    vector<vector<EEngineTileType>> field = 
                        {   {t0, t0, t0, t0},
                            {t0, t0, t0, t0},
                            {t0, t0, t0, t0},
                            {t0, t0, t0, t0}    };
    
    TEngine engine(field);
    
    
    EXPECT_TRUE(IsEqual(engine, field)) << "Empty tile is not empty on clear field";
        
}

TEST(EngineTest, Size) {
    TEngine engine;
    
    int x_size = engine.GetXSize();
    int y_size = engine.GetYSize();
    EXPECT_LE(x_size, MAX_FIELD_SIZE);
    EXPECT_GT(x_size, 0);
    EXPECT_LE(y_size, MAX_FIELD_SIZE);
    EXPECT_GT(y_size, 0);
    
}

TEST(EngineTest, FieldCorrectness) {
    
    vector<vector<EEngineTileType>> field(SIZE_OF_FIELD_X, vector<EEngineTileType>(SIZE_OF_FIELD_Y, EEngineTileType::TILE_128));
    field[0][0] = EEngineTileType::TILE_0;
    
    
    TEngine engine(field);
    
    for (int i = 0; i < engine.GetXSize(); i++) {
        for (int j = 0; j < engine.GetYSize(); j++) {
            if (!(i == 0 && j == 0)) {
                EXPECT_EQ(engine(i, j), EEngineTileType::TILE_128);
            } else if (i == 0 && j == 0) {
                EXPECT_EQ(engine(i, j), EEngineTileType::TILE_0);
            }
        }
    }
}

TEST(EngineTest, SimpleTurn) {
    vector<vector<EEngineTileType>> field = 
                        {   {t0, t0, t2, t2},
                            {t0, t4, t0, t0},
                            {t0, t4, t2, t8},
                            {t2, t0, t0, t0}    };
                            
    TEngine engine(field);
    
    auto result = engine.MakeTurn(ETurnDirection::LEFT);
    
    vector<vector<EEngineTileType>> result_field = 
                        {   {t4, t0, t0, t0},
                            {t4, t0, t0, t0},
                            {t4, t2, t8, t0},
                            {t2, t0, t0, t0}    };
            
    
    EXPECT_TRUE(IsEqual(engine, result_field)) << "Simple move failed";
    
    
}

TEST(EngineTest, ThreeTilesTurn) {
    vector<vector<EEngineTileType>> field = 
                        {   {t2, t2, t2, t0},
                            {t0, t0, t0, t0},
                            {t0, t2, t2, t2},
                            {t2, t0, t0, t0}    };
                            
    TEngine engine(field);
    
    auto result = engine.MakeTurn(ETurnDirection::LEFT);
    
    vector<vector<EEngineTileType>> result_field = 
                        {   {t4, t2, t0, t0},
                            {t0, t0, t0, t0},
                            {t4, t2, t0, t0},
                            {t2, t0, t0, t0}    };
            
    
    EXPECT_TRUE(IsEqual(engine, result_field)) << "2220< or 0222< move failed";
    
    
}

TEST(EngineTest, RandomTile) {
    vector<vector<EEngineTileType>> field = 
                        {   {t2, t0, t0, t0},
                            {t4, t0, t0, t0},
                            {t4, t0, t0, t0},
                            {t8, t0, t0, t0}    };
    
    TEngine engine(field);
    
    engine.MakeTurn(ETurnDirection::LEFT);
    engine.AfterTurn();
    
    bool encounter = false;
    bool double_encounter = false;
    
    bool correct = false;
   
    for (int i = 0; i < engine.GetXSize(); i++) {
        for (int j = 0; j < engine.GetYSize(); j++) {
            auto cell = engine(i, j);
            
            if (cell != field[i][j]) {
                if (encounter) {
                    double_encounter = true;
                    break;
                } else {
                    encounter = true;
                    if (cell == t2 || cell == t4) {
                        correct = true;
                    }
                }
            }
        }
    }
    
    EXPECT_TRUE(encounter) << "There is no random tile";
    EXPECT_TRUE(correct) << "There is tile !2 && !4";
    EXPECT_FALSE(double_encounter) << "There are more than one random tiles";
}

TEST(EngineTest, Lose) {
    vector<vector<EEngineTileType>> field = 
                        {   {t32, t16, t32, t0},
                            {t16, t32, t16, t32},
                            {t32, t16, t32, t16},
                            {t16, t32, t16, t32}    };
    
    TEngine engine(field);
    
    engine.MakeTurn(ETurnDirection::LEFT);
    engine.AfterTurn();
    
    EXPECT_TRUE(engine.IsLose());
    EXPECT_FALSE(engine.IsWin());
    EXPECT_TRUE(engine.IsEnd());
    
}

TEST (EngineTest, Win) {    
    vector<vector<EEngineTileType>> field = 
                        {   {t0, t2048, t0, t0},
                            {t0, t0, t0, t0},
                            {t0, t0, t0, t0},
                            {t0, t0, t0, t0}    };
                       
    TEngine engine(field);
    
    EXPECT_TRUE(engine.IsWin());
    EXPECT_FALSE(engine.IsLose());
    EXPECT_TRUE(engine.IsEnd());
}

TEST (EngineTest, MiddleGame) {
    
    vector<vector<EEngineTileType>> field = 
                        {   {t2, t1024, t0, t0},
                            {t4, t0, t4, t0},
                            {t4, t8, t2, t0},
                            {t8, t2, t4, t0}    };
                            
    TEngine engine(field);
    
    engine.MakeTurn(ETurnDirection::LEFT);
    engine.AfterTurn();
    
    
    EXPECT_FALSE(engine.IsWin());
    EXPECT_FALSE(engine.IsLose());
    EXPECT_FALSE(engine.IsEnd());
}

TEST (EngineTest, ExceptionAfterEnd) {
    vector<vector<EEngineTileType>> field = 
                        {   {t2, t2048, t0, t0},
                            {t4, t0, t4, t0},
                            {t4, t8, t2, t0},
                            {t8, t2, t4, t0}    };

    TEngine engine(field);
    
    EXPECT_THROW(engine.MakeTurn(ETurnDirection::LEFT), runtime_error) << "Didn't throw exceptions when WIN state tried to move";
    
    vector<vector<EEngineTileType>> field2 = 
                        {   {t2, t4, t2, t4},
                            {t4, t2, t4, t2},
                            {t2, t4, t2, t4},
                            {t4, t2, t4, t2}    };
    
    
    TEngine engine2(field);
    
    EXPECT_THROW(engine.MakeTurn(ETurnDirection::LEFT), runtime_error) << "Didn't throw exceptions when LOSE state tried to move";
    
}


class ViewTest : public ::testing::Test {
    public:
        ViewTest()
            : field({   {t2, t0, t0, t0},
                        {t4, t0, t0, t0},
                        {t0, t0, t0, t0},
                        {t0, t8, t0, t0}    })
            , field2({   {t2048, t1024, t512, t256},
                            {t128, t64, t32, t16},
                            {t8, t4, t2, t0},
                            {t0, t0, t0, t0}    })
            , engine(field)
            , engine2(field2)
            , view(&mockdisplay) {}
            
    protected:
        void SetUp() override {
        }
        
        //void TearDown() override {}
        
        NiceMock<MockDisplay> mockdisplay;
        
        vector<vector<EEngineTileType>> field;
        vector<vector<EEngineTileType>> field2;    
        TEngine engine;
        TEngine engine2;
        TView view;
    
};


TEST_F (ViewTest, BasicTest) {
    EXPECT_CALL(mockdisplay, DrawTile(0, 0, ETileType::TILE_2, 1.0f));
    EXPECT_CALL(mockdisplay, DrawTile(1, 0, ETileType::TILE_4, 1.0f));
    EXPECT_CALL(mockdisplay, DrawTile(3, 1, ETileType::TILE_8, 1.0f));
    
    
    view.Render(engine);
    
    EXPECT_CALL(mockdisplay, DrawTile)
    .Times(11);
    
    view.Render(engine2);
    
}


TEST_F (ViewTest, WinLoseScreens) {
    EXPECT_CALL(mockdisplay, DrawWinMessage);
    view.WinScreen(engine);
    
    TView view2(&mockdisplay);
    EXPECT_CALL(mockdisplay, DrawLoseMessage);
    view.LoseScreen(engine);
}

TEST (ViewTestNotBasic, Animation) {
    NiceMock<MockDisplay> mockdisplay;
    
    EXPECT_CALL(mockdisplay, DrawTile)
    .Times(AtLeast(MINIMAL_FRAME_PER_MOTION));
    
    vector<vector<EEngineTileType>> field = 
                        {   {t0, t0, t0, t2},
                            {t0, t0, t0, t0},
                            {t0, t0, t0, t0},
                            {t0, t0, t0, t0}    };
                            
    TEngine engine(field);
    
    auto result = engine.MakeTurn(ETurnDirection::LEFT);
    
    TView view (&mockdisplay);
    
    
    
    ASSERT_TRUE(result);
    
    engine.AfterTurn();
    view.Animate((*result).first, (*result).second, engine);

}