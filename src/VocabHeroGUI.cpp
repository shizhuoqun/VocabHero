// 英语勇者 · 单词讨伐战 —— Win32 图形界面版
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <commctrl.h>
#include <algorithm>
#include <array>
#include <random>
#include <set>
#include <string>
#include <vector>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// ===================== 题库结构 =====================
struct WordEntry {
    std::string word;
    std::string phonetic;
    std::string pos;
    std::string meaning;
    std::vector<std::string> distractors;
};

static const std::vector<WordEntry> CET4_WORDS = {
    {"abandon", "/əˈbændən/", "v.", "放弃；抛弃", {"获得；吸收", "冒险；尝试", "维持；坚持"}},
    {"ability", "/əˈbɪləti/", "n.", "能力；才能", {"障碍；阻碍", "态度；看法", "习惯；习俗"}},
    {"absorb", "/əbˈzɔːrb/", "v.", "吸收；使全神贯注", {"释放；排出", "分散；转移", "拒绝；驳回"}},
    {"academic", "/ˌækəˈdemɪk/", "adj.", "学术的；学院的", {"商业的；贸易的", "身体的；体力的", "艺术的；美学的"}},
    {"accompany", "/əˈkʌmpəni/", "v.", "陪伴；伴随", {"隔离；孤立", "反对；抵抗", "忽略；忽视"}},
    {"accomplish", "/əˈkɑːmplɪʃ/", "v.", "完成；实现", {"开始；着手", "放弃；停止", "破坏；损坏"}},
    {"account", "/əˈkaʊnt/", "n./v.", "账户；叙述；解释", {"折扣；减价", "收据；凭证", "预算；开支"}},
    {"accumulate", "/əˈkjuːmjuleɪt/", "v.", "积累；积聚", {"消耗；耗尽", "分散；散播", "减少；削减"}},
    {"accurate", "/ˈækjərət/", "adj.", "准确的；精确的", {"模糊的；含糊的", "粗略的；大概的", "错误的；不正确的"}},
    {"achieve", "/əˈtʃiːv/", "v.", "达到；取得", {"丧失；失去", "逃避；回避", "推迟；延期"}},
    {"acquire", "/əˈkwaɪər/", "v.", "获得；学到", {"失去；丢失", "转让；让出", "隐藏；掩盖"}},
    {"adapt", "/əˈdæpt/", "v.", "适应；改编", {"保持；维持", "拒绝；排斥", "复制；模仿"}},
    {"adequate", "/ˈædɪkwət/", "adj.", "足够的；适当的", {"缺乏的；不足的", "过度的；多余的", "不稳定的；易变的"}},
    {"adjust", "/əˈdʒʌst/", "v.", "调整；校准", {"固定；绑定", "破坏；打乱", "确认；批准"}},
    {"admit", "/ədˈmɪt/", "v.", "承认；准许进入", {"否认；拒绝承认", "排除；逐出", "怀疑；质疑"}},
    {"adopt", "/əˈdɑːpt/", "v.", "采用；收养", {"抛弃；遗弃", "拒绝；否决", "借用；暂用"}},
    {"advance", "/ədˈvæns/", "v./n.", "前进；进步", {"后退；撤退", "停滞；停顿", "倒退；退化"}},
    {"advantage", "/ədˈvæntɪdʒ/", "n.", "优势；优点", {"劣势；缺点", "障碍；阻力", "损失；亏欠"}},
    {"affect", "/əˈfekt/", "v.", "影响；感动", {"保护；庇护", "忽视；不理", "计算；估算"}},
    {"afford", "/əˈfɔːrd/", "v.", "负担得起；提供", {"买不起；负担不起", "拒绝给予", "浪费；挥霍"}},
    {"aggressive", "/əˈɡresɪv/", "adj.", "好斗的；有闯劲的", {"温和的；温柔的", "被动的；消极的", "谨慎的；小心的"}},
    {"alternative", "/ɔːlˈtɜːrnətɪv/", "n./adj.", "替代方案；可供选择的", {"必然选择；唯一方案", "附加条件；补充条款", "固定模式；传统做法"}},
    {"analyze", "/ˈænəlaɪz/", "v.", "分析；解析", {"综合；归纳", "忽略；略过", "猜测；推测"}},
    {"announce", "/əˈnaʊns/", "v.", "宣布；通告", {"隐瞒；保密", "询问；打听", "承诺；保证"}},
    {"annual", "/ˈænjuəl/", "adj.", "每年的；年度的", {"每月的；月度的", "每周的；周度的", "临时的；一次性的"}},
    {"anxious", "/ˈæŋkʃəs/", "adj.", "焦虑的；渴望的", {"平静的；镇定的", "冷漠的；漠不关心的", "满足的；满意的"}},
    {"apparent", "/əˈpærənt/", "adj.", "明显的；表面的", {"隐蔽的；隐藏的", "模糊的；难以察觉的", "内在的；本质的"}},
    {"appeal", "/əˈpiːl/", "v./n.", "呼吁；吸引力；上诉", {"命令；指示", "拒绝；驳回", "指责；谴责"}},
    {"appear", "/əˈpɪr/", "v.", "出现；显得", {"消失；不见", "隐藏；躲藏", "变成；转变为"}},
    {"apply", "/əˈplaɪ/", "v.", "申请；应用；涂抹", {"拒绝；驳回", "撤回；收回", "分配；分派"}},
    {"appreciate", "/əˈpriːʃieɪt/", "v.", "欣赏；感激；理解", {"贬低；轻视", "抱怨；不满", "误解；曲解"}},
    {"approach", "/əˈproʊtʃ/", "n./v.", "方法；接近", {"远离；离开", "障碍；阻碍", "结果；后果"}},
    {"appropriate", "/əˈproʊpriət/", "adj.", "适当的；合适的", {"不合适的；不恰当的", "过时的；陈旧的", "随意的；随便的"}},
    {"approve", "/əˈpruːv/", "v.", "批准；赞成", {"否决；反对", "怀疑；质疑", "拖延；搁置"}},
    {"arrange", "/əˈreɪndʒ/", "v.", "安排；整理", {"打乱；弄乱", "取消；撤销", "忽略；遗漏"}},
    {"artificial", "/ˌɑːrtɪˈfɪʃl/", "adj.", "人造的；虚假的", {"天然的；自然形成的", "真实的；真诚的", "手工制作的"}},
    {"assess", "/əˈses/", "v.", "评估；评定", {"忽略；忽视", "猜测；臆断", "记录；记载"}},
    {"assume", "/əˈsuːm/", "v.", "假设；认为；承担", {"证实；确认", "否认；否定", "放弃；推卸"}},
    {"attach", "/əˈtætʃ/", "v.", "附上；系上；重视", {"分离；拆下", "隐藏；藏起", "削弱；减弱"}},
    {"attempt", "/əˈtempt/", "n./v.", "尝试；企图", {"放弃；停止", "完成；达成", "逃避；回避"}},
    {"attitude", "/ˈætɪtuːd/", "n.", "态度；看法", {"能力；才能", "习惯；行为", "位置；方位"}},
    {"attract", "/əˈtrækt/", "v.", "吸引；引起", {"排斥；推开", "分散；转移", "拒绝；驳回"}},
    {"available", "/əˈveɪləbl/", "adj.", "可获得的；有空的", {"无法获得的；不可用的", "被占用的；忙碌的", "隐藏的；秘密的"}},
    {"avoid", "/əˈvɔɪd/", "v.", "避免；躲开", {"面对；正视", "追求；寻求", "遭遇；遇到"}},
    {"aware", "/əˈweər/", "adj.", "意识到的；知道的", {"不知道的；未察觉的", "忽略的；疏忽的", "确定的；确信的"}},
    {"basic", "/ˈbeɪsɪk/", "adj.", "基本的；基础的", {"高级的；复杂的", "额外的；附加的", "次要的；辅助的"}},
    {"behave", "/bɪˈheɪv/", "v.", "表现；举止", {"隐藏；躲藏", "改变；转变", "观察；观看"}},
    {"believe", "/bɪˈliːv/", "v.", "相信；认为", {"怀疑；不信", "否认；拒绝", "想象；幻想"}},
    {"benefit", "/ˈbenɪfɪt/", "n./v.", "益处；受益", {"损害；伤害", "损失；亏损", "负担；累赘"}},
    {"brief", "/briːf/", "adj.", "简短的；简要的", {"冗长的；啰嗦的", "详细的；详尽的", "复杂的；繁琐的"}}
};

