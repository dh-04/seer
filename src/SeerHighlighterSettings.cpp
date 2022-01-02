#include "SeerHighlighterSettings.h"
#include <QtCore/QList>

SeerHighlighterSettings::SeerHighlighterSettings () {
}

SeerHighlighterSettings::SeerHighlighterSettings (const SeerHighlighterSettings& other) {

    *this = other;
}

SeerHighlighterSettings::~SeerHighlighterSettings () {
}

SeerHighlighterSettings& SeerHighlighterSettings::operator== (const SeerHighlighterSettings& rhs) {

    _formats = rhs._formats;
}

QStringList SeerHighlighterSettings::keys () const {

    QList<QString> keylist = _formats.keys();

    QStringList keys;

    for (int i=0; i<keylist.count(); i++) {
        keys.push_back(keylist[i]);
    }

    return keys;
}

bool SeerHighlighterSettings::has (const QString& name) const {

    return _formats.contains(name);
}

QTextCharFormat SeerHighlighterSettings::get (const QString& name) const {

    return _formats[name];
}

void SeerHighlighterSettings::add (const QString& name, QTextCharFormat& format) {

    _formats[name] = format;
}

SeerHighlighterSettings SeerHighlighterSettings::populateForCPP () {

    SeerHighlighterSettings cppSettings;
    QTextCharFormat f;

    f.setFontWeight(QFont::Bold); f.setFontItalic(false); f.setForeground(Qt::darkMagenta);
    cppSettings.add("Class", f);

    f.setFontWeight(QFont::Normal); f.setFontItalic(false); f.setForeground(Qt::darkGreen);
    cppSettings.add("Quotation", f);

    f.setFontWeight(QFont::Normal); f.setFontItalic(true); f.setForeground(Qt::blue);
    cppSettings.add("Function", f);

    f.setFontWeight(QFont::Normal); f.setFontItalic(false); f.setForeground(Qt::red);
    cppSettings.add("Comment", f);

    f.setFontWeight(QFont::Normal); f.setFontItalic(false); f.setForeground(Qt::red);
    cppSettings.add("Multiline Comment", f);

    f.setFontWeight(QFont::Bold); f.setFontItalic(false); f.setForeground(Qt::darkBlue);
    cppSettings.add("Keyword", f);

    return cppSettings;
}

