#include "tibetan.h"
#include <rime/gear/translator_commons.h>
#include <rime/gear/grammar.h>

namespace rime {

TibetanTranslator::TibetanTranslator(const Ticket& ticket)
    : Translator(ticket), TranslatorOptions(ticket) {
    InitializeMaps();
}

void TibetanTranslator::InitializeMaps() {
    // 基本辅音
    consonant_map_ = {
        {"k", "ཀ"}, {"kh", "ཁ"}, {"g", "ག"}, {"ng", "ང"},
        {"c", "ཅ"}, {"ch", "ཆ"}, {"j", "ཇ"}, {"ny", "ཉ"},
        {"t", "ཏ"}, {"th", "ཐ"}, {"d", "ད"}, {"n", "ན"},
        {"p", "པ"}, {"ph", "ཕ"}, {"b", "བ"}, {"m", "མ"},
        {"ts", "ཙ"}, {"tsh", "ཚ"}, {"dz", "ཛ"}, {"w", "ཝ"},
        {"zh", "ཞ"}, {"z", "ཟ"}, {"'", "འ"}, {"y", "ཡ"},
        {"r", "ར"}, {"l", "ལ"}, {"sh", "ཤ"}, {"s", "ས"},
        {"h", "ཧ"}, {"a", "ཨ"}
    };
    
    // 元音
    vowel_map_ = {
        {"i", "ི"}, {"u", "ུ"}, {"e", "ེ"}, {"o", "ོ"}
    };
    
    // 下加字
    subjoined_map_ = {
        {"y", "ྱ"}, {"r", "ྲ"}, {"l", "ླ"}, {"w", "ྭ"}
    };
    
    // 特殊组合
    special_map_ = {
        {"k+y", "ཀྱ"}, {"kh+y", "ཁྱ"}, {"g+y", "གྱ"},
        {"p+y", "པྱ"}, {"ph+y", "ཕྱ"}, {"b+y", "བྱ"},
        {"m+y", "མྱ"}, {"k+r", "ཀྲ"}, {"kh+r", "ཁྲ"},
        {"g+r", "གྲ"}, {"t+r", "ཏྲ"}, {"th+r", "ཐྲ"},
        {"d+r", "དྲ"}, {"n+r", "ནྲ"}, {"p+r", "པྲ"},
        {"ph+r", "ཕྲ"}, {"b+r", "བྲ"}, {"m+r", "མྲ"},
        {"k+l", "ཀླ"}, {"g+l", "གླ"}, {"b+l", "བླ"},
        {"r+l", "རླ"}, {"s+l", "སླ"}, {"sh+r", "ཤྲ"},
        {"s+r", "སྲ"}, {"h+r", "ཧྲ"}
    };
}

bool TibetanTranslator::IsValidInput(const string& input) {
    // 检查输入是否只包含有效的藏文拉丁转写字符
    static const regex valid_input("[a-z'+ ]*");
    return regex_match(input, valid_input);
}

string TibetanTranslator::LatinToTibetan(const string& input) {
    if (input.empty()) return "";
    
    // 先检查特殊组合
    for (const auto& pair : special_map_) {
        if (input.find(pair.first) == 0) {
            return pair.second + LatinToTibetan(input.substr(pair.first.length()));
        }
    }
    
    // 检查辅音
    for (const auto& pair : consonant_map_) {
        if (input.find(pair.first) == 0) {
            string remaining = input.substr(pair.first.length());
            string result = pair.second;
            
            // 处理下加字
            for (const auto& sub_pair : subjoined_map_) {
                if (remaining.find(sub_pair.first) == 0) {
                    result += sub_pair.second;
                    remaining = remaining.substr(sub_pair.first.length());
                    break;
                }
            }
            
            // 处理元音
            for (const auto& vowel_pair : vowel_map_) {
                if (remaining.find(vowel_pair.first) == 0) {
                    result += vowel_pair.second;
                    remaining = remaining.substr(vowel_pair.first.length());
                    break;
                }
            }
            
            return result + LatinToTibetan(remaining);
        }
    }
    
    return input;  // 无法转换的部分原样返回
}

string TibetanTranslator::ProcessTibetanSyllable(const string& input) {
    string result = LatinToTibetan(input);
    
    // 添加音节分隔符（空格）
    if (!result.empty() && result.back() != ' ') {
        result += " ";
    }
    
    return result;
}

an<Translation> TibetanTranslator::Query(const string& input, const Segment& segment) {
    if (!IsValidInput(input)) {
        return nullptr;
    }
    
    auto translation = New<PhraseTranslation>(this);
    string tibetan = ProcessTibetanSyllable(input);
    
    if (!tibetan.empty()) {
        translation->Append(New<SimpleCandidate>(
            "tibetan",
            segment.start,
            segment.end,
            tibetan,
            "藏文"
        ));
    }
    
    return translation->empty() ? nullptr : translation;
}

TibetanEngine::TibetanEngine(const Ticket& ticket) : Engine(ticket) {
    // 初始化引擎
}

void TibetanEngine::ApplySchema(Schema* schema) {
    Engine::ApplySchema(schema);
    // 自定义schema应用逻辑
}

void TibetanEngine::SetSchema(Schema* schema) {
    Engine::SetSchema(schema);
    // 自定义schema设置逻辑
}

an<Processor> TibetanEngine::CreateProcessor() {
    return Processor::Require("processor")->Create(Ticket(this));
}

an<Segmentor> TibetanEngine::CreateSegmentor() {
    return Segmentor::Require("segmentor")->Create(Ticket(this));
}

TibetanTranslatorComponent::TibetanTranslatorComponent() {
    BindTicket(&Ticket::schema);
}

Translator* TibetanTranslatorComponent::Create(const Ticket& ticket) {
    return new TibetanTranslator(ticket);
}

TibetanEngineComponent::TibetanEngineComponent() {
    BindTicket(&Ticket::schema);
}

Engine* TibetanEngineComponent::Create(const Ticket& ticket) {
    return new TibetanEngine(ticket);
}

RIME_REGISTER_COMPONENT(TibetanTranslatorComponent)
RIME_REGISTER_COMPONENT(TibetanEngineComponent)