static const std::vector<WordEntry> CET6_WORDS = {
    {"abolish", "/əˈbɑːlɪʃ/", "v.", "废除；废止", {"建立；设立", "保留；维持", "修订；修改"}},
    {"absurd", "/əbˈsɜːrd/", "adj.", "荒谬的；荒唐的", {"合理的；恰当的", "正常的；普通的", "严肃的；庄重的"}},
    {"abundant", "/əˈbʌndənt/", "adj.", "丰富的；充裕的", {"稀缺的；不足的", "微小的；少量的", "贫瘠的；贫乏的"}},
    {"accelerate", "/əkˈseləreɪt/", "v.", "加速；促进", {"减速；放慢", "阻碍；抑制", "停止；中止"}},
    {"accommodate", "/əˈkɑːmədeɪt/", "v.", "容纳；适应；提供住宿", {"排除；拒绝接纳", "压缩；收缩", "破坏；损害"}},
    {"acknowledge", "/əkˈnɑːlɪdʒ/", "v.", "承认；致谢；告知收到", {"否认；不承认", "忽略；不理", "拒绝；驳回"}},
    {"acute", "/əˈkjuːt/", "adj.", "敏锐的；急性的；尖锐的", {"迟钝的；不敏感的", "慢性的；长期的", "平和的；温和的"}},
    {"adhere", "/ədˈhɪr/", "v.", "坚持；粘附；遵守", {"放弃；背离", "分离；脱落", "反对；抗拒"}},
    {"adjacent", "/əˈdʒeɪsnt/", "adj.", "相邻的；毗连的", {"遥远的；偏远的", "分离的；不相连的", "对面的；相反的"}},
    {"administer", "/ədˈmɪnɪstər/", "v.", "管理；施行；给予", {"放任；不管", "逃避；规避", "破坏；扰乱"}},
    {"advocate", "/ˈædvəkeɪt/", "v./n.", "提倡；拥护者", {"反对；驳斥", "忽视；漠视", "中立；旁观"}},
    {"aesthetic", "/esˈθetɪk/", "adj.", "审美的；美学的", {"实用的；功利的", "经济的；节约的", "道德的；伦理的"}},
    {"affiliate", "/əˈfɪlieɪt/", "v./n.", "使隶属；附属机构", {"分离；脱离", "独立；自治", "合并；吞并"}},
    {"agenda", "/əˈdʒendə/", "n.", "议程；日程", {"结论；决议", "记录；纪要", "报告；摘要"}},
    {"aggregate", "/ˈæɡrɪɡət/", "n./adj./v.", "合计；总计；聚集", {"分散；离散", "单独；个别", "减少；削减"}},
    {"alleviate", "/əˈliːvieɪt/", "v.", "减轻；缓解", {"加剧；恶化", "加重；增强", "引发；导致"}},
    {"allocate", "/ˈæləkeɪt/", "v.", "分配；拨给", {"收回；扣除", "集中；聚集", "浪费；滥用"}},
    {"ambiguous", "/æmˈbɪɡjuəs/", "adj.", "模棱两可的；含糊不清的", {"明确的；清楚的", "确定的；肯定的", "直白的；坦率的"}},
    {"amplify", "/ˈæmplɪfaɪ/", "v.", "放大；增强；扩大", {"缩小；削弱", "抑制；压制", "简化；精简"}},
    {"analogy", "/əˈnælədʒi/", "n.", "类比；比喻", {"对比；对照", "区别；差异", "例子；实例"}},
    {"anticipate", "/ænˈtɪsɪpeɪt/", "v.", "预期；预料；期待", {"回顾；回忆", "忽视；不预见", "失望；绝望"}},
    {"apparatus", "/ˌæpəˈrætəs/", "n.", "器械；装置；设备", {"原料；材料", "理论；原理", "人员；员工"}},
    {"articulate", "/ɑːrˈtɪkjuleɪt/", "adj./v.", "表达清晰的；清晰地说", {"含糊的；口齿不清的", "沉默的；寡言的", "混乱的；无序的"}},
    {"ascend", "/əˈsend/", "v.", "上升；攀登", {"下降；降落", "后退；撤退", "下沉；沉没"}},
    {"attribute", "/əˈtrɪbjuːt/", "v./n.", "归因于；属性", {"推断；推论", "否认；否定", "转移；挪作他用"}},
    {"authentic", "/ɔːˈθentɪk/", "adj.", "真实的；可信的；正宗的", {"伪造的；假冒的", "虚假的；不实的", "现代的；新式的"}},
    {"avert", "/əˈvɜːrt/", "v.", "避免；转移；使避开", {"吸引；招致", "面对；正视", "引发；导致"}},
    {"benchmark", "/ˈbentʃmɑːrk/", "n.", "基准；标杆", {"例外；特例", "上限；极限", "起点；开端"}},
    {"bewilder", "/bɪˈwɪldər/", "v.", "使迷惑；使不知所措", {"使清醒；使明白", "使兴奋；使激动", "使平静；使镇定"}},
    {"bias", "/ˈbaɪəs/", "n./v.", "偏见；偏向", {"客观；公正", "一致；统一", "明确；清晰"}},
    {"bizarre", "/bɪˈzɑːr/", "adj.", "奇异的；怪诞的", {"普通的；寻常的", "合理的；正常的", "熟悉的；常见的"}},
    {"blur", "/blɜːr/", "v./n.", "使模糊；模糊不清", {"使清晰；使聚焦", "使明亮；使发光", "使扩大；使放大"}},
    {"bolster", "/ˈboʊlstər/", "v.", "支持；增强；提振", {"削弱；损害", "反对；抵制", "破坏；拆毁"}},
    {"bureaucracy", "/bjʊˈrɑːkrəsi/", "n.", "官僚机构；官僚作风", {"民主制度", "自治组织", "市场机制"}},
    {"catastrophe", "/kəˈtæstrəfi/", "n.", "灾难；大祸", {"好运；福气", "胜利；成功", "机遇；转机"}},
    {"cease", "/siːs/", "v.", "停止；终止", {"开始；着手", "继续；持续", "加速；促进"}},
    {"coherent", "/koʊˈhɪrənt/", "adj.", "连贯的；一致的；有条理的", {"混乱的；不连贯的", "矛盾的；对立的", "模糊的；含糊的"}},
    {"coincide", "/ˌkoʊɪnˈsaɪd/", "v.", "同时发生；一致；重合", {"错开；不一致", "冲突；抵触", "分离；断开"}},
    {"commodity", "/kəˈmɑːdəti/", "n.", "商品；日用品", {"服务；劳务", "资本；资金", "资源；能源"}},
    {"compensate", "/ˈkɑːmpenseɪt/", "v.", "补偿；赔偿", {"惩罚；处罚", "索取；索要", "浪费；耗费"}},
    {"competent", "/ˈkɑːmpɪtənt/", "adj.", "有能力的；胜任的", {"无能的；不胜任的", "粗心的；马虎的", "懒惰的；懈怠的"}},
    {"comprehensive", "/ˌkɑːmprɪˈhensɪv/", "adj.", "全面的；综合的", {"片面的；局部的", "简略的；概要的", "肤浅的；表面的"}},
    {"compromise", "/ˈkɑːmprəmaɪz/", "n./v.", "妥协；折中", {"坚持；不让步", "决裂；破裂", "胜利；完胜"}},
    {"conceal", "/kənˈsiːl/", "v.", "隐藏；隐瞒", {"揭露；暴露", "公开；宣布", "展示；陈列"}},
    {"conceive", "/kənˈsiːv/", "v.", "构想；想象；怀孕", {"放弃；舍弃", "误解；曲解", "证实；确认"}},
    {"consecutive", "/kənˈsekjətɪv/", "adj.", "连续的；接连的", {"间断的；间歇的", "分散的；零散的", "交替的；轮换的"}},
    {"consensus", "/kənˈsensəs/", "n.", "共识；一致意见", {"分歧；争议", "反对；异议", "决定；裁决"}},
    {"constrain", "/kənˈstreɪn/", "v.", "限制；约束；强迫", {"释放；解放", "鼓励；促进", "扩大；扩展"}},
    {"contaminate", "/kənˈtæmɪneɪt/", "v.", "污染；弄脏；玷污", {"净化；清洁", "保护；保存", "改善；改良"}},
    {"contemplate", "/ˈkɑːntəmpleɪt/", "v.", "沉思；考虑；凝视", {"忽略；无视", "匆忙决定；草率从事", "遗忘；忘却"}}
};

