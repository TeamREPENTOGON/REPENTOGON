ffi.cdef [[
    struct GenericPrompt {
        private struct Sprite Sprite : 0x0;
        private int CurrentSelection : 0x128;
        private int SubmittedSelection : 0x12c;
    } : 0x134;
    typedef struct GenericPrompt* GenericPromptPtr;

    void L_GenericPrompt_Init(struct GenericPrompt*);
    void L_GenericPrompt_Destroy(struct GenericPrompt*);
    void L_GenericPrompt_Initialize(struct GenericPrompt*, bool);
    void L_GenericPrompt_Show(struct GenericPrompt*);
    bool L_GenericPrompt_IsActive(struct GenericPrompt*);
    void L_GenericPrompt_SetImageToVictoryRun(struct GenericPrompt*);
    void L_GenericPrompt_Update(struct GenericPrompt*, bool);
    void L_GenericPrompt_Render(struct GenericPrompt*);
    void L_GenericPrompt_SetText(struct GenericPrompt*, const char*, const char*, const char*, const char*, const char*);
]]

local repentogon = ffidll
local ffi = ffi

local GenericPromptMT
GenericPromptMT = {
    __type = "GenericPrompt",
    __gc = function(self)
        repentogon.L_GenericPrompt_Destroy(self)
    end,
    GetCurrentSelection = function(self)
        return ffi.getprivate(self, "CurrentSelection")
    end,
    GetSprite = function(self)
        return ffi.getprivate(self, "Sprite")
    end,
    GetSubmittedSelection = function(self)
        return ffi.getprivate(self, "SubmittedSelection")
    end,
    Initialize = function(self, smallPrompt)
        smallPrompt = ffichecks.optboolean(smallPrompt, false)
        repentogon.L_GenericPrompt_Initialize(self, smallPrompt)
    end,
    IsActive = function(self)
        return repentogon.L_GenericPrompt_IsActive(self)
    end,
    Render = function(self)
        repentogon.L_GenericPrompt_Render(self)
    end,
    SetImageToVictoryRun = function(self)
        repentogon.L_GenericPrompt_SetImageToVictoryRun(self)
    end,
    SetText = function(self, text1, text2, text3, text4, text5)
        repentogon.L_GenericPrompt_SetText(self,
            ffichecks.optstring(text1, ""), ffichecks.optstring(text2, ""), ffichecks.optstring(text3, ""),
            ffichecks.optstring(text4, ""), ffichecks.optstring(text5, ""))
    end,
    Show = function(self)
        repentogon.L_GenericPrompt_Show(self)
    end,
    Update = function(self, processInput)
        repentogon.L_GenericPrompt_Update(self, ffichecks.optboolean(processInput, false))
    end,
}

setmetatable(GenericPromptMT, { __index = function() end })
GenericPromptMT.__index = GenericPromptMT

local GenericPromptT = ffi.metatype("struct GenericPrompt", GenericPromptMT)

GenericPrompt = setmetatable({}, {
    __call = function(_)
        local prompt = GenericPromptT()
        repentogon.L_GenericPrompt_Init(prompt)
        return prompt
    end,
    __class = GenericPromptMT,
})
