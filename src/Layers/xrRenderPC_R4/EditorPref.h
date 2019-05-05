#pragma once

enum
{
    epoDrawPivot = (1 << 0),
    epoDrawAnimPath = (1 << 1),
    epoDrawJoints = (1 << 2),
    epoDrawBoneAxis = (1 << 3),
    epoDrawBoneNames = (1 << 4),
    epoDrawBoneShapes = (1 << 5),
    epoShowHint = (1 << 6),
    epoDrawLOD = (1 << 7),
    epoDiscardInstance = (1 << 8),
    epoDeffLoadRB = (1 << 9),
    epoDeffLoadCF = (1 << 10),
    epoSelectInGroup = (1 << 11),
};

enum ETAction
{
    etaSelect = 0,
    etaAdd,
    etaMove,
    etaRotate,
    etaScale,
    etaMaxActions
};

enum ETAxis
{
    etAxisY,
    etAxisX,
    etAxisZ,
    etAxisZX,
    etAxisUndefined,
};

enum ETFlags
{
    etfCSParent = (1 << 0),
    etfNUScale = (1 << 1),
    etfNormalAlign = (1 << 2),
    etfGSnap = (1 << 3),
    etfOSnap = (1 << 4),
    etfMTSnap = (1 << 5),
    etfVSnap = (1 << 6),
    etfASnap = (1 << 7),
    etfMSnap = (1 << 8),
};

class EditorPref // Lord: добавлять данный класс, подумать, что можно было бы сделать
{
private:
    EditorPref(void)
    {
        this->object_flags.zero();
        this->Settings_flags.zero();
        this->bDrawAnimationPath = false;
        this->snap_angle = 0.0f;
        this->snap_move = 0.0f;
        this->snap_moveto = 0.0f;
        this->bDrawGrid = true;
    }

public:
    inline static EditorPref& GetInstance(void)
    {
        static EditorPref instance;
        return instance;
    }

    void OnClose(void);
    void ApplyValues(void);
    void Edit(void);
    void OnCreate(void);
    void OnDestroy(void);
    ~EditorPref(void);

public:
    bool bDrawAnimationPath;
    bool bDrawGrid;
    Flags32 object_flags;
    Flags32 Settings_flags;
    float snap_angle;
    float snap_move;
    float snap_moveto;
};