// ===================== 游戏逻辑 =====================
enum class Mode { CET4 = 1, CET6 = 2, Mixed = 3 };
enum class Phase { Start, Question, Feedback, End };

struct GameState {
    int hp = 100, score = 0, combo = 0, correct = 0, wrong = 0, wave = 1;
};
struct Question {
    const WordEntry* entry = nullptr;
    std::array<std::string, 4> options{};
    char correctLetter = 'A';
};

static const int TOTAL_WAVES = 10;
static const int PLAYER_HP_MAX = 100;

static const wchar_t* MONSTER_NAMES[5] = {L"单词史莱姆", L"语法骷髅", L"阅读石像鬼", L"听力幽灵", L"词汇巨龙"};
static const wchar_t* MONSTER_ARTS[5] = {
    L"   /\\_/\\   \n  ( o.o )  \n   > ^ <   ",
    L"   .-.    \n  (o.o)   \n  /|_|\\   \n   / \\    ",
    L"  /\\_/\\   \n {(>.<)}  \n  /| |\\   \n _/   \\_  ",
    L"   ~~~    \n  (o.o)   \n  (   )   \n ~~~~~~~  ",
    L"   /\\____ \n  < o.o ]----\n   \\_^_/   \n  /     \\  "
};

static std::wstring utf8ToWide(const std::string& s) {
    if (s.empty()) return L"";
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    if (n <= 0) return L"";
    std::wstring w(n, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], n);
    w.resize(n - 1);
    return w;
}
static std::wstring toW(int v) { return std::to_wstring(v); }
static void setText(HWND h, const std::wstring& s) { SetWindowTextW(h, s.c_str()); }

