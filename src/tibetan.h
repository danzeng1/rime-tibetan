#ifndef RIME_TIBETAN_H_
#define RIME_TIBETAN_H_

#include <rime_api.h>
#include <rime/common.h>
#include <rime/component.h>
#include <rime/registry.h>
#include <rime/key_table.h>
#include <rime/schema.h>
#include <rime/translation.h>
#include <rime/translator.h>
#include <rime/processor.h>
#include <rime/engine.h>
#include <rime/context.h>
#include <rime/menu.h>
#include <rime/algo/algebra.h>
#include <rime/dict/dictionary.h>
#include <rime/dict/user_dictionary.h>
#include <rime/gear/translator_commons.h>

namespace rime {

class TibetanTranslator : public Translator, public TranslatorOptions {
public:
    TibetanTranslator(const Ticket& ticket);
    
    virtual an<Translation> Query(const string& input,
                                 const Segment& segment);
    
    // 藏文音节处理
    string ProcessTibetanSyllable(const string& input);
    
    // 拉丁转藏文转换规则
    string LatinToTibetan(const string& input);
    
    // 检查是否是有效输入
    bool IsValidInput(const string& input);
    
private:
    map<string, string> consonant_map_;
    map<string, string> vowel_map_;
    map<string, string> subjoined_map_;
    map<string, string> special_map_;
    void InitializeMaps();
};

class TibetanEngine : public Engine {
public:
    TibetanEngine(const Ticket& ticket);
    
    void ApplySchema(Schema* schema) override;
    void SetSchema(Schema* schema) override;
    
    // 自定义处理器
    an<Processor> CreateProcessor();
    
    // 自定义分段器
    an<Segmentor> CreateSegmentor();
};

class TibetanTranslatorComponent : public TibetanTranslator::Component {
public:
    TibetanTranslatorComponent();
    Translator* Create(const Ticket& ticket);
};

class TibetanEngineComponent : public TibetanEngine::Component {
public:
    TibetanEngineComponent();
    Engine* Create(const Ticket& ticket);
};

}  // namespace rime

#endif  // RIME_TIBETAN_H_