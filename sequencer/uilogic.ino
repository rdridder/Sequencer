void processButtonPressLogic(int buttonValue) {
  if(buttonValue < 10) {
    // Do nothing
    return;
  } else if(buttonValue <  100) {
    // back
  } else if(buttonValue <  155) {
    // step
    uint8_t bpm = sequencer.getTempo() + 1;
    sequencer.setTempo(bpm);
    updateUI = true;   
    DEBUG_PRINTLN("tempo");
  } else if(buttonValue <  300) {
    // Step
    sequencer.nextStep();
    ui.setMaskValue(sequencer.getNote());
    updateUI = true;
    DEBUG_PRINTLN("step");
  } else if(buttonValue <  475) {
    // Mode 
    if(sequencer.getMode() == 0) {
      sequencer.setMode(1);
    } else {
      sequencer.setMode(0);
    }
    // 0 = edit
    // 1 = play
    updateUI = true;
    DEBUG_PRINTLN("play / pause");
  }
  DEBUG_PRINT("buttonValue ");
  DEBUG_PRINTLN(buttonValue);
}