static int monsterHpForWave(int w) { return 30 + (w - 1) * 8; }
static int attackDamage(int combo) { return 10 + std::min(combo, 5) * 2; }
static int counterDamage(int wave) { return std::min(8 + wave * 2, 24); }

static std::mt19937 g_rng(std::random_device{}());
static std::set<std::string> g_used;

static const std::vector<WordEntry>* choosePool(Mode mode, int wave) {
    if (mode == Mode::CET4) return &CET4_WORDS;
    if (mode == Mode::CET6) return &CET6_WORDS;
    double p = wave <= 3 ? 0.15 : (wave <= 6 ? 0.50 : 0.80);
    std::uniform_real_distribution<double> d(0.0, 1.0);
    return d(g_rng) < p ? &CET6_WORDS : &CET4_WORDS;
}

static Question makeQuestion(Mode mode, int wave) {
    const std::vector<WordEntry>* pool = choosePool(mode, wave);
    std::vector<size_t> cand;
    for (size_t i = 0; i < pool->size(); ++i)
        if (g_used.find(pool->at(i).word) == g_used.end()) cand.push_back(i);
    if (cand.empty()) { g_used.clear(); for (size_t i = 0; i < pool->size(); ++i) cand.push_back(i); }
    std::uniform_int_distribution<size_t> di(0, cand.size() - 1);
    const WordEntry* e = &pool->at(cand[di(g_rng)]);
    g_used.insert(e->word);

    Question q; q.entry = e;
    std::vector<std::string> opts; opts.push_back(e->meaning);
    for (const auto& d : e->distractors)
        if (d != e->meaning && std::find(opts.begin(), opts.end(), d) == opts.end()) opts.push_back(d);
    if (opts.size() < 4) {
        std::vector<std::string> extra;
        for (const auto* p : {&CET4_WORDS, &CET6_WORDS})
            for (const auto& w : *p) if (w.meaning != e->meaning) extra.push_back(w.meaning);
        std::shuffle(extra.begin(), extra.end(), g_rng);
        for (const auto& x : extra) {
            if (opts.size() >= 4) break;
            if (std::find(opts.begin(), opts.end(), x) == opts.end()) opts.push_back(x);
        }
    }
    std::shuffle(opts.begin(), opts.end(), g_rng);
    for (size_t i = 0; i < opts.size(); ++i) {
        q.options[i] = opts[i];
        if (opts[i] == e->meaning) q.correctLetter = static_cast<char>('A' + i);
    }
    return q;
}

