//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2002-2012 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef __MSYNTHESIZER_H__
#define __MSYNTHESIZER_H__

#include <atomic>
#include "effects/effect.h"
#include "libmscore/synthesizerstate.h"

namespace Ms {

struct MidiPatch;
class NPlayEvent;
class Synthesizer;
class Effect;
class Xml;

//---------------------------------------------------------
//   MasterSynthesizer
//    hosts several synthesizers
//---------------------------------------------------------

class MasterSynthesizer : public QObject {
      Q_OBJECT

      float _gain             { 0.1   };     // -20dB
      float _boost            { 10.0  };     // +20dB
      double _masterTuning    { 440.0 };

   public:
      static const int MAX_BUFFERSIZE = 8192;
      static const int MAX_EFFECTS = 2;

   private:
      std::atomic<bool> lock1      { false };
      std::atomic<bool> lock2      { true  };
      std::vector<Synthesizer*> _synthesizer;
      std::vector<Effect*> _effectList[MAX_EFFECTS];
      Effect* _effect[MAX_EFFECTS]  { nullptr, nullptr };

      float _sampleRate;

      float effect1Buffer[MAX_BUFFERSIZE];
      float effect2Buffer[MAX_BUFFERSIZE];
      int indexOfEffect(int ab, const QString& name);

   public slots:
      void sfChanged() { emit soundFontChanged(); }
      void setGain(float f);

   signals:
      void soundFontChanged();
      void gainChanged(float);

   public:
      MasterSynthesizer();
      ~MasterSynthesizer();
      void registerSynthesizer(Synthesizer*);

      void init();

      float sampleRate()            { return _sampleRate; }
      void setSampleRate(float val);

      void process(unsigned, float*);
      void play(const NPlayEvent&, unsigned);

      void setMasterTuning(double val);
      double masterTuning() const      { return _masterTuning; }

      int index(const QString&) const;
      QString name(unsigned) const;

      QList<MidiPatch*> getPatchInfo() const;

      SynthesizerState state() const;
      bool setState(const SynthesizerState&);

      Synthesizer* synthesizer(const QString& name);
      const std::vector<Effect*>& effectList(int ab) const { return _effectList[ab]; }
      const std::vector<Synthesizer*> synthesizer() const { return _synthesizer; }
      void registerEffect(int ab, Effect*);

      void reset();
      void allSoundsOff(int channel);
      void allNotesOff(int channel);

      void setEffect(int ab, int idx);
      Effect* effect(int ab);
      int indexOfEffect(int ab);

      float gain() const     { return _gain; }
      float boost() const    { return _boost; }
      void setBoost(float v) { _boost = v; }
      };

}
#endif

