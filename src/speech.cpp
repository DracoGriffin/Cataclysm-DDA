#include "speech.h"

#include <map>
#include <vector>
#include <utility>

#include "json.h"
#include "rng.h"
#include "translations.h"

std::map<std::string, std::vector<SpeechBubble> > speech;

SpeechBubble nullSpeech = { "hsss", 0 };

void load_speech( JsonObject &jo )
{
    const std::string label = jo.get_string( "speaker" );
    const std::string sound = _( jo.get_string( "sound" ) );
    const int volume = jo.get_int( "volume" );
    std::map<std::string, std::vector<SpeechBubble> >::iterator speech_type = speech.find( label );

    // Construct a vector matching the label if needed.
    if( speech_type == speech.end() ) {
        speech[ label ] = std::vector<SpeechBubble>();
        speech_type = speech.find( label );
    }

    const SpeechBubble speech = {sound, volume};

    speech_type->second.push_back( speech );
}

void reset_speech()
{
    speech.clear();
}

const SpeechBubble &get_speech( const std::string &label )
{
    const std::map<std::string, std::vector<SpeechBubble> >::iterator speech_type = speech.find(
                label );

    if( speech_type == speech.end() || speech_type->second.empty() ) {
        // Bad lookup, return a fake sound, also warn?
        return nullSpeech;
    }

    return random_entry_ref( speech_type->second );
}