// ===================== 界面控件 ID =====================
enum {
    ID_STATUS = 100, ID_PLABEL, ID_PBAR, ID_PHP, ID_MLABEL, ID_MBAR, ID_MHP,
    ID_SCORE, ID_ART, ID_QUESTION, ID_WORDINFO, ID_FEEDBACK, ID_NEXT,
    ID_OPT_A = 200, ID_OPT_B, ID_OPT_C, ID_OPT_D,
    ID_ST_TITLE = 300, ID_ST_SUB, ID_R4, ID_R6, ID_RMIX, ID_START, ID_ST_TIPS,
    ID_END_TITLE = 400, ID_END_STATS, ID_RESTART, ID_QUIT
};

static HWND hStatus, hPLabel, hPBar, hPHP, hMLabel, hMBar, hMHP;
static HWND hScore, hArt, hQuestion, hWordInfo, hFeedback, hNext;
static HWND hOpt[4], hStTitle, hStSub, hR4, hR6, hRMix, hStart, hStTips;
static HWND hEndTitle, hEndStats, hRestart, hQuit;

static Phase g_phase = Phase::Start;
static Mode g_mode = Mode::Mixed;
static GameState g_state;
static Question g_q;
static int g_monsterHp = 0;
static int g_monsterMax = 0;
static bool g_victory = false;

static void showCtrl(HWND h, bool show) { ShowWindow(h, show ? SW_SHOW : SW_HIDE); }

static void applyPhase(Phase p) {
    g_phase = p;
    bool game = (p == Phase::Question || p == Phase::Feedback);
    showCtrl(hStatus, game); showCtrl(hPLabel, game); showCtrl(hPBar, game); showCtrl(hPHP, game);
    showCtrl(hMLabel, game); showCtrl(hMBar, game); showCtrl(hMHP, game);
    showCtrl(hScore, game); showCtrl(hArt, game); showCtrl(hQuestion, game); showCtrl(hWordInfo, game);
    showCtrl(hFeedback, game);
    for (int i = 0; i < 4; ++i) { showCtrl(hOpt[i], game); EnableWindow(hOpt[i], p == Phase::Question); }
    showCtrl(hNext, p == Phase::Feedback);
    showCtrl(hStTitle, p == Phase::Start); showCtrl(hStSub, p == Phase::Start);
    showCtrl(hR4, p == Phase::Start); showCtrl(hR6, p == Phase::Start); showCtrl(hRMix, p == Phase::Start);
    showCtrl(hStart, p == Phase::Start); showCtrl(hStTips, p == Phase::Start);
    showCtrl(hEndTitle, p == Phase::End); showCtrl(hEndStats, p == Phase::End);
    showCtrl(hRestart, p == Phase::End); showCtrl(hQuit, p == Phase::End);
}

static void refreshHud() {
    const wchar_t* mn = L"混合模式";
    if (g_mode == Mode::CET4) mn = L"四级模式";
    else if (g_mode == Mode::CET6) mn = L"六级模式";
    setText(hStatus, std::wstring(L"模式：") + mn + L"    波次：" + toW(g_state.wave) + L"/" + toW(TOTAL_WAVES));
    setText(hPLabel, L"勇者 HP");
    SendMessageW(hPBar, PBM_SETRANGE, 0, MAKELPARAM(0, PLAYER_HP_MAX));
    SendMessageW(hPBar, PBM_SETPOS, std::max(0, g_state.hp), 0);
    setText(hPHP, toW(std::max(0, g_state.hp)) + L"/" + toW(PLAYER_HP_MAX));
    int ai = std::min((g_state.wave - 1) / 2, 4);
    setText(hMLabel, std::wstring(L"怪物 [") + MONSTER_NAMES[ai] + L"] HP");
    SendMessageW(hMBar, PBM_SETRANGE, 0, MAKELPARAM(0, g_monsterMax));
    SendMessageW(hMBar, PBM_SETPOS, std::max(0, g_monsterHp), 0);
    setText(hMHP, toW(std::max(0, g_monsterHp)) + L"/" + toW(g_monsterMax));
    setText(hScore, L"分数：" + toW(g_state.score) + L"    连击：x" + toW(g_state.combo) +
                    L"    答对：" + toW(g_state.correct) + L"    答错：" + toW(g_state.wrong));
    setText(hArt, MONSTER_ARTS[ai]);
}

static void newWave() {
    g_monsterMax = monsterHpForWave(g_state.wave);
    g_monsterHp = g_monsterMax;
}

static void askQuestion() {
    g_q = makeQuestion(g_mode, g_state.wave);
    refreshHud();
    setText(hQuestion, L"请选出下列单词的正确中文释义：");
    setText(hWordInfo, utf8ToWide(g_q.entry->word) + L"    " + utf8ToWide(g_q.entry->phonetic) + L"    " + utf8ToWide(g_q.entry->pos));
    for (int i = 0; i < 4; ++i) {
        std::wstring s(1, static_cast<wchar_t>('A' + i));
        s += L". " + utf8ToWide(g_q.options[i]);
        setText(hOpt[i], s);
        EnableWindow(hOpt[i], TRUE);
    }
    setText(hFeedback, L"点击下方 A / B / C / D 作答……");
    applyPhase(Phase::Question);
}

static void startGame() {
    g_state = GameState();
    g_used.clear();
    newWave();
    askQuestion();
}

static void answer(int idx) {
    char pick = static_cast<char>('A' + idx);
    bool hit = (pick == g_q.correctLetter);
    std::wstring fb;
    if (hit) {
        int dmg = attackDamage(g_state.combo);
        g_monsterHp -= dmg;
        g_state.score += 10 + std::min(g_state.combo, 5) * 2;
        ++g_state.combo; ++g_state.correct;
        fb = L"命中！造成 " + toW(dmg) + L" 点伤害，怪物剩余 " + toW(std::max(0, g_monsterHp)) + L" HP。\r\n";
        fb += L"正确释义：" + std::wstring(1, g_q.correctLetter) + L". " + utf8ToWide(g_q.entry->meaning);
        fb += L"    连击 x" + toW(g_state.combo);
    } else {
        int dmg = counterDamage(g_state.wave);
        g_state.hp -= dmg;
        g_state.combo = 0; ++g_state.wrong;
        fb = L"答错！你选择了 " + std::wstring(1, pick) + L" ，怪物反击！\r\n";
        fb += L"正确答案：" + std::wstring(1, g_q.correctLetter) + L". " + utf8ToWide(g_q.entry->meaning) + L"\r\n";
        fb += L"你受到 " + toW(dmg) + L" 点伤害，剩余 " + toW(std::max(0, g_state.hp)) + L" HP。连击已清空。";
    }
    if (g_monsterHp <= 0) {
        int bonus = 30 + g_state.wave * 10;
        g_state.score += bonus;
        fb += L"\r\n击败怪物！本波奖励 +" + toW(bonus) + L" 分。";
    }
    setText(hFeedback, fb);
    setText(hNext, (g_state.hp <= 0 || g_monsterHp <= 0) ? L"继续" : L"下一题");
    refreshHud();
    applyPhase(Phase::Feedback);
}

static void showEndStats();

static void continueGame() {
    if (g_state.hp <= 0) {
        g_victory = false;
        applyPhase(Phase::End);
        setText(hEndTitle, L"阵亡……勇者倒下了");
        showEndStats();
        return;
    }
    if (g_monsterHp <= 0) {
        if (g_state.wave >= TOTAL_WAVES) {
            g_state.score += 200; g_victory = true;
            applyPhase(Phase::End);
            setText(hEndTitle, L"通关！英语勇者凯旋");
            showEndStats();
            return;
        }
        ++g_state.wave;
        newWave();
    }
    askQuestion();
}

static void showEndStats() {
    int total = g_state.correct + g_state.wrong;
    int acc = total > 0 ? static_cast<int>(1.0 * g_state.correct / total * 100 + 0.5) : 0;
    setText(hEndStats,
        L"最终分数：" + toW(g_state.score) + L"\r\n\r\n" +
        L"答对：" + toW(g_state.correct) + L" 题    答错：" + toW(g_state.wrong) + L" 题\r\n" +
        L"正确率：" + toW(acc) + L"%\r\n\r\n" +
        L"通过波次：" + toW(std::min(g_state.wave - (g_monsterHp > 0 ? 0 : 1), TOTAL_WAVES)) + L"/" + toW(TOTAL_WAVES) + L"\r\n" +
        (g_victory ? L"通关奖励 +200 分已计入。" : L"再接再厉，背好单词再来挑战！"));
}

// ===================== 窗口过程 =====================
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
            INITCOMMONCONTROLSEX icc = {sizeof(icc), ICC_PROGRESS_CLASS | ICC_BAR_CLASSES};
            InitCommonControlsEx(&icc);
            HFONT hFont = CreateFontW(19, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH, L"Microsoft YaHei");
            HFONT hBig = CreateFontW(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH, L"Microsoft YaHei");
            HFONT hArtFont = CreateFontW(17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                FIXED_PITCH, L"Consolas");

            auto mkStatic = [&](int id, int x, int y, int w, int h, HFONT f) {
                HWND c = CreateWindowExW(0, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_LEFT,
                                         x, y, w, h, hwnd, reinterpret_cast<HMENU>(id),
                                         reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), nullptr);
                SendMessageW(c, WM_SETFONT, reinterpret_cast<WPARAM>(f), TRUE);
                return c;
            };
            auto mkButton = [&](int id, const wchar_t* text, int x, int y, int w, int h, HFONT f) {
                HWND c = CreateWindowExW(0, L"Button", text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                         x, y, w, h, hwnd, reinterpret_cast<HMENU>(id),
                                         reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), nullptr);
                SendMessageW(c, WM_SETFONT, reinterpret_cast<WPARAM>(f), TRUE);
                return c;
            };

            hStatus = mkStatic(ID_STATUS, 20, 14, 820, 28, hFont);
            hPLabel = mkStatic(ID_PLABEL, 20, 52, 90, 24, hFont);
            hPBar = CreateWindowExW(0, PROGRESS_CLASSW, L"", WS_CHILD | WS_VISIBLE,
                                    110, 55, 230, 18, hwnd, reinterpret_cast<HMENU>(ID_PBAR),
                                    reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), nullptr);
            hPHP = mkStatic(ID_PHP, 350, 52, 90, 24, hFont);
            hMLabel = mkStatic(ID_MLABEL, 460, 52, 160, 24, hFont);
            hMBar = CreateWindowExW(0, PROGRESS_CLASSW, L"", WS_CHILD | WS_VISIBLE,
                                    620, 55, 200, 18, hwnd, reinterpret_cast<HMENU>(ID_MBAR),
                                    reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), nullptr);
            hMHP = mkStatic(ID_MHP, 830, 52, 100, 24, hFont);
            hScore = mkStatic(ID_SCORE, 20, 86, 820, 26, hFont);
            hArt = CreateWindowExW(0, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_LEFT,
                                   300, 118, 280, 108, hwnd, reinterpret_cast<HMENU>(ID_ART),
                                   reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), nullptr);
            SendMessageW(hArt, WM_SETFONT, reinterpret_cast<WPARAM>(hArtFont), TRUE);
            hQuestion = mkStatic(ID_QUESTION, 24, 236, 820, 26, hFont);
            hWordInfo = mkStatic(ID_WORDINFO, 24, 266, 820, 26, hFont);
            hOpt[0] = mkButton(ID_OPT_A, L"A.", 40, 304, 380, 48, hFont);
            hOpt[1] = mkButton(ID_OPT_B, L"B.", 440, 304, 380, 48, hFont);
            hOpt[2] = mkButton(ID_OPT_C, L"C.", 40, 362, 380, 48, hFont);
            hOpt[3] = mkButton(ID_OPT_D, L"D.", 440, 362, 380, 48, hFont);
            hFeedback = mkStatic(ID_FEEDBACK, 24, 424, 820, 78, hFont);
            hNext = mkButton(ID_NEXT, L"下一题", 330, 512, 200, 42, hFont);

            hStTitle = CreateWindowExW(0, L"Static", L"英语勇者 · 单词讨伐战", WS_CHILD | WS_VISIBLE | SS_CENTER,
                40, 70, 780, 56, hwnd, reinterpret_cast<HMENU>(ID_ST_TITLE),
                reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), nullptr);
            SendMessageW(hStTitle, WM_SETFONT, reinterpret_cast<WPARAM>(hBig), TRUE);
            hStSub = mkStatic(ID_ST_SUB, 40, 134, 780, 30, hFont);
            SetWindowTextW(hStSub, L"答对单词题才能攻击怪物，答错会被反击！共 10 波，通关或阵亡结束。");
            hR4 = mkButton(ID_R4, L"四级模式（全程四级核心词）", 250, 200, 360, 44, hFont);
            hR6 = mkButton(ID_R6, L"六级模式（全程六级核心词）", 250, 256, 360, 44, hFont);
            hRMix = mkButton(ID_RMIX, L"混合模式（前期四级，后期六级）", 250, 312, 360, 44, hFont);
            hStart = mkButton(ID_START, L"开始挑战", 300, 390, 260, 52, hBig);
            hStTips = mkStatic(ID_ST_TIPS, 60, 470, 740, 90, hFont);
            SetWindowTextW(hStTips, L"混合模式难度递进：1-3 波以四级为主，4-6 波各半，7-10 波以六级为主。\r\n答对按连击加成造成伤害（最高 20），答错怪物反击并清空连击。");

            hEndTitle = CreateWindowExW(0, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER,
                40, 110, 780, 56, hwnd, reinterpret_cast<HMENU>(ID_END_TITLE),
                reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), nullptr);
            SendMessageW(hEndTitle, WM_SETFONT, reinterpret_cast<WPARAM>(hBig), TRUE);
            hEndStats = CreateWindowExW(0, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER,
                120, 190, 620, 200, hwnd, reinterpret_cast<HMENU>(ID_END_STATS),
                reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE)), nullptr);
            SendMessageW(hEndStats, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
            hRestart = mkButton(ID_RESTART, L"重新开始", 220, 420, 200, 52, hFont);
            hQuit = mkButton(ID_QUIT, L"退出", 440, 420, 200, 52, hFont);

            applyPhase(Phase::Start);
            return 0;
        }
        case WM_COMMAND: {
            int id = LOWORD(wp);
            if (id >= ID_OPT_A && id <= ID_OPT_D && g_phase == Phase::Question) {
                answer(id - ID_OPT_A);
            } else if (id == ID_NEXT && g_phase == Phase::Feedback) {
                continueGame();
            } else if (id == ID_R4) { g_mode = Mode::CET4; }
            else if (id == ID_R6) { g_mode = Mode::CET6; }
            else if (id == ID_RMIX) { g_mode = Mode::Mixed; }
            else if (id == ID_START) { startGame(); }
            else if (id == ID_RESTART) { applyPhase(Phase::Start); }
            else if (id == ID_QUIT) { PostQuitMessage(0); }
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hwnd, &ps);
            RECT rc; GetClientRect(hwnd, &rc);
            FillRect(dc, &rc, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_CTLCOLORSTATIC: {
            HDC dc = reinterpret_cast<HDC>(wp);
            SetBkMode(dc, TRANSPARENT);
            return reinterpret_cast<LRESULT>(GetStockObject(WHITE_BRUSH));
        }
        case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int cmdShow) {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = L"VocabHeroGUI";
    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowExW(0, L"VocabHeroGUI", L"英语勇者 · 单词讨伐战",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600, nullptr, nullptr, hInst, nullptr);
    if (!hwnd) return 1;
    ShowWindow(hwnd, cmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
